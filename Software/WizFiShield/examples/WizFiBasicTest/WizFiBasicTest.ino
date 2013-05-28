/******************************************************************
 WizFiShield Basic Test Example
 
 This sketch connect to the server after receiving the serial command. 
 
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

// WizFi210 communicates using SPI, so include the SPI library:
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

unsigned char  SIP[4] 	= {192, 168, 1, 7}; // Server IP address to connnect
unsigned int ServerPort = 5000;  // Server port number

WizFi2x0Class myWizFi;
WizFiClient myClient;

boolean Wifi_setup = false;

///////////////////////////////
// 1msec Timer
void Timer1_ISR()
{
  uint8_t i;
  
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
  myClient =  WizFiClient(SIP, ServerPort);
   
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
 
  delay(1000);
  
}

void loop()
{
  uint8_t retval;
  byte rcvdBuf[129];
 
  memset(rcvdBuf, 0, 129);
  
  if(Wifi_setup)
  {  
    myWizFi.RcvPacket();

     if(myClient.available()) {
      if(myClient.read(rcvdBuf))
       {
         Serial.print("CID[");
         Serial.print((char)myClient.GetCID());
         Serial.print("]");
         Serial.println((char *)rcvdBuf);
         myClient.write(rcvdBuf);
       }
    }  
     CheckConsoleInput();

  } 
}


void CheckConsoleInput(void)
{
  
  uint8_t ch;
  
  if(Serial.available() > 0)
    ch = Serial.read();
    
  switch(ch)
  {
  case 'd':
  case 'D':
    Serial.println("Disconnect Request received");
    if(myClient.IsConnected()) {
      if (myClient.disconnect() == 1) Serial.println("Disconnected! ");
      else Serial.println("Disconnection Failed");
    } 
    break;
  case 'c':
  case 'C':
    Serial.println("Connect Request received");
    if(!(myClient.IsConnected())) {
      if (myClient.connect() == 1) Serial.println("Connected! ");
      else Serial.println("Connection Failed");
    }
    break;
  }
  
}

