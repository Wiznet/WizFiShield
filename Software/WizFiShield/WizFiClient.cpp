#include <WizFiClient.h>
#include "StringDef.h"

/////////////////////////////////////////////////////////
// WIZnet WizFi Shield library is Copyright(c) 2012 by WIZnet, Inc.
// and is licensed under the LGPL.
// 
// Author : James YS Kim

#include <avr/pgmspace.h>

#ifdef CODEMEM

prog_char DNS_Success_2[] PROGMEM = "DNS Query Success";   // "String 0" etc are strings to store - change to suit.
prog_char DNS_Fail_2[] PROGMEM = "DNS Query Failed";
prog_char Conn_F_Msg_2[] PROGMEM = "Connection Failed";
prog_char Disconn_F_Msg_2[] PROGMEM = "Disconnection Failed";
prog_char Conn_S_Msg_2[] PROGMEM = "CID[%c] Connected";
prog_char Disconn_S_Msg_2[] PROGMEM = "CID[%c] Disconnected";
prog_char PeerIP_Msg_2[] PROGMEM = "Peer IP: %d.%d.%d.%d";
prog_char PeerPort_Msg_2[] PROGMEM = "Peer Portnum: %u";
prog_char Sock_F_Msg_2[] PROGMEM = "Socket Failure";
prog_char Inval_Noti_Msg_2[] PROGMEM = "Invalid Notification";



PROGMEM const char *string_table_2[] = 
{
	DNS_Success_2,			// 0
	DNS_Fail_2,				// 1
	Conn_F_Msg_2,			// 2
	Disconn_F_Msg_2,		// 3
	Conn_S_Msg_2,			// 4
	Disconn_S_Msg_2,		// 5
	PeerIP_Msg_2,			// 6
	PeerPort_Msg_2,			// 7
	Sock_F_Msg_2,			// 8
	Inval_Noti_Msg_2,		// 9
};

#endif

WizFiClient::WizFiClient(void)
{
	WizFi = &myWizFi;
	bOpen = false;
	bDNSQuery = false;

	
	CID = 0xFF;
}

WizFiClient::WizFiClient(uint8_t *ip, uint16_t port)//, WizFi2x0Class WizFi)
{
	memcpy(DstIP, ip, 4);
	DstPort = port;
	DstDomain = NULL;
	
       WizFi = &myWizFi;
	   
	bOpen = false;
	bDNSQuery = false;

	CID = 0xFF;
}

WizFiClient::WizFiClient(const char* domain, uint16_t port)//, WizFi2x0Class WizFi)
{
	memset(DstIP, 0, 4);
	DstPort = port;
	DstDomain = domain;

       WizFi = &myWizFi;
	   
	bOpen = false;
	bDNSQuery = true;

	CID = 0xFF;
}

boolean WizFiClient::available()
{
	byte IPAddr[16];
	byte tmpstr[22];
	byte final_str[32];
	
	if(myWizFi.NOTI_TYPE == DISCONNECT_NOTI)
	{
		if(myWizFi.NOTI_CH == CID)
		{
			myWizFi.NOTI_TYPE = NO_NOTI;
	 		myWizFi.ChangeESCState(WizFi2x0_ESC_IDLE);

#ifdef CODEMEM
			memset(final_str, 0, 32);
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_2[5]))); // Necessary casts and dereferencing, just copy.
		       sprintf((char *)final_str, (char *)tmpstr, (char)CID);
			Serial.println((char *)final_str );
#endif			
			
	 		bOpen = false;
			CID = 0xFF;
//			memset(DstIP, 0, 4);
//			DstPort = 0;
		}
	}else if(myWizFi.NOTI_TYPE == CONNECT_NOTI)
	{
		if(CID == 0xFF)
		{
			CID = myWizFi.NOTI_CH;
			myWizFi.NOTI_TYPE = NO_NOTI;
			myWizFi.ChangeESCState(WizFi2x0_ESC_IDLE);
			
			bOpen = true;
			myWizFi.GetPeerIPAddr(IPAddr);
			myWizFi.SetIP(DstIP, IPAddr);
			DstPort = myWizFi.GetPeerPortnum();

#ifdef CODEMEM
			memset(final_str, 0, 32);
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_2[4]))); // Necessary casts and dereferencing, just copy. 
			sprintf((char *)final_str, (char *)tmpstr, (char)CID);
			Serial.println((char *)final_str );
			
			memset(final_str, 0, 32);
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_2[6]))); // Necessary casts and dereferencing, just copy. 
			sprintf((char *)final_str, (char *)tmpstr, DstIP[0], DstIP[1], DstIP[2], DstIP[3]);
			Serial.println((char *)final_str);
			
			memset(final_str, 0, 32);
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_2[7]))); // Necessary casts and dereferencing, just copy. 
			sprintf((char *)final_str, (char *)tmpstr, DstPort);
			Serial.println((char *)final_str);
#endif			
		}
	}else if(myWizFi.NOTI_TYPE == SOCKFAILURE_NOTI)
	{
#ifdef CODEMEM
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_2[8]))); // Necessary casts and dereferencing, just copy. 	
		Serial.println((char *)tmpstr);
#endif			
		myWizFi.NOTI_TYPE = NO_NOTI;
		myWizFi.ChangeESCState(WizFi2x0_ESC_IDLE);
	}else if(myWizFi.NOTI_TYPE == INVALID_NOTI)
	{
#ifdef CODEMEM
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_2[9]))); // Necessary casts and dereferencing, just copy. 	
		Serial.println((char *)tmpstr);
#endif			
		myWizFi.NOTI_TYPE = NO_NOTI;
		myWizFi.ChangeESCState(WizFi2x0_ESC_IDLE);
	}

	return bOpen;
	
}

uint8_t WizFiClient::connect()
{
	uint8_t retval;
	byte tmpstr[22];
	byte tmpIP[16];
	byte final_str[40];

	if(bDNSQuery)
	{
		WizFi->SetDomainName((byte *)DstDomain);
		WizFi->Current_Command_Code = OP_DNSLOOK;
		WizFi->CmdResult = CMD_AVAILABLE;
		WizFi->Current_ReplyState = WizFi2x0_ReplyState_IDLE;
		while(1)
		{
			WizFi->RcvPacket();
			retval = WizFi->SendCommand(WizFi->Current_Command_Code);
			if(retval == 1)
			{
				WizFi->GetPeerIPAddr(tmpIP);
				WizFi->SetIP(DstIP, tmpIP);
#ifdef CODEMEM
			       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_2[0]))); // Necessary casts and dereferencing, just copy. 
				Serial.println((char *)tmpstr );
#endif			
				break;
			}
			else if(retval == 2 || retval == 3)
			{
#ifdef CODEMEM
				strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_2[1]))); // Necessary casts and dereferencing, just copy. 
   				Serial.println( (char *)tmpstr );
#endif			
				return 0; //DNS Query Failed
			}
		}
	}
	
	WizFi->Current_Command_Code = OP_NCTCP;
	WizFi->CmdResult = CMD_AVAILABLE;
	WizFi->Current_ReplyState = WizFi2x0_ReplyState_IDLE;
//	memset(tmpstr, 0, sizeof(tmpstr));
//	sprintf((char *)tmpstr, "%d.%d.%d.%d", DstIP[0], DstIP[1], DstIP[2], DstIP[3]);
//	Serial.println((char *)tmpstr);
	
//#ifndef CODEMEM
	memset(tmpstr, 0, 22);
//       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[6]))); // Necessary casts and dereferencing, just copy. 
	sprintf((char *)tmpstr, "%d.%d.%d.%d", DstIP[0], DstIP[1], DstIP[2], DstIP[3]);
//	Serial.println((char *)tmpstr);
//#endif			
			
	WizFi->SetPeerIPAddr(tmpstr);
	WizFi->SetPeerPortnum(DstPort);
	while(1)
	{
		WizFi->RcvPacket();
		retval = WizFi->SendCommand(WizFi->Current_Command_Code);

		if(retval == 1)
		{
			bOpen = true;
			WizFi->bCommandMode = false;
			CID = WizFi->CID;
#ifdef CODEMEM
			memset(final_str, 0, 40);
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_2[4]))); // Necessary casts and dereferencing, just copy. 
			sprintf((char *)final_str, (char *)tmpstr, (char)CID);
			Serial.println((char *)final_str );
#endif			
			return 1;
		}else
		{
			if(retval == 2)
			{
//				Serial.println("Error reply");
				return 0; // Connection faiied
			}
			else if(retval == 3)
			{
//				Serial.println("Timeout");
				return 0; // Connection faiied
			}
		}
	}

}

uint8_t WizFiClient::disconnect()
{
	uint8_t retval;
	byte tmpstr[22];
//	byte tmpIP[16];
	byte final_str[24];

	WizFi->Current_Command_Code = OP_NCLOSE;
	WizFi->CID = CID;
	WizFi->CmdResult = CMD_AVAILABLE;
	WizFi->Current_ReplyState = WizFi2x0_ReplyState_IDLE;
	WizFi->Current_CmdState = WizFi2x0_CmdState_IDLE;

//	Serial.println("disconnect() called");	

	while(1)
	{
		WizFi->RcvPacket();
		retval = WizFi->SendCommand(WizFi->Current_Command_Code);

		if(retval == 1)
		{
//			Serial.print("CID[");
//			Serial.print((char)CID);
//			Serial.println("] Disconnected!");
			
#ifdef CODEMEM
			memset(final_str, 0, 24);
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_2[5]))); // Necessary casts and dereferencing, just copy.
		       sprintf((char *)final_str, (char *)tmpstr, (char)CID);
			Serial.println((char *)final_str );
#endif			
	 		bOpen = false;
			CID = 0xFF;
//			memset(DstIP, 0, 4);
//			DstPort = 0;
			
			return 1;
		}else
		{			
			if(retval == 2 || retval == 3)
			{
//				Serial.println("Disconnection Failed");

#ifdef CODEMEM
			       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_2[3]))); // Necessary casts and dereferencing, just copy. 	
				Serial.println((char *)tmpstr);
#endif			
				
				bOpen = false;
				CID = 0xFF;
//				memset(DstIP, 0, 4);
//				DstPort = 0;
				
				return 0;
			}
		}
	}
}

uint8_t WizFiClient::async_connect()
{
	uint8_t retval;
/*
	if(!WizFi->IsCommandMode())
	{
		retval = WizFi->EnterCommandMode();
		if(retval != 1)
			return retval;
		else
		{
			WizFi->Current_Command_Code = OP_NCTCP;
			WizFi->SetPeerIPAddr((byte *)"192.168.123.119");
			WizFi->SetPeerPortnum(5000);
			return 4;
		}
	}
	else
	{
		if(WizFi->Current_Command_Code == OP_AT)
		{
			WizFi->Current_Command_Code = OP_NCTCP;
			WizFi->SetPeerIPAddr((byte *)"192.168.123.119");
			WizFi->SetPeerPortnum(5000);
			return 4;
		}
		retval = WizFi->SendCommand(WizFi->Current_Command_Code);

		if(retval == 1)
		{
			WizFi->Current_Command_Code = OP_AT;
			bOpen = true;
			WizFi->bCommandMode = false;
			CID = WizFi->CID;
			Serial.println((char *)WizFi->MsgBuf);
//			Serial.print("My Client CID: ");
//			Serial.println((char)CID);
			return 1;
		}else
		{
			return retval;
		}
	}
*/
	return 0;
}

uint8_t WizFiClient::async_disconnect()
{
	uint8_t retval;

/*
	if(!WizFi->IsCommandMode())
	{
		retval = WizFi->EnterCommandMode();
		if(retval != 1)
			return retval;
		else
		{
			WizFi->Current_Command_Code = OP_NCLOSEALL;
			return 4;
		}
	}
	else
	{
		retval = WizFi->SendCommand(WizFi->Current_Command_Code);

		if(retval == 1)
		{
			WizFi->Current_Command_Code = OP_AT;
			bOpen = false;
			return 1;
		}else
		{
			return retval;
		}
	}
*/
	return 0;
}


boolean WizFiClient::IsConnected()
{
	return bOpen;
}

void WizFiClient::SetIsConnected(boolean flag)
{
	bOpen = flag;
}

uint8_t WizFiClient::GetCID(void)
{
	return CID;
}

void WizFiClient::GetCIDstr(byte * strCID)
{
	sprintf((char *)strCID, "%d", CID);

	return ;
}

void WizFiClient::write(byte value)
{
//	sprintf(tmpstr, "%cS%c%c%cE", ASCII_ESC, CID, value, ASCII_ESC);
	WizFi->write(ASCII_ESC);
	WizFi->write('S');
	WizFi->write(CID);
	WizFi->write(value);
	WizFi->write(ASCII_ESC);
	WizFi->write('E');

}

void WizFiClient::write(byte *buf)
{
	WizFi->write(ASCII_ESC);
	WizFi->write('S');
	WizFi->write(CID);
	WizFi->write(buf);
	WizFi->write(ASCII_ESC);
	WizFi->write('E');

}

void WizFiClient::write(byte *buf, size_t size)
{
	WizFi->write(ASCII_ESC);
	WizFi->write('S');
	WizFi->write(CID);
	WizFi->write(buf, size);
	WizFi->write(ASCII_ESC);
	WizFi->write('E');

}

uint8_t WizFiClient::read(void)
{
	uint8_t index, i=0;
	byte DBG_Buf[10], buf;

	index = CID - '0';


//	WizFi->RcvPacket();

	if(WizFi->IsDataRcvd[index])
	{		
		if(WizFi->Current_ESC_Data_Length > 0)
		{
			buf = WizFi->RcvdBuf[WizFi->readPtr];
			WizFi->RcvdBuf[WizFi->readPtr++] = '\0';
			if(WizFi->readPtr == MAX_DATA_BUFSIZE)
				WizFi->readPtr = 0;
			WizFi->Current_ESC_Data_Length--;
		}

		return buf;
	}
	return 0;
}

uint8_t WizFiClient::read(byte *buf)
{
	uint8_t index, i=0;
	byte DBG_Buf[10];

	index = CID - '0';


//	WizFi->RcvPacket();

	if(WizFi->IsDataRcvd[index])
	{		
		while(WizFi->Current_ESC_Data_Length > 0)
		{
			buf[i++] = WizFi->RcvdBuf[WizFi->readPtr];
			WizFi->RcvdBuf[WizFi->readPtr++] = '\0';
			
			if(WizFi->readPtr == MAX_DATA_BUFSIZE)
				WizFi->readPtr = 0;
			WizFi->Current_ESC_Data_Length--;
		}
		WizFi->IsDataRcvd[index] = false;

		return i;
	}
	return 0;
}

uint8_t WizFiClient::read(byte *buf, size_t size)
{
	uint8_t index, retval, i=0;

	index = CID - '0';

	retval = 0;
	
//	WizFi->RcvPacket();
	
	if(WizFi->IsDataRcvd[index])
	{
		while(WizFi->readPtr != WizFi->writePtr)
		{
			buf[i++] = WizFi->RcvdBuf[WizFi->readPtr];
			WizFi->RcvdBuf[WizFi->readPtr++] = '\0';
		}
		WizFi->IsDataRcvd[index] = false;

		return i;
	}
/*	
	if(WizFi->IsDataRcvd[index])
	{
		memset(buf, 0, 33);
		memcpy(buf, WizFi->RcvdBuf[index], WizFi->RxPtr[index]);
		memset(WizFi->RcvdBuf[index], 0, 32);
		retval = WizFi->RxPtr[index];
		WizFi->RxPtr[index] = 0;
		WizFi->IsDataRcvd[index] = false;
	}
*/	
	return retval;
}

