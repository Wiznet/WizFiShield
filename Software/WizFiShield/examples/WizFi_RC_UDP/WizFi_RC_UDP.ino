/******************************************************************
 WizFiShield UDP application to control RC Car.
 
 Circuit:
 WizFiShield connected to Arduino via SPI
 
 RST: pin 2  // Output
 DRDY: pin 3  // Input
 CSB: pin 4  // output

 MOSI: pin 11  // output
 MISO: pin 12  // input
 SCK: pin 13  // out
 
 Created 27 Sep. 2012
 by James YS Kim  (jameskim@wiznet.co.kr, javakys@gmail.com)
 
 Modified 27 May. 2013
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
#include <WizFiUDP.h>
#include <TimerOne.h>

#define SSID    ""        // SSID of your AP
#define Key     ""  // Key or Passphrase
// Wi-Fi security option (NO_SECURITY, WEP_SECURITY, WPA_SECURITY, WPA2PSK_SECURITY)
//#define Security        WPA_SECURITY

#define MAX_SOCK_NUM    4

#define LEFT_PWM     3
#define LEFT_1       4
#define LEFT_2       2
#define RIGHT_PWM    6
#define RIGHT_1      5
#define RIGHT_2      7

uint8_t current_speed;

unsigned int SrcPort = 35000;

WizFi2x0Class myWizFi;
WizFiUDP  myUDP;

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
  byte retval;
  
  Serial.begin(9600);
  Serial.println("\r\nSerial Init");
  
  // initalize WizFiShield
  myWizFi.SetPinMap(8, 9, 10);
  myWizFi.begin();
  myUDP = WizFiUDP((uint8_t *)NULL, 0, SrcPort);

  pinSetup();
  driveStop();
 
  current_speed = 64; 
 
  // Timer1 Initialize
  Timer1.initialize(1000); // 1msec
  Timer1.attachInterrupt(Timer1_ISR);
 
  myWizFi.SendSync();
  myWizFi.ReplyCheckTimer.TimerStart(3000);

  Serial.println("Send Sync data");
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
    
    retval = myWizFi.associate(SSID, Key, Security, true);
    
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
  
  if(myUDP.open())
    Serial.println("UDP socket created successfully");
  else
    Serial.println("UDP socket creation failed");
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
    if(myUDP.available())
    {
      if(myUDP.read(rcvdBuf))
      {
        if(rcvdBuf[0] == '1') //Backward , right wheel
        {
          rightBwd();
        }else if(rcvdBuf[0] == '2') //Forward
        {
          rightFwd();
        }else if(rcvdBuf[0] == '0') //Stop
        {
          rightStop();
        }

        if(rcvdBuf[1] == '1') //Backward , left wheel
        {
          leftBwd();
        }else if(rcvdBuf[1] == '2') //Forward
        {
          leftFwd();
        }else if(rcvdBuf[1] == '0') //Stop
        {
          leftStop();
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

