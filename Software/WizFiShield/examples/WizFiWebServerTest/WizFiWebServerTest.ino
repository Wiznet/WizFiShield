/******************************************************************
 WizFiShield Web Server Test Example
 
 A simple web server that shows "Hello World" string 
 
 Circuit:
 WizFi2x0 connected to Arduino via SPI
 
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

#define SSID    ""        // SSID of your AP
#define Key     ""  // Key or Passphrase
// Wi-Fi security option (NO_SECURITY, WEP_SECURITY, WPA_SECURITY, WPA2PSK_SECURITY)
//#define Security        WPA_SECURITY

#define MAX_SOCK_NUM    4

unsigned int SrcPort = 80;

WizFi2x0Class myWizFi;
WizFiClient myClient[MAX_SOCK_NUM];//(SIP, ServerPort);
WizFiServer myServer(SrcPort);

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
  
  for(i=0; i<MAX_SOCK_NUM; i++)
    myClient[i] =  WizFiClient();

  myWizFi.begin();
 
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
      return;
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
      Serial.println("WizFi2xo AP Associated");
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
           Serial.print("CID[");
           Serial.print((char)myClient[i].GetCID());
           Serial.print("]");
           Serial.println((char *)rcvdBuf);
           
           if((rcvdBuf[retval - 1] == 0x0A) && (rcvdBuf[retval - 2] == 0x0D) && (rcvdBuf[retval - 3] == 0x0A) && (rcvdBuf[retval - 4] == 0x0D))
           {
             Serial.print("Receiving Completed");
             myClient[i].write((byte *)"HTTP/1.1 200 OK\r\n");
             myClient[i].write((byte *)"Content-Type: text/html\r\n");
             myClient[i].write((byte *)"\r\n");
             myClient[i].write((byte *)"Hello World !\r\n");
           
             delay(100);
             myClient[i].disconnect();
           }
         }
       }
    }
  }
}


