/******************************************************************
 WizFiShield UDP application to control RC Car.

 Circuit:
 WizFiShield connected to Arduino via SPI
 
# Default pin assignment is as followings
==============================================
// Default SPI pin map
 MOSI: pin 11  // output
 MISO: pin 12  // input
 SCK:  pin 13  // out

// Additional I/O for control
 RST:  pin 2  // Output
 DRDY: pin 3  // Input
 CSB:  pin 4  // output
==============================================
 
 Created 27 Sep. 2012
 by James YS Kim  (jameskim@wiznet.co.kr, javakys@gmail.com)
 
 Modified 27 May. 2013
 by Jinbuhm Kim  (jbkim@wiznet.co.kr, jinbuhm.kim@gmail.com)
 
 Modified 2 August. 2013 
 -. Some comments added
 -. Fix SPI & Motor control pin map conflict.
 by Jinbuhm Kim  (jbkim@wiznet.co.kr, jinbuhm.kim@gmail.com)
 
*****************************************************************/

// WizFiShield communicates using SPI, so include the SPI library:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPI.h>
#include <WizFi2x0.h>
#include <WizFiClient.h>
#include <WizFiServer.h>
#include <TimerOne.h>
#include <HTMLParser.h>
#include <HTMLPages.h>

byte IsTimer1Expired = 0;
uint16_t CurrentTime = 0;

#define SSID 	  ""        // SSID of your AP
#define Key 	  ""  // Key or Passphrase
// Wi-Fi security option (NO_SECURITY, WEP_SECURITY, WPA_SECURITY, WPA2PSK_SECURITY)
// Define correct Wi-Fi security according to your wi-fi environment 
//#define Security        WPA_SECURITY

#define MAX_SOCK_NUM    4

// Pin assignment for Motor control
// ===================================
#define LEFT_PWM     10
#define LEFT_1       9  
#define LEFT_2       8
#define RIGHT_PWM    6
#define RIGHT_1      5
#define RIGHT_2      7
// ===================================

uint8_t current_speed;

unsigned int SrcPort = 80;

WizFi2x0Class myWizFi;
WizFiClient myClient[MAX_SOCK_NUM];//(SIP, ServerPort);
WizFiServer myServer(SrcPort);
HTMLParser myHTMLParser;
HTMLPages myHTMLPages;

boolean Wifi_setup = false;
boolean ConnectionState = false;
boolean Disconnect_flag = false;
boolean Connect_flag = false;

///////////////////////////////
// 1msec Timer
void Timer1_ISR()
{
  myWizFi.ReplyCheckTimer.CheckIsTimeout();
}
//
//////////////////////////////

void setup() {
  byte retval, i;
  
  Serial.begin(9600);
  Serial.println("\r\nSerial Init");
  
  for(i=0; i<MAX_SOCK_NUM; i++) myClient[i] =  WizFiClient();
  myWizFi.begin();
 
  // Timer1 Initialize
  Timer1.initialize(1000); // 1msec
  Timer1.attachInterrupt(Timer1_ISR);
 
  myWizFi.SendSync();
  myWizFi.ReplyCheckTimer.TimerStart(3000);
  Serial.println("Send Sync data");
  
  pinSetup();
  driveStop();
  
  current_speed = 64;
  
  while(1)
  {
    if(myWizFi.CheckSyncReply())
    {
      myWizFi.ReplyCheckTimer.TimerStop();
      Serial.println("Rcvd Sync data");
      break;
    }
    if(myWizFi.ReplyCheckTimer.GetIsTimeout())
    {
      Serial.println("Rcving Sync Timeout!!");
      // Nothing to do forever;
      for(;;)
      ;
    }
  }

  ////////////////////////////////////////////////////////////////////////////
  // AP association  
  while(1)
  {
    byte tmpstr[32];
    
    retval = myWizFi.associate(SSID, Key, WPA_SECURITY, true);
    
    if(retval == 1){
      myWizFi.GetSrcIPAddr(tmpstr);
      Serial.println("AP association Success");
      Serial.print("MY IPAddress: ");
      Serial.println((char *)tmpstr);
      Wifi_setup = true;
      break;
    }else{
      Serial.println("AP association Failed");
    }
  }
  
  if(myServer.begin())
    Serial.println("Server Listen OK");
  else
    Serial.println("Server Listen Failed");
}

void loop()
{
  uint8_t retval, i;
  byte rcvdBuf[129];
  byte cmd;
  char ch;
 
  memset(rcvdBuf, 0, 129);
   
  if(Wifi_setup)
  {  
    myWizFi.RcvPacket();
    for(i=0; i<MAX_SOCK_NUM; i++)
    {
      if(myClient[i].available()){    
         retval = myClient[i].read(rcvdBuf);   
         if(retval > 0)
         {
/*           
           Serial.print("CID[");
           Serial.print((char)myClient[i].GetCID());
           Serial.print("]");
           Serial.println((char *)rcvdBuf);
*/         
             myHTMLParser.Parsing((char *)rcvdBuf);
             
           if((rcvdBuf[retval - 1] == 0x0A) && (rcvdBuf[retval - 2] == 0x0D) && (rcvdBuf[retval - 3] == 0x0A) && (rcvdBuf[retval - 4] == 0x0D))
           {
             retval = myHTMLParser.GetParam();
             if(retval == FW_CMD)
             {
               Serial.println("FW_CMD Rcvd");
               myHTMLParser.SetParam(NO_CMD);
               moveForward();
             }else if(retval == BW_CMD)
             {
               Serial.println("BW_CMD Rcvd");
               myHTMLParser.SetParam(NO_CMD);
               moveBackward();
             }else if(retval == RT_CMD)
             {
               Serial.println("RT_CMD Rcvd");
               myHTMLParser.SetParam(NO_CMD);
               turnRight();
             }else if(retval == LF_CMD)
             {
               Serial.println("LW_CMD Rcvd");
               myHTMLParser.SetParam(NO_CMD);
               turnLeft();
             }
             
             while(myHTMLPages.GetNextLine((char *)rcvdBuf))
             {
               myClient[i].write((byte *)rcvdBuf);
             }
             delay(500);
             myClient[i].disconnect();
             driveStop();
           }
         }
       }
    }
  }
}
void pinSetup()
{
  // Motor control pins- the PWM pins are speed control while the other pins
  //  control the "mode" of operation- forward, reverse, stop, brake. These
  //  functions are abstracted in some utility functions below, so a user need
  //  only call the "driveFwd()" function, eg, to move the robot forward.
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LEFT_1, OUTPUT);
  pinMode(LEFT_2, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);
  pinMode(RIGHT_1, OUTPUT);
  pinMode(RIGHT_2, OUTPUT);
}

void driveStop()
{
  rightStop();
  leftStop();
}

void rightStop()
{
  digitalWrite(RIGHT_1, LOW);
  digitalWrite(RIGHT_2, LOW);
  analogWrite(RIGHT_PWM, 0);
}

void leftStop()
{
  digitalWrite(LEFT_1, LOW);
  digitalWrite(LEFT_2, LOW);
  analogWrite(LEFT_PWM, 0);
}

void moveForward()
{
  rightFwd();
  leftFwd();
}

void moveBackward()
{
  rightBwd();
  leftBwd();
}

void turnRight()
{
  rightBwd();
  leftFwd();
}

void turnLeft()
{
  rightFwd();
  leftBwd();
}

void rightFwd()
{
  digitalWrite(RIGHT_1, LOW);
  digitalWrite(RIGHT_2, HIGH);
  analogWrite(RIGHT_PWM, current_speed);
}

void rightBwd()
{
  digitalWrite(RIGHT_1, HIGH);
  digitalWrite(RIGHT_2, LOW);
  analogWrite(RIGHT_PWM, current_speed);
}

void leftFwd()
{
  digitalWrite(LEFT_1, LOW);
  digitalWrite(LEFT_2, HIGH);
  analogWrite(LEFT_PWM, current_speed);
}

void leftBwd()
{
  digitalWrite(LEFT_1, HIGH);
  digitalWrite(LEFT_2, LOW);
  analogWrite(LEFT_PWM, current_speed);
}

