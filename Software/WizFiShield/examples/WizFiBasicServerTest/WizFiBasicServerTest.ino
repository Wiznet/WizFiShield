/******************************************************************
 WizFiShield Basic Server Test Example
 
 This sketch makes the server socket and accpet TCP connection from the client 
 
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

// WizFi210 communicates using SPI, so include the SPI library:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPI.h>
#include <WizFi2x0.h>
#include <WizFiClient.h>
#include <WizFiServer.h>
#include <TimerOne.h>

#define SSID 		""        // SSID of your AP
#define Key 	  ""  // Key or Passphrase
// Wi-Fi security option (NO_SECURITY, WEP_SECURITY, WPA_SECURITY, WPA2PSK_SECURITY)
//#define Security        WPA_SECURITY

unsigned int SrcPort = 50000;    // listening port number

WizFi2x0Class myWizFi;
WizFiClient myClient[4];//(SIP, ServerPort);
WizFiServer myServer(SrcPort);
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
  byte retval, i;
  
  Serial.begin(9600);
  Serial.println("\r\nSerial Init");
  
  for(i=0; i<4; i++)
    myClient[i] =  WizFiClient();
  myWizFi.begin();
 
  ConnectInterval.init();
  
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
  
  if(myServer.begin())
    Serial.println("Server Listen OK");
  else
    Serial.println("Server Listen Failed");
}

void loop()
{
  uint8_t i;
  byte rcvdBuf[129];
 
  memset(rcvdBuf, 0, 129);
  
  if(Wifi_setup)
  {  
    myWizFi.RcvPacket();
    for(i=0; i<4; i++)
    {
      if(myClient[i].available()){       
         if(myClient[i].read(rcvdBuf))
         {
           Serial.print("CID[");
           Serial.print((char)myClient[i].GetCID());
           Serial.print("]");
           Serial.println((char *)rcvdBuf);
           myClient[i].write(rcvdBuf);
         }
      }
    }
  }
}

