#include <WizFiUDP.h>
#include "StringDef.h"

#include <avr/pgmspace.h>

#ifdef CODEMEM

prog_char DNS_Success_1[] PROGMEM = "DNS Query Success";   // "String 0" etc are strings to store - change to suit.
prog_char DNS_Fail_1[] PROGMEM = "DNS Query Failed";
prog_char Conn_F_Msg_1[] PROGMEM = "Connection Failed";
prog_char Disconn_F_Msg_1[] PROGMEM = "Disconnection Failed";
prog_char Conn_S_Msg_1[] PROGMEM = "CID[%c] Connected";
prog_char Disconn_S_Msg_1[] PROGMEM = "CID[%c] Disconnected";
prog_char PeerIP_Msg_1[] PROGMEM = "Peer IP: %d.%d.%d.%d";
prog_char PeerPort_Msg_1[] PROGMEM = "Peer Portnum: %u";
prog_char Sock_F_Msg_1[] PROGMEM = "Socket Failure";
prog_char Inval_Noti_Msg_1[] PROGMEM = "Invalid Notification";



PROGMEM const char *string_table_1[] = 
{
	DNS_Success_1,			// 0
	DNS_Fail_1,				// 1
	Conn_F_Msg_1,			// 2
	Disconn_F_Msg_1,		// 3
	Conn_S_Msg_1,			// 4
	Disconn_S_Msg_1,		// 5
	PeerIP_Msg_1,			// 6
	PeerPort_Msg_1,			// 7
	Sock_F_Msg_1,			// 8
	Inval_Noti_Msg_1,		// 9
};

#endif

WizFiUDP::WizFiUDP(void)
{
	WizFi = &myWizFi;
	bOpen = false;
	bDNSQuery = false;

	
	CID = 0xFF;
}

WizFiUDP::WizFiUDP(uint8_t *ip, uint16_t port, uint16_t src_port)
{
	memcpy(DstIP, ip, 4);
	DstPort = port;
	DstDomain = NULL;
	SrcPort = src_port;
	
       WizFi = &myWizFi;
	   
	bOpen = false;
	bDNSQuery = false;

	if(ip == NULL && port == 0)
		Mode = SERVER_MODE;
	else
		Mode = CLIENT_MODE;

	CID = 0xFF;
}

WizFiUDP::WizFiUDP(const char* domain, uint16_t port, uint16_t src_port)
{
	memset(DstIP, 0, 4);
	DstPort = port;
	DstDomain = domain;
	SrcPort = src_port;

       WizFi = &myWizFi;
	   
	bOpen = false;
	bDNSQuery = true;

	if(domain == NULL && port == 0)
		Mode = SERVER_MODE;
	else
		Mode = CLIENT_MODE;
	
	CID = 0xFF;

}


uint8_t WizFiUDP::open(void)
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
		
		while(1)
		{
			WizFi->RcvPacket();
			retval = WizFi->SendCommand(WizFi->Current_Command_Code);
			if(retval == 1)
			{
				WizFi->GetPeerIPAddr(tmpIP);
				WizFi->SetIP(DstIP, tmpIP);
#ifdef CODEMEM
			       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_1[0]))); // Necessary casts and dereferencing, just copy. 
				Serial.println((char *)tmpstr );
#endif			
				break;
			}
			else if(retval == 2 || retval == 3)
			{
#ifdef CODEMEM
				strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_1[1]))); // Necessary casts and dereferencing, just copy. 
   				Serial.println( (char *)tmpstr );
#endif			
				return 0; //DNS Query Failed
			}
		}
	}

	if(Mode == CLIENT_MODE)
		WizFi->Current_Command_Code = OP_NCUDP;
	else
		WizFi->Current_Command_Code = OP_NSUDP;
		
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
	WizFi->SetSrcPortnum(SrcPort);
	WizFi->CmdResult = CMD_AVAILABLE;
	
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
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_1[4]))); // Necessary casts and dereferencing, just copy. 
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

uint8_t WizFiUDP::close()
{
	uint8_t retval;
	byte tmpstr[22];
//	byte tmpIP[16];
	byte final_str[24];

	WizFi->Current_Command_Code = OP_NCLOSE;
	WizFi->CID = CID;
	WizFi->CmdResult = CMD_AVAILABLE;
	
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
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_1[5]))); // Necessary casts and dereferencing, just copy.
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
			       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_1[3]))); // Necessary casts and dereferencing, just copy. 	
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

boolean WizFiUDP::available()
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
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_1[5]))); // Necessary casts and dereferencing, just copy.
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
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_1[4]))); // Necessary casts and dereferencing, just copy. 
			sprintf((char *)final_str, (char *)tmpstr, (char)CID);
			Serial.println((char *)final_str );
			
			memset(final_str, 0, 32);
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_1[6]))); // Necessary casts and dereferencing, just copy. 
			sprintf((char *)final_str, (char *)tmpstr, DstIP[0], DstIP[1], DstIP[2], DstIP[3]);
			Serial.println((char *)final_str);
			
			memset(final_str, 0, 32);
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_1[7]))); // Necessary casts and dereferencing, just copy. 
			sprintf((char *)final_str, (char *)tmpstr, DstPort);
			Serial.println((char *)final_str);
#endif			
		}
	}else if(myWizFi.NOTI_TYPE == SOCKFAILURE_NOTI)
	{
#ifdef CODEMEM
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_1[8]))); // Necessary casts and dereferencing, just copy. 	
		Serial.println((char *)tmpstr);
#endif			
		myWizFi.NOTI_TYPE = NO_NOTI;
		myWizFi.ChangeESCState(WizFi2x0_ESC_IDLE);
	}else if(myWizFi.NOTI_TYPE == INVALID_NOTI)
	{
#ifdef CODEMEM
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table_1[9]))); // Necessary casts and dereferencing, just copy. 	
		Serial.println((char *)tmpstr);
#endif			
		myWizFi.NOTI_TYPE = NO_NOTI;
		myWizFi.ChangeESCState(WizFi2x0_ESC_IDLE);
	}

	return bOpen;
	
}

uint8_t WizFiUDP::GetCID(void)
{
	return CID;
}

void WizFiUDP::GetCIDstr(byte * strCID)
{
	sprintf((char *)strCID, "%d", CID);

	return ;
}

void WizFiUDP::GetCurrentDestInfo(byte *ipaddr, uint16_t *portnum)
{
	myWizFi.GetPeerIPAddr(ipaddr);
	*portnum = myWizFi.GetPeerPortnum();
}

void WizFiUDP::SetCurrentDestInfo(byte *ipaddr, uint16_t portnum)
{
	myWizFi.SetPeerIPAddr(ipaddr);
	myWizFi.SetPeerPortnum(portnum);
}

void WizFiUDP::write(byte value)
{
	byte tmpstr[16];

	
//	sprintf(tmpstr, "%cS%c%c%cE", ASCII_ESC, CID, value, ASCII_ESC);
	WizFi->write(ASCII_ESC);
	WizFi->write('U');
	WizFi->write(CID);
	memset(tmpstr, 0, 16);
	WizFi->GetPeerIPAddr(tmpstr);
	WizFi->write(tmpstr);
	WizFi->write(':');
	memset(tmpstr, 0, 16);
	sprintf((char *)tmpstr, "%u", WizFi->GetPeerPortnum());
	WizFi->write(tmpstr);
	WizFi->write(':');
	WizFi->write(value);
	WizFi->write(ASCII_ESC);
	WizFi->write('E');

}

void WizFiUDP::write(byte *buf)
{
	byte tmpstr[16];
	
	WizFi->write(ASCII_ESC);
	WizFi->write('U');
	WizFi->write(CID);
	memset(tmpstr, 0, 16);
	WizFi->GetPeerIPAddr(tmpstr);
	WizFi->write(tmpstr);
	WizFi->write(':');
	memset(tmpstr, 0, 16);
	sprintf((char *)tmpstr, "%u", WizFi->GetPeerPortnum());
	WizFi->write(tmpstr);
	WizFi->write(':');
	WizFi->write(buf);
	WizFi->write(ASCII_ESC);
	WizFi->write('E');

}

void WizFiUDP::write(byte *buf, size_t size)
{
	byte tmpstr[16];
	
	WizFi->write(ASCII_ESC);
	WizFi->write('U');
	WizFi->write(CID);
	memset(tmpstr, 0, 16);
	WizFi->GetPeerIPAddr(tmpstr);
	WizFi->write(tmpstr);
	WizFi->write(':');
	memset(tmpstr, 0, 16);
	sprintf((char *)tmpstr, "%u", WizFi->GetPeerPortnum());
	WizFi->write(tmpstr);
	WizFi->write(':');
	WizFi->write(buf, size);
	WizFi->write(ASCII_ESC);
	WizFi->write('E');

}

uint8_t WizFiUDP::read(void)
{
	return WizFi->read();
}

uint8_t WizFiUDP::read(byte *buf)
{
	uint8_t index, i=0;
	byte DBG_Buf[10];

	index = CID - '0';


//	WizFi->RcvPacket();

	if(WizFi->IsDataRcvd[index])
	{		
		while(WizFi->Current_ESC_Data_Length > 0)
		{
			buf[i++] = WizFi->RcvdBuf[WizFi->readPtr++];
			if(WizFi->readPtr == 128)
				WizFi->readPtr = 0;
			WizFi->Current_ESC_Data_Length--;
		}
		WizFi->IsDataRcvd[index] = false;

		return i;
	}
	return 0;
}

uint8_t WizFiUDP::read(byte *buf, size_t size)
{
	uint8_t index, retval, i=0;

	index = CID - '0';

	retval = 0;
	
//	WizFi->RcvPacket();
	
	if(WizFi->IsDataRcvd[index])
	{
		while(WizFi->readPtr != WizFi->writePtr)
		{
			buf[i++] = WizFi->RcvdBuf[WizFi->readPtr++];
		}
		WizFi->IsDataRcvd[index] = false;

		return i;
	}
	return retval;
}


