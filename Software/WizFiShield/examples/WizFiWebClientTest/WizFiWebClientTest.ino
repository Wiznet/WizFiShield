/******************************************************************
 WizFiShield Web Client Test Example
 
 This sketch connect to a website(http://www.google.com) using WizFi Shield
 
 Circuit:
 WizFiShield connected to Arduino via SPI
 
 RST: pin 2  // Output
 DRDY: pin 3  // Input
 CSB: pin 4  // output

 MOSI: pin 11  // output
 MISO: pin 12  // input
 SCK: pin 13  // out

 Created 18 Sep. 2012
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
#include <TimerOne.h>

#define SSID    ""        // SSID of your AP
#define Key     ""  // Key or Passphrase
// Wi-Fi security option (NO_SECURITY, WEP_SECURITY, WPA_SECURITY, WPA2PSK_SECURITY)
//#define Security        WPA_SECURITY

WizFi2x0Class myWizFi;
WizFiClient myClient;
TimeoutClass ConnectInterval;

boolean Wifi_setup = false;

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
  
  // initalize WizFi2x0 module:
  myWizFi.begin();
 
  ConnectInterval.init();

  // Socket Creation with Server IP address and Server Port num 
  myClient =  WizFiClient("google.com", 80);
  
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
    retval = myWizFi.associate(SSID, Key, Security, true);
    
    if(retval == 1){
      Serial.println("AP association Success");
      Wifi_setup = true;
      break;
    }else{
      Serial.println("AP association Failed");
    }
  }
  
}

void loop()
{
  uint8_t retval;
  byte rcvdBuf[129];
 
  memset(rcvdBuf, 0, 129);
  
  if(Wifi_setup)
  {  
     myWizFi.RcvPacket();
     
     if(myClient.available()){       
       if(myClient.read(rcvdBuf))
       {
         Serial.print((char *)rcvdBuf);
       }
     }else{
         retval = myClient.connect();
         if(retval == 1)
         {
           Serial.println("Connected! ");
           myClient.write((byte *)"GET /search?q=WizFi210 HTTP/1.0\r\n\r\n");
         }else
           Serial.println("Connection Failed");
       }
  }
}

