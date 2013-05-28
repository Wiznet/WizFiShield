/******************************************************************
 WizFiShield UDP Client Test Example
 
 This sketch show how to send/receive the data using UDP protocol 
 when the peer's IP address and port number is known.
 
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
#include <WizFiUDP.h>
#include <TimerOne.h>

#define SSID    ""        // SSID of your AP
#define Key     ""  // Key or Passphrase
// Wi-Fi security option (NO_SECURITY, WEP_SECURITY, WPA_SECURITY, WPA2PSK_SECURITY)
//#define Security        WPA_SECURITY

unsigned char  SIP[4] 	        = {192, 168, 1, 13}; // Specifiy the server IP address
unsigned int ServerPort = 50000;  // Specifiy the server port number

WizFi2x0Class myWizFi;
WizFiUDP myUDP;
TimeoutClass ConnectInterval;

boolean Wifi_setup = false;

///////////////////////////////
// 1msec Timer
void Timer1_ISR()
{
  myWizFi.ReplyCheckTimer.CheckIsTimeout();
  ConnectInterval.CheckIsTimeout();
}
//
//////////////////////////////

void setup() {
  byte retval;
//  byte retry_count = 0;
//  byte tmpstr[64];
  
  Serial.begin(9600);
  Serial.println("\r\nSerial Init");
  
  // initalize WizfiShield:
  myWizFi.begin();
  myUDP =  WizFiUDP(SIP, ServerPort, 50000);
   
  ConnectInterval.init();
  
  // Timer1 Initialize
  Timer1.initialize(10000); // 10msec
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
 
  if(myUDP.open())
    Serial.println("UDP socket created successfully");
  else
    Serial.println("UDP socket creation failed");

    ConnectInterval.TimerStart(100);  
}

void loop()
{
  byte rcvdBuf[129];
 
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


