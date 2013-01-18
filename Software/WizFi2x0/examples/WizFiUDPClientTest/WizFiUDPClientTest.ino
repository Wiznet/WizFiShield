/******************************************************************
 WizFi2x0 Shield test code for Arduino Uno
 
 Circuit:
 WizFi2x0 connected to Arduino via SPI
 
 RST: pin 2  // Output
 DRDY: pin 3  // Input
 CSB: pin 4  // output

 MOSI: pin 11  // output
 MISO: pin 12  // input
 SCK: pin 13  // out
 
 Created 18 Sep. 2012
 by James YS Kim  (jameskim@wiznet.co.kr, javakys@gmail.com)

*****************************************************************/

// WizFi210 communicates using SPI, so include the SPI library:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPI.h>
#include <WizFi2x0.h>
#include <WizFiUDP.h>
#include <TimerOne.h>

byte IsTimer1Expired = 0;
uint16_t CurrentTime = 0;

#define SSID 		""        // SSID of your AP
#define Key 	        ""    // Key or Passphrase

unsigned char  SIP[4] 	        = {192, 168, 123, 170};
unsigned int ServerPort = 50000;

WizFi2x0Class myWizFi;
WizFiUDP myUDP;
TimeoutClass ConnectInterval;

SPIChar spichar;

// pins used for the connection with the WizFi210

boolean Wifi_setup = false;
boolean ConnectionState = false;
boolean Disconnect_flag = false;
boolean Connect_flag = false;

///////////////////////////////
// 1msec Timer
void Timer1_ISR()
{
  uint8_t i;
  
  myWizFi.ReplyCheckTimer.CheckIsTimeout();
  ConnectInterval.CheckIsTimeout();
}
//
//////////////////////////////

void setup() {
  byte key, retval, i;
  byte retry_count = 0;
  byte tmpstr[64];
  
  Serial.begin(9600);
  Serial.println("\r\nSerial Init");
  
  // initalize WizFi2x0 module:
  myWizFi.begin();
  myUDP =  WizFiUDP(SIP, ServerPort, 50000);
   
  ConnectInterval.init();
  
  // Timer1 Initialize
  Timer1.initialize(1000); // 1msec
  Timer1.attachInterrupt(Timer1_ISR);

 
  myWizFi.SendSync();
  myWizFi.ReplyCheckTimer.TimerStart(1000);
  
  //Serial.println("Send Sync data");
  
  while(1)
  {
    if(myWizFi.CheckSyncReply())
    {
      myWizFi.ReplyCheckTimer.TimerStop();
      //Serial.println("Rcvd Sync data");
      break;
    }
    if(myWizFi.ReplyCheckTimer.GetIsTimeout())
    {
      //Serial.println("Rcving Sync Timeout!!");
      return;
    }
  }

  ////////////////////////////////////////////////////////////////////////////
  // AP association  
  while(1)
  {
    retval = myWizFi.associate(SSID, Key, WEP_SECURITY, true);
    
    if(retval == 1){
      Serial.println("WizFi2x0 AP Associated");
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

    ConnectInterval.TimerStart(100);  
}

void loop()
{
  uint8_t retval, i;
  byte rcvdBuf[129];
  byte cmd;
  byte TxBuf[100];
 
  memset(rcvdBuf, 0, 129);
  
  if(Wifi_setup)
  {  
     myWizFi.RcvPacket();
     
     if(myUDP.available())
     {
       if(ConnectInterval.GetIsTimeout())
       {
         myUDP.write((byte *)"Time elapsed..I'm alive");
         ConnectInterval.TimerStart();
         Serial.println("Time elapsed..I'm alive");
       }
     }
  }
}


