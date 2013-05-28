/******************************************************************
 WizFiShield Limitted AP Test Example
 
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

byte IsTimer1Expired = 0;
uint16_t CurrentTime = 0;

#define SSID 		"LimitedAP"     // SSID of WizFiShield
#define Key 	  "1234567890"    // Key or Passphrase
// Wi-Fi security option (NO_SECURITY, WEP_SECURITY)
#define Security        WEP_SECURITY

unsigned char  SIP[4] 	= {192, 168, 1, 100}; // specify the peer's IP address to communicate
unsigned int ServerPort = 5000;

WizFi2x0Class myWizFi;
WizFiClient myClient;
TimeoutClass ConnectInterval;

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
  byte key, retval, i;
  byte retry_count = 0;
  byte tmpstr[64];
  
  Serial.begin(9600);
  Serial.println("\r\nSerial Init");
  
  // initalize WizFiShield:
  myWizFi.begin();
  myClient =  WizFiClient(SIP, ServerPort);
   
  // Timer1 Initialize
  Timer1.initialize(1000); // 1msec
  Timer1.attachInterrupt(Timer1_ISR);

  myWizFi.SetSrcIPAddr((byte *)"192.168.1.99");  // set the IP address of Limited AP
  myWizFi.SetSrcSubnet((byte *)"255.255.255.0");
  myWizFi.SetSrcGateway((byte *)"192.168.1.99");
  
  myWizFi.SetOperatingMode(LIMITEDAP_MODE);
 
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
    retval = myWizFi.associate(SSID, Key, Security, false);
    
    if(retval == 1){
      Serial.println("AP association Success");
      Wifi_setup = true;
      break;
    }else{
      Serial.println("AP association Failed");
    }
  }
 
  retval = myWizFi.SetTxPower(5);
  if(retval == 1){ Serial.println("Tx Power set successfully"); }
  else{ Serial.println("Tx Power parameter is invalid"); }
  
  delay(1000);
  Serial.flush();
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
         Serial.print("CID[");
         Serial.print((char)myClient.GetCID());
         Serial.print("]");
         Serial.println((char *)rcvdBuf);
         Serial.flush();
         myClient.write(rcvdBuf);
       }

       if(Disconnect_flag)
       {
         retval = myClient.disconnect();
         Disconnect_flag = false;
       }
     }else{
       if(Connect_flag)
       {
         retval = myClient.connect();
         Connect_flag = false;
       }
     }
     CheckConsoleInput();
  }
 
}

void CheckConsoleInput(void)
{
  uint8_t ch;
  boolean IsCommandRcvd = false;
  
  if(Serial.available() > 0){
    ch = Serial.read();
    IsCommandRcvd = true;
  }

  if(IsCommandRcvd){    
    switch(ch)
    {
    case 'd':
    case 'D':
      Disconnect_flag = true;
      break;
    case 'c':
    case 'C':
      Connect_flag = true;
      break;
    case 's':
    case 'S':
       if(myClient.IsConnected())
         Serial.println("Connected");
       else
         Serial.println("Disconnected");
       break;
    }
  } 
}
