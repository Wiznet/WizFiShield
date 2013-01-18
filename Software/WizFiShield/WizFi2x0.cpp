#include <stdio.h>
#include <string.h>
#include "WizFi2x0.h"
#include <Config.h>
#include "StringDef.h"

/////////////////////////////////////////////////////////
// WIZnet WizFi Shield library is Copyright(c) 2012 by WIZnet, Inc.
// and is licensed under the LGPL.
// 
// Author : James YS Kim

#include <avr/pgmspace.h>

#ifdef CODEMEM
prog_char AT_Msg[] PROGMEM = "AT\r\n";   // "String 0" etc are strings to store - change to suit.
prog_char ATE_Msg[] PROGMEM = "ATE0\r\n";
prog_char WS_Msg[] PROGMEM = "AT+WS\r\n";
prog_char WA_Msg[] PROGMEM = "AT+WA=%s\r\n";
prog_char WAUTH_Msg[] PROGMEM = "AT+WAUTH=1\r\n";
prog_char NMAC_Msg[] PROGMEM = "AT+NMAC=%s\r\n";
prog_char XDUM_Msg[] PROGMEM = "AT+XDUM=0\r\n";
prog_char WD_Msg[] PROGMEM = "AT+WD\r\n";
prog_char WM_Msg[] PROGMEM = "AT+WM=%d\r\n";
prog_char DHCPSRVR_Msg[] PROGMEM = "AT+DHCPSRVR=1\r\n";
prog_char WP_Msg[] PROGMEM = "AT+WP=%d\r\n";
prog_char DNSSET_Msg[] PROGMEM = "AT+DNSSET=%s\r\n";
prog_char DNSSET2_Msg[] PROGMEM = "AT+DNSSET=%s,%s\r\n";
prog_char DNSLOOKUP_Msg[] PROGMEM = "AT+DNSLOOKUP=%s,3,5\r\n";
prog_char WAUTO_Msg[] PROGMEM = "AT+WAUTO=0,%s\r\n";
prog_char NAUTO_Msg[] PROGMEM = "AT+NAUTO=%d,%d,%s,%u\r\n";
prog_char NDHCP_Msg[] PROGMEM = "AT+NDHCP=%d\r\n";
prog_char WWEP_Msg[] PROGMEM = "AT+WWEP1=%s\r\n";
prog_char WWPA_Msg[] PROGMEM = "AT+WWPA=%s\r\n";
prog_char WPAPSK_Msg[] PROGMEM = "AT+WPAPSK=%s,%s\r\n";
prog_char BDATA_Msg[] PROGMEM = "AT+BDATA=1\r\n";
prog_char WATA_Msg[] PROGMEM = "ATA\r";
prog_char NSET_Msg[] PROGMEM = "AT+NSET=%s,%s,%s\r\n";
prog_char ATCID_Msg[] PROGMEM = "AT+CID=?\r\n";
prog_char ATO_Msg[] PROGMEM = "AT\r";
prog_char NCTCP_Msg[] PROGMEM = "AT+NCTCP=%s,%u\r\n";
prog_char NSTCP_Msg[] PROGMEM = "AT+NSTCP=%u\r\n";
prog_char NCLOSE_Msg[] PROGMEM = "AT+NCLOSE=%c\r\n";
prog_char NCLOSEALL_Msg[] PROGMEM = "AT+NCLOSEALL\r\n";
prog_char CMDMODE_Msg[] PROGMEM = "+++";
prog_char WS2_Msg[] PROGMEM = "AT+WS=,,%d\r\n";

prog_char DNS_Success[] PROGMEM = "DNS Query Success";   // "String 0" etc are strings to store - change to suit.
prog_char DNS_Fail[] PROGMEM = "DNS Query Failed";
prog_char Conn_F_Msg[] PROGMEM = "Connection Failed";
prog_char Disconn_F_Msg[] PROGMEM = "Disconnection Failed";
prog_char Conn_S_Msg[] PROGMEM = "CID[%c] Connected";
prog_char Disconn_S_Msg[] PROGMEM = "CID[%c] Disconnected";
prog_char PeerIP_Msg[] PROGMEM = "Peer IP: %d.%d.%d.%d";
prog_char PeerPort_Msg[] PROGMEM = "Peer Portnum: %u";
prog_char Sock_F_Msg[] PROGMEM = "Socket Failure";
prog_char Inval_Noti_Msg[] PROGMEM = "Invalid Notification";

prog_char NCUDP_Msg[] PROGMEM = "AT+NCUDP=%s,%u,%u\r\n";
prog_char NSUDP_Msg[] PROGMEM = "AT+NSUDP=%u\r\n";


PROGMEM const char *string_table[] = 
{
	AT_Msg,				// 0
	ATE_Msg,				// 1
	WS_Msg,				// 2
	WA_Msg,				// 3
	WAUTH_Msg,			// 4
	NMAC_Msg,			// 5
	XDUM_Msg,			// 6
	WD_Msg,				// 7
	WM_Msg,				// 8
	DHCPSRVR_Msg,		// 9
	WP_Msg,				// 10
	DNSSET_Msg,			// 11
	DNSSET2_Msg,		// 12
	DNSLOOKUP_Msg,		// 13
	WAUTO_Msg,			// 14
	NAUTO_Msg,			// 15
	NDHCP_Msg,			// 16
	WWEP_Msg,			// 17
	WWPA_Msg,			// 18
	WPAPSK_Msg,			// 19
	BDATA_Msg,			// 20
	WATA_Msg,			// 21
	NSET_Msg,			// 22
	ATCID_Msg,			// 23
	ATO_Msg,			// 24
	NCTCP_Msg,			// 25
	NSTCP_Msg,			// 26
	NCLOSE_Msg,			// 27
	NCLOSEALL_Msg,		// 28
	CMDMODE_Msg,		// 29
	WS2_Msg,			// 30
	DNS_Success,			// 31
	DNS_Fail,				// 32
	Conn_F_Msg,			// 33
	Disconn_F_Msg,		// 34
	Conn_S_Msg,			// 35
	Disconn_S_Msg,		// 36
	PeerIP_Msg,			// 37
	PeerPort_Msg,			// 38
	Sock_F_Msg,			// 39
	Inval_Noti_Msg,		// 40
	NCUDP_Msg,			// 41
	NSUDP_Msg			// 42
};

#endif

WizFi2x0Class::WizFi2x0Class()
{
}

void WizFi2x0Class::begin(void)
{
	uint8_t i;
	
	// Set IO Pins Direction
	pinMode(WizFi2x0_DataReady, INPUT);
	pinMode(WizFi2x0_CS, OUTPUT);
	pinMode(WizFi2x0_RST, OUTPUT);

	// SPI Init
	SPI.setClockDivider(SPI_CLOCK_DIV16); // Max Clock Frequency
	SPI.begin();

	//Reset Wifi2x0
	digitalWrite(WizFi2x0_CS, HIGH);
	digitalWrite(WizFi2x0_RST, HIGH);

	digitalWrite(WizFi2x0_RST, LOW);
	delay(500);

	digitalWrite(WizFi2x0_RST, HIGH);
	delay(4000);

	ReplyCheckTimer.init();

	retryCount = 0;

	CID = 0xFF;

	Current_CmdState = WizFi2x0_CmdState_IDLE;
	Current_ESC_State = WizFi2x0_ESC_IDLE;
	Current_Command_Code = OP_AT;
	bCommandMode = true;
	bAssociated = false;

	OperatingMode = INFRA_MODE;

	readPtr = writePtr = 0;
	
	for(i=0; i<MAX_SOCK_NUM; i++)
	{
		IsDataRcvd[i] = false;
		SockAvailable[i] = false;
	}

	NOTI_TYPE = NO_NOTI;

	bFirstEnd = false;
	

//	Client = &myClient;

}

uint8_t WizFi2x0Class::associate(void)
{
	uint8_t retval;
	
	Current_Command_Code = OP_WA;
	Current_CmdState = WizFi2x0_CmdState_IDLE;
	
	while(1)
	{
		retval = SendCommand(Current_Command_Code);

		if(retval == 1)
		{
			bAssociated = true;
			return 1;
		}else
		{
			if(retval == 2 || retval == 3)
				return 0;
		}
	}
}

uint8_t WizFi2x0Class::associate(const char *ssid, const char *passphrase, SECUTYPE EncryptType, boolean isDHCP)
{
	uint8_t retval;

	Current_Command_Code = OP_AT;
	Current_CmdState = WizFi2x0_CmdState_IDLE;
	
	while(1)
	{
		retval = SendCommand(Current_Command_Code);

		if(retval == 1)
		{
			switch(Current_Command_Code)
			{
			case OP_AT:
				Current_Command_Code = OP_ATE;
				SetSSID((byte *)ssid);
				SetKEY((byte *)passphrase);
				SetDHCPFlag((uint8_t)isDHCP);
				SetSecurityType(EncryptType);
				break;
			case OP_ATE:
				Current_Command_Code = OP_XDUM;
				break;
			case OP_XDUM:
				Current_Command_Code = OP_BDATA;
				break;
			case OP_BDATA:
				Current_Command_Code = OP_WD;
				break;
			case OP_WD:
				Current_Command_Code = OP_WM;
				break;
			case OP_WM:
				if(EncryptType == NO_SECURITY)
				{
					if(DHCP_Flag == 1)
						Current_Command_Code = OP_NDHCP;
					else
						Current_Command_Code = OP_NSET;
				}
				else
				{
					if(OperatingMode == LIMITEDAP_MODE){
						Current_Command_Code = OP_WAUTH;
					}else{
						if(EncryptType == WEP_SECURITY)
							Current_Command_Code = OP_WWEP;
						else if(EncryptType == WPA_SECURITY)
							Current_Command_Code = OP_WWPA;
						else if(EncryptType == WPA2PSK_SECURITY)
							Current_Command_Code = OP_WPAPSK;
					}
				}
				break;
			case OP_WAUTH:
				if(EncryptType == WEP_SECURITY)
					Current_Command_Code = OP_WWEP;
				else if(EncryptType == WPA_SECURITY)
					Current_Command_Code = OP_WWPA;
				else if(EncryptType == WPA2PSK_SECURITY)
					Current_Command_Code = OP_WPAPSK;
				
				break;				
			case OP_WWEP:
			case OP_WWPA:
			case OP_WPAPSK:
				if(OperatingMode == INFRA_MODE && DHCP_Flag == 1)
					Current_Command_Code = OP_NDHCP;
				else
					Current_Command_Code = OP_NSET;
				break;
			case OP_NDHCP:
				Current_Command_Code = OP_WA;
				break;
			case OP_NSET:
				Current_Command_Code = OP_WA;
				break;
			case OP_WA:
				if(OperatingMode == LIMITEDAP_MODE)
				{
					Current_Command_Code = OP_DHCPSRVR;
					break;
				}else
				{
					bAssociated = true;
					return 1;
				}
			case OP_DHCPSRVR:
				bAssociated = true;
				return 1;
			}
		}else if(retval == 2 || retval == 3)
			return 0; // association failed
	}
}

uint8_t WizFi2x0Class::disassociate(void)
{
	uint8_t retval;

	Current_Command_Code = OP_WD;
	Current_CmdState = WizFi2x0_CmdState_IDLE;
	
	while(1)
	{
		retval = SendCommand(Current_Command_Code);

		if(retval == 1)
		{
			bAssociated = false;
			return 1;
		}else if(retval == 2 || retval == 3)
		{
			return 0; //Disassociation Failed
		}
	}
	
}

boolean WizFi2x0Class::IsAssociated(void)
{
	return bAssociated;
}

uint8_t WizFi2x0Class::SetMACAddr(byte * buf)
{
	uint8_t retval;

	memset(SRCMACAddr, 0, 18);
	memcpy(SRCMACAddr, buf, 17);
	
	Current_Command_Code = OP_NMAC;
	Current_CmdState = WizFi2x0_CmdState_IDLE;

	
	while(1)
	{
		retval = SendCommand(Current_Command_Code);

		if(retval == 1)
		{
			return 1;
		}else if(retval == 2 || retval == 3)
		{
			return 2;
		}
	}
}

uint8_t WizFi2x0Class::GetMACAddr(byte * buf)
{
	return 0;
}


uint8_t WizFi2x0Class::SetTxPower(uint8_t power_level)
{
	uint8_t retval;
	
	Current_Command_Code = OP_WP;
	Current_CmdState = WizFi2x0_CmdState_IDLE;

	if(power_level >= 0 && power_level <= 15)
		PowerLevel = power_level;
	else{
#ifdef DEBUG_ENABLE		
		Serial.println("Invalid Tx Power");
#endif
		return 2;
	}
	
	while(1)
	{
		retval = SendCommand(Current_Command_Code);

		if(retval == 1)
		{
			return 1;
		}else if(retval == 2 || retval == 3)
		{
			return 2;
		}
	}
}

uint8_t WizFi2x0Class::GetTxPower(void)
{
	return PowerLevel;
}


void WizFi2x0Class::SetOperatingMode(OPMODE mode)
{
	if(OperatingMode == INFRA_MODE || OperatingMode == ADHOC_MODE || OperatingMode == LIMITEDAP_MODE)
		OperatingMode = mode;
}

uint8_t WizFi2x0Class::wifi_scan(void)
{
	uint8_t retval;

	Current_Command_Code = OP_WS;
	Current_CmdState = WizFi2x0_CmdState_IDLE;
	
	while(1)
	{
		retval = SendCommand(Current_Command_Code);

		if(retval == 1)
		{
			return 1;
		}else if(retval == 2 || retval == 3)
		{
			return 0; //Disassociation Failed
		}
	}

}

uint8_t WizFi2x0Class::wifiScan()
{
	m_channel = 0;
	m_filterRSSI = 0;

	return wifi_scan();
}

uint8_t WizFi2x0Class::wifiScan(uint8_t channel)
{
	if(channel >= 0 && channel <= 14)
		m_channel = channel;
	else{
#ifdef DEBUG_ENABLE		
		Serial.println("Invalid Channel");
#endif
		return 2;
	}
	m_filterRSSI = 0;
	
	return wifi_scan();
}

uint8_t WizFi2x0Class::wifiScan(uint8_t channel, uint8_t RSSI)
{
	if(channel >= 0 && channel <= 14)
		m_channel = channel;
	else{
#ifdef DEBUG_ENABLE		
		Serial.println("Invalid Channel");
#endif
		return 2;
	}
	if(RSSI >= 0 && RSSI <= 100)
		m_filterRSSI = RSSI;
	else{
#ifdef DEBUG_ENABLE		
		Serial.println("Invalid RSSI");
#endif
		return 2;
	}

	return wifi_scan();
}

bool WizFi2x0Class::CheckRSSIPower(uint8_t value, char * buf)
{
	uint8_t tmpvalue, i;

	tmpvalue = 0;
	
	for(i=1; buf[i] != '\0'; i++)
	{
		tmpvalue *= 10;
		tmpvalue += buf[i] - '0';
	}

	if(value >= tmpvalue)
		return true;
	else
		return false;
}

uint8_t WizFi2x0Class::write(byte ch)
{
	if(ByteStuff(&ch))
	{
		digitalWrite(WizFi2x0_CS, LOW);
		SPI.transfer(spichar.SPI_ESC_CHAR);
		digitalWrite(WizFi2x0_CS, HIGH);
	}

	digitalWrite(WizFi2x0_CS, LOW);
	SPI.transfer(ch);
	digitalWrite(WizFi2x0_CS, HIGH);

//	Serial.println((char *)"WizFi2x0Class::write() called");
	return 1;
}

uint8_t WizFi2x0Class::write(byte *buf)
{
	int i = 0;

//	Serial.println((char *)buf);
	while(*(buf + i) != '\0')
	{
		write(*(buf + i));
		//delay(10);
		i++;
	}

	BufClear();
	
	return (uint8_t)i;
}

uint8_t WizFi2x0Class::write(byte *buf, size_t size)
{
	int i = 0;
	
	while(i < size)
	{
		write(*(buf + i));
		//delay(10);
		i++;
	}

	BufClear();
	
	return (uint8_t)i;
}


uint8_t WizFi2x0Class::read(void)
{
	uint8_t result;

	digitalWrite(WizFi2x0_CS, LOW);
	result = SPI.transfer(0);
	digitalWrite(WizFi2x0_CS, HIGH);

	return result;
}

uint8_t WizFi2x0Class::read(byte *buf)
{
	int idx = 0;
	byte tmp;
	byte chk = 0;
	
	while(digitalRead(WizFi2x0_DataReady) == HIGH)
	{
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(tmp == spichar.SPI_ESC_CHAR)
				chk = 1;
			else
			{
				if(chk == 1)
				{
					chk = 0;
					tmp ^= 0x20;
				}			
				buf[idx++] = tmp;
			}
		}
	}

	return idx;
	
}

uint8_t WizFi2x0Class::read(byte *buf, size_t size)
{
	int idx = 0;
	byte tmp;
	byte chk = 0;
	
	while(digitalRead(WizFi2x0_DataReady) == HIGH)
	{
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(tmp == spichar.SPI_ESC_CHAR)
				chk = 1;
			else
			{
				if(chk == 1)
				{
					chk = 0;
					tmp ^= 0x20;
				}
				if(idx < size)
					buf[idx++] = tmp;
			}
		}
	}

	return idx;
	
}

void WizFi2x0Class::BufClear(void)
{
	memset((char *)MsgBuf, 0, sizeof(MsgBuf));
}


uint8_t WizFi2x0Class::SendCommand(uint8_t command)
{
	uint8_t retval;
	
	if(Current_CmdState == WizFi2x0_CmdState_IDLE)
	{
		memset((char *)MsgBuf, 0, sizeof(MsgBuf));
		MakeCommand(command);
		Current_CmdState = WizFi2x0_CmdState_Ready;
#ifdef DEBUG_ENABLE		
		Serial.println((char *)MsgBuf);
#endif
		return 5;
	}else if(Current_CmdState == WizFi2x0_CmdState_Ready)
	{
		ReplyCheckTimer.TimerStart(20000);
		write(MsgBuf);
		
		RxIdx = 0;
		Current_CmdState = WizFi2x0_CmdState_Sent;
		Current_ReplyState = WizFi2x0_ReplyState_IDLE;
	}else if(Current_CmdState == WizFi2x0_CmdState_Sent)
	{
		retval = CheckReply(command);
		if(retval == 1)
		{
			Current_CmdState = WizFi2x0_CmdState_IDLE;
			Current_ReplyState = WizFi2x0_ReplyState_IDLE;
			ReplyCheckTimer.TimerStop();
			return 1;
		}else if(retval == 2)
		{
			ReplyCheckTimer.TimerStop();
			Current_CmdState = WizFi2x0_CmdState_IDLE;
			Current_ReplyState = WizFi2x0_ReplyState_IDLE;
#ifdef DEBUG_ENABLE		
			Serial.println("Timeout  with ERROR reply");
#endif
			return 2; //Timeout  with ERROR reply
		}

		if(ReplyCheckTimer.GetIsTimeout())
		{
			ReplyCheckTimer.TimerStop();
			Current_CmdState = WizFi2x0_CmdState_IDLE;
			Current_ReplyState = WizFi2x0_ReplyState_IDLE;
#ifdef DEBUG_ENABLE		
			Serial.println("Timeout with no reply");
#endif
			return 3; //Timeout with no reply
		}
	}
	return 0;
}

void WizFi2x0Class::MakeCommand(uint8_t command)
{
	byte tmpstr[128];

	memset(tmpstr, 0, 128);
	
	if(command == OP_AT)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[0]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr);
	}else if(command == OP_ATE)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[1]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr);
	}else if(command == OP_WS)
	{
		if(m_channel == 0)
		{
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[2]))); // Necessary casts and dereferencing, just copy.
			sprintf((char *)MsgBuf, (char *)tmpstr);
		}else
		{
		       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[30]))); // Necessary casts and dereferencing, just copy.
			sprintf((char *)MsgBuf, (char *)tmpstr, m_channel);
		}
	}else if(command == OP_WA)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[3]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, SSID);
	}else if(command == OP_WAUTH)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[4]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr);
	}else if(command == OP_NMAC)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[5]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, SRCMACAddr);
	}else if(command == OP_XDUM)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[6]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr);
	}else if(command == OP_WD)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[7]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr);
	}else if(command == OP_WM)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[8]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, OperatingMode);
	}else if(command == OP_DHCPSRVR)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[9]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr);
	}else if(command == OP_WP)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[10]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, PowerLevel);
	}else if(command == OP_DNSSET)
	{
		if(!strcmp((char const *)(&DNSIPAddr[1][0]), "0.0.0.0"))
		{
	       	strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[11]))); // Necessary casts and dereferencing, just copy.
			sprintf((char *)MsgBuf, (char *)tmpstr, &DNSIPAddr[0][0]);
		}else
		{
	       	strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[12]))); // Necessary casts and dereferencing, just copy.
			sprintf((char *)MsgBuf, (char *)tmpstr, &DNSIPAddr[0][0], &DNSIPAddr[1][0]);	
		}
	}else if(command == OP_DNSLOOK)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[13]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, DomainName);
	}else if(command == OP_WAUTO)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[14]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, SSID);
	}else if(command == OP_NAUTO)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[15]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, Connection_Type, Socket_Type, PeerIPAddr, PeerPortNum);
	}else if(command == OP_NDHCP)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[16]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, DHCP_Flag);
	}else if(command == OP_WWEP)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[17]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, Key);
	}else if(command == OP_WWPA)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[18]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, Key);
	}else if(command == OP_WPAPSK)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[19]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, SSID, Key);
	}else if(command == OP_BDATA)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[20]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr);
	}else if(command == OP_ATA)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[21]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr);
	}else if(command == OP_NSET)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[22]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, SrcIPAddr, SrcSubnet, SrcGateway);
	}else if(command == OP_ATCID)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[23]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr);
	}else if(command == OP_ATO)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[24]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr);
	}else if(command == OP_NCTCP)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[25]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, PeerIPAddr, PeerPortNum);
	}else if(command == OP_NSTCP)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[26]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, SrcPortNum);
	}else if(command == OP_NCLOSE)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[27]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, CID);
	}else if(command == OP_NCLOSEALL)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[28]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr);
	}else if(command == OP_CMDMODE)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[29]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr);
	}else if(command == OP_NCUDP)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[41]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, PeerIPAddr, PeerPortNum, SrcPortNum);
	}else if(command == OP_NSUDP)
	{
	       strcpy_P((char *)tmpstr, (char*)pgm_read_word(&(string_table[42]))); // Necessary casts and dereferencing, just copy.
		sprintf((char *)MsgBuf, (char *)tmpstr, SrcPortNum);
	}
}

void WizFi2x0Class::ChangeESCState(uint8_t state)
{
	Current_ESC_State = state;
}

void WizFi2x0Class::RcvPacket(void)
{
	byte tmp;
	byte DBG_Buf[32];
	
	switch(Current_ESC_State)
	{
	case WizFi2x0_ESC_IDLE:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(tmp == ASCII_ESC)
			{
				Current_ESC_State = WizFi2x0_ESC_STARTED;
#ifdef DEBUG_ENABLE		
				Serial.println("<ESC>");
#endif
			}else if(tmp == '[')
			{
				Current_ESC_State = WizFi2x0_ESC_NOTIFY;
				memset(NotifyBuf, 0, 40);
				NotifyIdx = 0;
#ifdef DEBUG_ENABLE		
				Serial.println("NOTIFY Message was started");
#endif
			}else
			{
#ifdef DEBUG_ENABLE
				memset(DBG_Buf, 0, 10);
				sprintf((char *)DBG_Buf, " %02X", tmp);
				Serial.print((char)tmp);
				Serial.println((char *)DBG_Buf);
#endif
			}
		}
		break;
	case WizFi2x0_ESC_STARTED:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(tmp == 'Z')
			{
				Current_ESC_State = WizFi2x0_ESC_SOCK;
				Current_Sock_Type = PROTO_TCP;
#ifdef DEBUG_ENABLE		
				Serial.println("Z");
#endif
			}
			else if(tmp == 'y')
			{
				Current_ESC_State = WizFi2x0_ESC_SOCK;
				Current_Sock_Type = PROTO_UDP;
#ifdef DEBUG_ENABLE		
				Serial.println("y");
#endif
			}
			else
			{
#ifdef DEBUG_ENABLE		
				Serial.println((char)tmp);
#endif
				Current_ESC_State = WizFi2x0_ESC_IDLE;
			}
		}
		break;
	case WizFi2x0_ESC_SOCK:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(((tmp >= '0') && (tmp <= '9')) || ((tmp >= 'a') && (tmp <= 'f')))
			{
				if(Current_Sock_Type == PROTO_TCP)
				{
					Current_ESC_State = WizFi2x0_ESC_CID;
					CurrentSockIndex = tmp - '0';
					memset(RcvdBuf, 0, 128);
					ESC_Length_Idx = 0;
					ESC_Data_Length = 0;
#ifdef DEBUG_ENABLE		
					Serial.println((char)tmp);
#endif
				}else
				{
					Current_ESC_State = WizFi2x0_ESC_PEERIP;
					CurrentSockIndex = tmp - '0';
					memset(RcvdBuf, 0, 128);
					ESC_Length_Idx = 0;
					ESC_Data_Length = 0;
#ifdef DEBUG_ENABLE		
					Serial.println((char)tmp);
#endif
				}
			}else
			{
				Current_ESC_State = WizFi2x0_ESC_IDLE;
			}
		}
		break;
	case WizFi2x0_ESC_PEERIP:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(((tmp >= '0') && (tmp <= '9')) || (tmp == '.'))
				RcvdBuf[ESC_Length_Idx++] = tmp;
			else if(tmp == ' ')
			{
				memcpy(PeerIPAddr, RcvdBuf, 16);
				memset(RcvdBuf, 0, 128);
				ESC_Length_Idx = 0;
				ESC_Data_Length = 0;
				PeerPortNum = 0;
				
//				Serial.println((char *)PeerIPAddr);
				Current_ESC_State = WizFi2x0_ESC_PEERPORT;
			}
			else
				Current_ESC_State = WizFi2x0_ESC_IDLE;
		}
		break;
	case WizFi2x0_ESC_PEERPORT:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if((tmp >= '0') && (tmp <= '9'))
			{
				PeerPortNum *= 10;
				PeerPortNum += (uint16_t)(tmp - '0');
//				sprintf((char *)DBG_Buf, "%u", PeerPortNum);
//				Serial.println((char *)DBG_Buf);
			}
			else if(tmp == '\t')
			{
				memset(RcvdBuf, 0, 128);
				ESC_Length_Idx = 0;
				ESC_Data_Length = 0;
//				sprintf((char *)DBG_Buf, "%u", PeerPortNum);
//				Serial.println((char *)DBG_Buf);
				
				Current_ESC_State = WizFi2x0_ESC_CID;
			}
			else
				Current_ESC_State = WizFi2x0_ESC_IDLE;

		}
		break;
	case WizFi2x0_ESC_CID:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if((tmp >= '0') && (tmp <= '9'))
			{
				ESC_Data_Length *= 10;
				ESC_Data_Length += (uint16_t)(tmp - '0');
				
				if(++ESC_Length_Idx == 4)
				{
					Current_ESC_State = WizFi2x0_ESC_LENGTH;
					ESC_Data_Idx = 0;
					Current_ESC_Data_Length = 0;
#ifdef DEBUG_ENABLE		
					Serial.print("Data Length: ");
					sprintf((char *)DBG_Buf, "%d ", ESC_Data_Length);
					Serial.println((char *)DBG_Buf);
#endif					
				}
#ifdef DEBUG_ENABLE		
				Serial.println((char)tmp);
#endif
			}else
			{
				Current_ESC_State = WizFi2x0_ESC_IDLE;
			}
		}
		break;
	case WizFi2x0_ESC_LENGTH:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			RcvdBuf[writePtr++] = tmp;
			Current_ESC_Data_Length++;
			if(writePtr == 128)
				writePtr = 0;
			
#ifdef DEBUG_ENABLE		
			Serial.print((char)tmp);
#endif
			ESC_Data_Idx++;

			if(ESC_Data_Length > 128)
			{
				if(Current_ESC_Data_Length == 128)
					IsDataRcvd[CurrentSockIndex] = true;
			}
			
			if(ESC_Data_Idx == ESC_Data_Length)
			{
#ifdef DEBUG_ENABLE		
				Serial.println("All Data Received");
#endif				

				if((Current_ESC_Data_Length > 0) && (IsDataRcvd[CurrentSockIndex] == false))
					IsDataRcvd[CurrentSockIndex] = true;
				
				Current_ESC_State = WizFi2x0_ESC_IDLE;
			}
		}
		break;
	case WizFi2x0_ESC_NOTIFY:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(tmp != ']')
				NotifyBuf[NotifyIdx++] = tmp;
			else
			{
				NotifyBuf[NotifyIdx++] = tmp;
				RxIdx = NotifyIdx;
#ifdef DEBUG_ENABLE		
				Serial.println("Enter ParseNotify: ");
				Serial.println((char *)NotifyBuf);
#endif

				ParseNotify(NotifyBuf);
				Current_ESC_State = WizFi2x0_ESC_IDLE;
#ifdef DEBUG_ENABLE		
				Serial.println("NOTIFY Message was ended");
#endif

			}
		}
		break;
	case WizFi2x0_ESC_NOTI_OK:
		break;
	}
}

uint8_t WizFi2x0Class::ParseNotify(byte *buf)
{
	int retval;
	long retval32;
	uint8_t Token[33];
	
	Current_Ptr = 0;
	retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
	Serial.println((char *)Token);
#endif
	if(!strcmp((char const*)Token, "DISCONNECT"))
	{
		retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
		Serial.println((char *)Token);
#endif
		if(retval == -1)
		{
			return -1;
		}else
		{
			if(Token[0] >= '0' && Token[0] <= '9')						
				NOTI_CH = Token[0];
			else if(Token[0] >= 'a' && Token[0] <= 'f')
				NOTI_CH = Token[0];
			NOTI_TYPE = DISCONNECT_NOTI;
			
			return 1;
		}
	}else if(!strcmp((char const*)Token, "CONNECT"))
	{
		retval = GetToken(buf, Token);
		if(retval == -1)
			return -1;
#ifdef DEBUG_ENABLE		
		Serial.println((char *)Token);
#endif
		retval = GetToken(buf, Token);
		if(retval == -1)
			return -1;
		if(Token[0] >= '0' && Token[0] <= '9')
		{
			NOTI_CH = Token[0];
			if(Token[1] == ']')
				return 1;
		}else if(Token[0] >= 'a' && Token[0] <= 'f')
		{
			NOTI_CH = Token[0];
			if(Token[1] == ']')
				return 1;
		}else
			return -1;

#ifdef DEBUG_ENABLE		
		Serial.println((char *)Token);
#endif
		retval = GetToken(buf, Token);
		if(retval == -1)
			return -1;
		SetPeerIPAddr((byte *)Token);
#ifdef DEBUG_ENABLE		
		Serial.println((char *)Token);
#endif
		retval = GetToken(buf, Token);
		if(retval == -1)
			return -1;
#ifdef DEBUG_ENABLE		
		Serial.println((char *)Token);
#endif
		retval32 = StrToInt(Token);
		if(retval32 == -1)
			return -1;
		SetPeerPortnum(retval32);
#ifdef DEBUG_ENABLE		
		Serial.println((char *)Token);
#endif

		NOTI_TYPE = CONNECT_NOTI;

		return 1;
		
	}else if(!strcmp((char const*)Token, "ERROR"))
	{
		NOTI_TYPE = SOCKFAILURE_NOTI;
		
		return 1;
	}else
	{
		NOTI_TYPE = INVALID_NOTI;

		return 1;
	}
}

#ifdef LINE_PARSE
uint8_t WizFi2x0Class::CheckReply(uint8_t command)
{
	byte tmp;
	uint8_t retval;
	bool flag;
	

	tmp = read();
	
	if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
	{
		if(tmp != 0x0D && tmp != 0x0A)
		{
			MsgBuf[RxIdx++] = tmp;
		}else{
			if(tmp == 0x0A)
			{
				if(RxIdx > 0)
				{
#ifdef DEBUG_ENABLE		
					Serial.println((char *)MsgBuf);
#endif
					retval = ParseReply(MsgBuf, command);
					memset(MsgBuf, 0, sizeof(MsgBuf));
					RxIdx = 0;

					if(retval == 1 || retval == 2)
					{
						/////////////////////////////////////////	
						//Flush rx data
						while(digitalRead(WizFi2x0_DataReady) == HIGH)
						{
							tmp = read();
						}
					}
					return retval;
					
				}else
					return 0;
			}
		}
	}

	return 0;
}

#else
uint8_t WizFi2x0Class::CheckReply(uint8_t command)
{
	byte tmp;

	tmp = read();
	if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
	{
		MsgBuf[RxIdx++] = tmp;
	}

	if(tmp == ']')
	{
		if(command == OP_NSTCP)
		{
			if(bFirstEnd == false)
			{
				bFirstEnd = true;
				return 0;
			}else
				bFirstEnd = false;
		}else
			bFirstEnd = false;
		
		/////////////////////////////////////////	
		//Flush rx data
		while(digitalRead(WizFi2x0_DataReady) == HIGH)
		{
			tmp = read();
		}
		//
		////////////////////////////////////////
#ifdef DEBUG_ENABLE		
		Serial.println((char *)MsgBuf);
#endif
		if(ParseReply(MsgBuf, command))
			return 1; // Reply is OK
		else
			return 2; // Reply is FAILED
	}

	return 0; // Reply is in progress
	
}
#endif

#ifdef LINE_PARSE
uint8_t WizFi2x0Class::ParseReply(byte * buf, uint8_t command)
{
	int retval;
	uint8_t Token[33];
	char tmpStr[5];
	uint8_t APListIndex;
	
	switch(command)
	{
	case OP_AT: 
	case OP_ATE:
	case OP_WD:
	case OP_WP:
	case OP_WM:
	case OP_WWEP:
	case OP_WWPA:
	case OP_WPAPSK:
	case OP_NDHCP:
	case OP_WAUTO:
	case OP_NAUTO:
	case OP_DNSSET:
	case OP_NSET:
	case OP_ATO:
	case OP_NCLOSE:
	case OP_NCLOSEALL:
	case OP_BDATA:
	case OP_XDUM:
	case OP_NMAC:
	case OP_DHCPSRVR:
	case OP_WAUTH:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
		while(1)
		{
			if(retval == -1){
#ifdef DEBUG_ENABLE		
				Serial.println("-1");
#endif
				return 0;
			}
#ifdef DEBUG_ENABLE		
			Serial.println((char *)Token);
#endif
			if(!strcmp((char const *)Token, "[OK]"))
				return 1;		
			else if((!strcmp((char const *)Token, "[ERROR]")) || (!strcmp((char const *)Token, "[ERROR:")))
				return 2; //Failed
			retval = GetToken(buf, Token);
		}
		break;
	case OP_DNSLOOK:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
		Serial.println((char *)buf);
#endif
		while(1)
		{
			if(retval == -1)
				return 0;
			switch(Current_ReplyState)
			{
			case WizFi2x0_ReplyState_IDLE:
				if(!strcmp((char const *)Token, "IP:"))
					Current_ReplyState = WizFi2x0_ReplyState_IP_HEAD;
				break;
			case WizFi2x0_ReplyState_IP_HEAD:
				SetPeerIPAddr(Token);
				Current_ReplyState = WizFi2x0_ReplyState_IP;
				break;
			case WizFi2x0_ReplyState_IP:
				if(!strcmp((char const*)Token, "[OK]"))
					return 1;
				else if(!strcmp((char const *)Token, "[ERROR]"))
					return 2;
				break;
			}
			retval = GetToken(buf, Token);
		}
		break;
	case OP_NCTCP:
	case OP_NCUDP:
	case OP_NSUDP:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
		Serial.println((char *)buf);
#endif
		while(1)
		{
			if(retval == -1){
				return 0;
			}
#ifdef DEBUG_ENABLE		
			Serial.println((char *)Token);
#endif
			switch(Current_ReplyState)
			{
			case WizFi2x0_ReplyState_IDLE:
				if(!strcmp((char const*)Token, "[CONNECT"))
					Current_ReplyState = WizFi2x0_ReplyState_CONNECT;
				break;
			case WizFi2x0_ReplyState_CONNECT:
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				if(Token[0] >= '0' && Token[0] <= '9')
				{
					CID = Token[0];
					Current_ReplyState = WizFi2x0_ReplyState_CID;
				}
				else if(Token[0] >= 'a' && Token[0] <= 'f')
				{
					CID = Token[0];
					Current_ReplyState = WizFi2x0_ReplyState_CID;
				}else
					return 0;
				break;
			case WizFi2x0_ReplyState_CID:
				while(1){
					if(retval == -1)
						return 2;
					if(!strcmp((char const*)Token, "[OK]"))
						return 1;
					retval = GetToken(buf, Token);
				}
			}
			retval = GetToken(buf, Token);
		}
		break;
	case OP_NSTCP:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
		Serial.println((char *)buf);
#endif
		while(1)
		{
			if(retval == -1)
				return 0;
			switch(Current_ReplyState)
			{
			case WizFi2x0_ReplyState_IDLE:
				if(!strcmp((char const*)Token, "[CONNECT"))
					Current_ReplyState = WizFi2x0_ReplyState_CONNECT;
				break;
			case WizFi2x0_ReplyState_CONNECT:
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				if(Token[0] >= '0' && Token[0] <= '9')
				{
					CID = Token[0];
					Current_ReplyState = WizFi2x0_ReplyState_CID;
				}
				else if(Token[0] >= 'a' && Token[0] <= 'f')
				{
					CID = Token[0];
					Current_ReplyState = WizFi2x0_ReplyState_CID;
				}else
					return 2;
				break;
			case WizFi2x0_ReplyState_CID:
				while(1){
					if(retval == -1)
						return 2;
					if(!strcmp((char const*)Token, "[OK]"))
						return 1;
					retval = GetToken(buf, Token);
				}					
				break;
			}
			retval = GetToken(buf, Token);
		}
		break;
	case OP_WA:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
		Serial.println((char *)buf);
#endif
		while(1)
		{
			if(retval == -1)
				return 0;
			switch(Current_ReplyState)
			{
			case WizFi2x0_ReplyState_IDLE:
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				if(!strcmp((char const *)Token, "Gateway"))
				{
					Current_ReplyState = WizFi2x0_ReplyState_IP_HEAD;
					return 0;
				}else if(!strcmp((char const *)Token, "[ERROR]"))
					return 2;
				break;
			case WizFi2x0_ReplyState_IP_HEAD:
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				if(Token[retval - 1] == ':')
					Token[retval - 1] = '\0';
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				SetSrcIPAddr(Token);
				Current_ReplyState = WizFi2x0_ReplyState_IP;
				break;
			case WizFi2x0_ReplyState_IP:
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				if(Token[retval - 1] == ':')
					Token[retval - 1] = '\0';
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				SetSrcSubnet(Token);
				Current_ReplyState = WizFi2x0_ReplyState_SUBNET;
				break;
			case WizFi2x0_ReplyState_SUBNET:
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				if(Token[retval - 1] == ':')
					Token[retval - 1] = '\0';
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				SetSrcGateway(Token);
				Current_ReplyState = WizFi2x0_ReplyState_GATEWAY;
				break;
			case WizFi2x0_ReplyState_GATEWAY:
				while(1)
				{
					if(retval == -1)
						return 2;
					if(!strcmp((char const*)Token, "[OK]"))
						return 1;

					retval = GetToken(buf, Token);
				}
			}
			retval = GetToken(buf, Token);
		}
		break;
	case OP_WS:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
		Serial.println((char *)buf);
#endif

		if(!strcmp((char const *)Token, "No.Of")){
			Current_ReplyState = WizFi2x0_ReplyState_NOofAP;
		}			

		while(1)
		{
			if(retval == -1){
				if(Current_ReplyState != WizFi2x0_ReplyState_IP_HEAD)
					Current_ReplyState = WizFi2x0_ReplyState_IP_HEAD;
				return 0;
			}
			switch(Current_ReplyState)
			{
			case WizFi2x0_ReplyState_IDLE:
				Scan_AP = true;
				
				if(!strcmp((char const *)Token, "Security")){
					Current_ReplyState = WizFi2x0_ReplyState_IP_HEAD;
				}			
				break;
			case WizFi2x0_ReplyState_IP_HEAD:
				sprintf(Scan_BSSID, "%s", Token);
#ifdef DEBUG_ENABLE		
				Serial.print((char *)Token);
				Serial.print("\t");
#endif
				Current_ReplyState = WizFi2x0_ReplyState_BSSID;
				break;
			case WizFi2x0_ReplyState_BSSID:
				if(Token[2] != '\0')
				{
					sprintf(Scan_SSID, "%s", Token);
#ifdef DEBUG_ENABLE		
					Serial.print((char *)Token);
					Serial.print("\t");
#endif
					Current_ReplyState = WizFi2x0_ReplyState_SSID;
				}
				else
				{
					if((Token[0] >= '0' && Token[0] <= '9') && (Token[1] >= '0' && Token[1] <= '9'))
					{
						Scan_SSID[0] = '\0';
						sprintf(Scan_CH, "%s", Token); 
						Current_ReplyState = WizFi2x0_ReplyState_CH;
					}else
					{
						sprintf(Scan_SSID, "%s", Token);
						Current_ReplyState = WizFi2x0_ReplyState_SSID;
					}
				}
#ifdef DEBUG_ENABLE		
				Serial.print((char *)buf);
				Serial.print("\t");
#endif	
				break;
			case WizFi2x0_ReplyState_SSID:
				sprintf(Scan_CH, "%s", Token);
				if(m_channel != 0)
				{
					sprintf(tmpStr, "%02d", m_channel);
					if(strcmp(tmpStr, Scan_CH)!=0)
						Scan_AP = false;
				}
#ifdef DEBUG_ENABLE		
				Serial.print((char *)Token);
				Serial.print("\t");
#endif
				Current_ReplyState = WizFi2x0_ReplyState_CH;
				break;
			case WizFi2x0_ReplyState_CH:
				sprintf(Scan_TYPE, "%s", Token);
#ifdef DEBUG_ENABLE		
				Serial.print((char *)Token);
				Serial.print("\t");
#endif
				Current_ReplyState = WizFi2x0_ReplyState_TYPE;
				break;
			case WizFi2x0_ReplyState_TYPE:
				sprintf(Scan_RSSI, "%s", Token);
				if(Scan_AP && (m_filterRSSI != 0))
					Scan_AP = CheckRSSIPower(m_filterRSSI, Scan_RSSI);
#ifdef DEBUG_ENABLE		
				Serial.print((char *)Token);
				Serial.print("\t");
#endif
				Current_ReplyState = WizFi2x0_ReplyState_RSSI;
				break;
			case WizFi2x0_ReplyState_RSSI:
				sprintf(Scan_SECURITY, "%s", Token);
#ifdef DEBUG_ENABLE		
				Serial.print((char *)Token);
				Serial.print("\r\n");
#endif
				Current_ReplyState = WizFi2x0_ReplyState_SECURITY;

				if(Scan_AP)
				{
//					Serial.print((char *)Scan_BSSID);
//					Serial.print("\t");
					Serial.print((char *)Scan_SSID);
//					Serial.print("\t");
//					Serial.print((char *)Scan_CH);
//					Serial.print("\t");
//					Serial.print((char *)Scan_TYPE);
//					Serial.print("\t");
//					Serial.print((char *)Scan_RSSI);
//					Serial.print("\t");
//					Serial.print((char *)Scan_SECURITY);
					Serial.print("\r\n");
				}

				Scan_AP = true;
				break;
			case WizFi2x0_ReplyState_SECURITY:
				sprintf(Scan_BSSID, "%s", Token);
#ifdef DEBUG_ENABLE		
				Serial.print((char *)buf);
				Serial.print("\t");
#endif
				Current_ReplyState = WizFi2x0_ReplyState_BSSID;
				break;
			case WizFi2x0_ReplyState_NOofAP:
				while(1)
				{
					if(retval == -1)
					{
						Current_ReplyState = WizFi2x0_ReplyState_WaitOK;
						return 0;
					}
					retval = GetToken(buf, Token);
				}
				break;
			case WizFi2x0_ReplyState_WaitOK:
				while(1)
				{
					if(retval == -1)
						return 2;
					if(!strcmp((char const *)Token, "[OK]"))
						return 1;
					retval = GetToken(buf, Token);
				}
				break;
			}
			retval = GetToken(buf, Token);
		}
		break;
	case OP_ATA:
	case OP_ATCID:
		return 1;
	}
	return 0;
}

#else
uint8_t WizFi2x0Class::ParseReply(byte * buf, uint8_t command)
{
	int retval;
	uint8_t Token[33];
	uint8_t tmpToken[33];
	uint8_t APListIndex;
	
	switch(command)
	{
	case OP_AT: 
	case OP_ATE:
	case OP_WD:
	case OP_WP:
	case OP_WM:
	case OP_WWEP:
	case OP_WWPA:
	case OP_WPAPSK:
	case OP_NDHCP:
	case OP_WAUTO:
	case OP_NAUTO:
	case OP_DNSSET:
	case OP_NSET:
	case OP_ATO:
	case OP_NCLOSE:
	case OP_NCLOSEALL:
	case OP_BDATA:
	case OP_DHCPSRVR:
	case OP_NMAC:
	case OP_WAUTH:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
		Serial.println((char *)Token);
#endif
		while(1)
		{
			if(!strcmp((char const *)Token, "[OK]"))
				return 1;
			else
			{
				retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				if(!strcmp((char const *)Token, "[OK]"))
					return 1;
				if(retval == -1)
					return 0;
			}
		}
	case OP_DNSLOOK:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
		Serial.println((char *)Token);
#endif

		while(1)
		{
			if(!strcmp((char const *)Token, "IP"))
			{
				retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				if(retval > 0)
					SetPeerIPAddr(Token);
				else if(retval == -1)
					return 0;

				while(1)
				{
					retval = GetToken(buf, Token);
					if(!strcmp((char const *)Token, "[OK]"))
						return 1;
					if(retval == -1)
						return 0;
				}
			}
			else
			{
				retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				if(retval == -1)
					return 0;
			}
		}
	case OP_XDUM:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
		Serial.println((char *)Token);
#endif

		while(1)
		{
			if(!strcmp((char const *)Token, "[OK]"))
				return 1;
			else
			{
				retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				if(retval == -1)
					return 0;
			}
		}
		
	case OP_NCTCP:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);

		while(1)
		{
#ifdef DEBUG_ENABLE		
			if(retval != 0)
				Serial.println((char *)Token);
#endif
			if(!strcmp((char const*)Token, "[CONNECT"))
			{
				retval = GetToken(buf, Token);
				if(retval!=0)
				{
#ifdef DEBUG_ENABLE		
					Serial.println((char *)Token);
#endif
					if(Token[0] >= '0' && Token[0] <= '9')						
						CID = Token[0];
					else if(Token[0] >= 'a' && Token[0] <= 'f')
						CID = Token[0];

					return 1;
				}
				
			}else
			{
				retval = GetToken(buf, Token);
				if(retval == -1)
					return 0;
			}
		}
	case OP_NSTCP:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
		Serial.println("NSTCP");
		Serial.println((char *)Token);
#endif

		while(1)
		{			
			if(!strcmp((char const*)Token, "[CONNECT"))
			{
				retval = GetToken(buf, Token);
				if(retval<=0)
				{
					return 0;
				}
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif
				if(Token[0] >= '0' && Token[0] <= '9')						
					CID = Token[0];
				else if(Token[0] >= 'a' && Token[0] <= 'f')
					CID = Token[0];

				while(1)
				{
					retval = GetToken(buf, Token);
#ifdef DEBUG_ENABLE		
					Serial.println((char *)Token);
#endif
					if(!strcmp((char const*)Token, "[OK]"))
						return 1;
					if(retval == -1)
						return 0;
				}
			}else
			{
				retval = GetToken(buf, Token);
				if(retval == -1)
					return 0;
			}
		}
	case OP_WA:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
		
		while(1)
		{
#ifdef DEBUG_ENABLE		
			if(retval != 0)
				Serial.println((char *)Token);
#endif
			if(!strcmp((char const *)Token, "Gateway"))
			{
				while(GetToken(buf, Token) == 0);
#ifdef DEBUG_ENABLE		
				Serial.print("SrcIPAddr: ");
				Serial.println((char *)Token);
#endif
				SetSrcIPAddr(Token);
				while(GetToken(buf, Token) == 0);
#ifdef DEBUG_ENABLE		
				Serial.print("SrcSubnet: ");
				Serial.println((char *)Token);
#endif
				SetSrcSubnet(Token);
				while(GetToken(buf, Token) == 0);
#ifdef DEBUG_ENABLE		
				Serial.print("SrcGateway: ");
				Serial.println((char *)Token);
#endif
				SetSrcGateway(Token);
#ifdef DEBUG_ENABLE		
				Serial.println((char *)Token);
#endif

				while(1)
				{
					retval = GetToken(buf, Token);

					if(retval == -1)
						return 0;
					
					if(!strcmp((char const *)Token, "[OK]"))
						return 1;
				}
			}else
			{
				retval = GetToken(buf, Token);
				if(retval == -1)
					return 0;
			}
		}
	case OP_WS:

		Current_Ptr = 0;
		retval = GetToken(buf, Token);

		return 1;
#if 0
		while(1)
		{
			if(!strcmp((char const *)Token, "BSSIDSSIDChannelTypeRSSISecurity"))
			{
				retval = GetToken(Token);
//				printf("\r\n Token Length: %d, Token : %s", retval, Token);
				memset(tmpToken, 0, 33);
				memcpy(tmpToken, Token, 13);
				APListIndex = WIZFI210_GetAPListCount();
				while(strcmp((char const *)tmpToken, "No.OfAPFound:")!=0)
				{
//					printf("\r\nBSSI: %s", Token);
					WIZFI210_SetBSSID(APListIndex, Token);
					
					retval = GetToken(Token);
//					printf("\tSSID: %s", Token);
					WIZFI210_SetSSID(APListIndex, Token);
					
					retval = GetToken(Token);
//					printf("\t\tChannel: %s", Token);
					WIZFI210_SetChannel(APListIndex, Token);
					
					retval = GetToken(Token);
//					printf("\tType: %s", Token);
					WIZFI210_SetType(APListIndex, Token);

					retval = GetToken(Token);
//					printf("\tRSSI: %s", Token);
					WIZFI210_SetRSSI(APListIndex, Token);

					retval = GetToken(Token);
//					printf("\t\tSecurity: %s", Token);
					WIZFI210_SetSecurity(APListIndex, Token);

					APListIndex++;
					WIZFI210_SetAPListCount(APListIndex);
					
					retval = GetToken(Token);
					memset(tmpToken, 0, 33);
					memcpy(tmpToken, Token, 13);
				}
				retval = GetToken(Token);
//				printf("\r\n %s", Token);
				if(!strcmp((char const *)Token, "[OK]"))
					return 1;
				else
					return 0;
			}else
			{
				retval = GetToken(Token);
				if(retval == -1)
					return 0;
			}
		}
#endif
	case OP_ATA:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);

		while(1)
		{
			if(!strcmp((char const *)Token, "[OK]"))
			{
				return 1;
			}
			else
			{
				memset(tmpToken, 0, 33);
				memcpy(tmpToken, Token, 7);
				if(!strcmp((char const *)tmpToken, "[ERROR]"))
					return 0;
				retval = GetToken(buf, Token);
				if(!strcmp((char const *)Token, "[OK]"))
				{
					return 1;
				}
				if(retval == -1)
					return 0;
			}
		}
	case OP_ATCID:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);

		while(1)
		{
			if(!strcmp((char const *)Token, "NovalidCids"))
			{				
				return 1;
			}
			else if(!strcmp((char const *)Token, "CIDTYPEMODELOCALPORTREMOTEPORTREMOTEIP"))
			{
				return 1;
			}
			retval = GetToken(buf, Token);
			if(retval == -1)
				return 0;
		}
	}
	return 0;
}

#endif

#ifdef LINE_PARSE
int WizFi2x0Class::GetToken(byte * buf, uint8_t * Token)
{
	uint8_t i = 0;
	memset(Token, 0, 33);

	while(Current_Ptr <= RxIdx)
	{
		
		if(buf[Current_Ptr] != ',' && buf[Current_Ptr] != ' ' && buf[Current_Ptr] != '\t' && buf[Current_Ptr] != 0x0D && buf[Current_Ptr] != 0x0A && buf[Current_Ptr] != '\0')
		{
			Token[i++] = buf[Current_Ptr++];
		}
		else
		{
			if((buf[Current_Ptr] == ',') ||(buf[Current_Ptr] == '\0')||(buf[Current_Ptr] == ':')
				||(buf[Current_Ptr] == ' ') ||(buf[Current_Ptr] == '\t') ||(buf[Current_Ptr] == 0x0D) ||(buf[Current_Ptr] == 0x0A))
			{
				Current_Ptr++;
				if(i > 0)
					return i;
			}else if(buf[Current_Ptr] == 0x0D)
			{
				if(buf[Current_Ptr + 1] == 0x0A)
				{
					Current_Ptr += 2;
					if(i > 0)
						return i;
				}else
				{
					Current_Ptr++;
					if(i > 0)
						return i;
				}
			}else if(buf[Current_Ptr] == 0x0A)
			{
				if(buf[Current_Ptr + 1] == 0x0D)
				{
					Current_Ptr += 2;
					if(i > 0)
						return i;
				}else
				{
					Current_Ptr++;
					if(i > 0)
						return i;
				}
			}
		}
	}

	return -1;
}

#else
int WizFi2x0Class::GetToken(byte * buf, uint8_t * Token)
{
	uint8_t i = 0;
	memset(Token, 0, 33);

	while(Current_Ptr <= RxIdx)
	{
		
		if(buf[Current_Ptr] != ',' && buf[Current_Ptr] != 0x0D && buf[Current_Ptr] != 0x0A && buf[Current_Ptr] != '\0' 
			&& buf[Current_Ptr] != ':' && buf[Current_Ptr] != ' ' && buf[Current_Ptr] != '\t')
		{
			Token[i++] = buf[Current_Ptr++];
		}
		else
		{
			if((buf[Current_Ptr] == ',') ||(buf[Current_Ptr] == '\0')||(buf[Current_Ptr] == ':')
				||(buf[Current_Ptr] == ' ') ||(buf[Current_Ptr] == '\t'))
			{
				Current_Ptr++;
				if(i > 0)
					return i;
			}else if(buf[Current_Ptr] == 0x0D)
			{
				if(buf[Current_Ptr + 1] == 0x0A)
				{
					Current_Ptr += 2;
					if(i > 0)
						return i;
				}else
				{
					Current_Ptr++;
					if(i > 0)
						return i;
				}
			}else if(buf[Current_Ptr] == 0x0A)
			{
				if(buf[Current_Ptr + 1] == 0x0D)
				{
					Current_Ptr += 2;
					if(i > 0)
						return i;
				}else
				{
					Current_Ptr++;
					if(i > 0)
						return i;
				}
			}
		}
	}

	return -1;
}
#endif

uint8_t WizFi2x0Class::CheckSyncReply(void)
{
	byte key;

	key = read();
	if((key != spichar.SPI_IDLE_CHAR) && ( key != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (key != spichar.SPI_INVALID_CHAR_ALL_ONE))
	{
		;
	}

	if(digitalRead(WizFi2x0_DataReady) == LOW)
		return 1;

	delay(10); // wait during 10ms
	
	return 0;
}

void WizFi2x0Class::SendSync(void)
{
	write(spichar.SPI_IDLE_CHAR);	
}

boolean WizFi2x0Class::ByteStuff(byte *ch)
{
	if((*ch == spichar.SPI_ESC_CHAR) ||
		(*ch == spichar.SPI_XON_CHAR) ||
		(*ch == spichar.SPI_XOFF_CHAR) ||
		(*ch == spichar.SPI_IDLE_CHAR) ||
		(*ch == spichar.SPI_INVALID_CHAR_ALL_ONE) ||
		(*ch == spichar.SPI_INVALID_CHAR_ALL_ZERO) ||
		(*ch == spichar.SPI_LINK_READY) )
	{
		*ch = *ch^0x20;
		return true;
	}else
		return false;
}

void WizFi2x0Class::StrToStr(byte * dst, byte * src, size_t size)
{
	memset((char *)dst, 0, sizeof(size));
	sprintf((char *)dst, (char *)src);
}

void WizFi2x0Class::SetSSID(byte *buf)
{
	StrToStr(SSID, buf, sizeof(SSID));
}

void WizFi2x0Class::GetSSID(byte * buf)
{
	StrToStr(buf, SSID, sizeof(SSID));
}

void WizFi2x0Class::SetKEY(byte *buf)
{
	StrToStr(Key, buf, sizeof(Key));
}

void WizFi2x0Class::GetKEY(byte *buf)
{
	StrToStr(buf, Key, sizeof(Key));
}

void WizFi2x0Class::SetSrcIPAddr(byte *buf)
{
	StrToStr(SrcIPAddr, buf, sizeof(SrcIPAddr));
}

void WizFi2x0Class::GetSrcIPAddr(byte *buf)
{
	StrToStr(buf, SrcIPAddr, sizeof(SrcIPAddr));
}

void WizFi2x0Class::SetSrcSubnet(byte *buf)
{
	StrToStr(SrcSubnet, buf, sizeof(SrcSubnet));
}

void WizFi2x0Class::GetSrcSubnet(byte *buf)
{
	StrToStr(buf, SrcSubnet, sizeof(SrcSubnet));
}

void WizFi2x0Class::SetSrcGateway(byte *buf)
{
	StrToStr(SrcGateway, buf, sizeof(SrcGateway));
}

void WizFi2x0Class::GetSrcGateway(byte * buf)
{
	StrToStr(buf, SrcGateway, sizeof(SrcGateway));
}

void WizFi2x0Class::SetDomainName(byte *buf)
{
	StrToStr(DomainName, buf, sizeof(DomainName));
}

void WizFi2x0Class::GetDomainName(byte *buf)
{
	StrToStr(buf, DomainName, sizeof(DomainName));
}

/*
uint8_t WizFi2x0Class::SetDNSIPAddr(byte *buf1, byte *buf2)
{
	uint8_t retval;
	
	Current_Command_Code = OP_DNSSET;
	Current_CmdState = WizFi2x0_CmdState_IDLE;

	StrToStr(&DNSIPAddr[0][0], buf1, 16);
	StrToStr(&DNSIPAddr[1][0], buf2, 16);
	
	while(1)
	{
		retval = SendCommand(Current_Command_Code);

		if(retval == 1)
		{
			return 1;
		}else
		{
			StrToStr(&DNSIPAddr[0][0], (byte *)"0.0.0.0", 16);
			StrToStr(&DNSIPAddr[1][0], (byte *)"0.0.0.0", 16);
			
			return retval;
		}
	}
	
}

void WizFi2x0Class::GetDNSIPAddr(byte *buf1, byte *buf2)
{
	StrToStr(buf1, &DNSIPAddr[0][0], 16);
	StrToStr(buf2, &DNSIPAddr[1][0], 16);
}
*/

void WizFi2x0Class::SetSrcPortnum(unsigned int portnum)
{
	uint8_t tmpstr[20];
	
	SrcPortNum = portnum;

	sprintf((char *)tmpstr, "SPort: %u", SrcPortNum);	
	Serial.println("SerSrcPortnum");
	Serial.println((char *)tmpstr);
}

uint16_t WizFi2x0Class::GetSrcPortnum(void)
{
	return SrcPortNum;
}

void WizFi2x0Class::SetPeerIPAddr(byte *buf)
{
	memset((char *)PeerIPAddr, 0, sizeof(PeerIPAddr));
	sprintf((char *)PeerIPAddr, (char *)buf);
}

void WizFi2x0Class::GetPeerIPAddr(byte *buf)
{
	memset((char *)buf, 0, sizeof(PeerIPAddr));
	sprintf((char *)buf, (char *)PeerIPAddr);
}

void WizFi2x0Class::SetPeerPortnum(uint16_t portnum)
{
	PeerPortNum = portnum;
}

uint16_t WizFi2x0Class::GetPeerPortnum(void)
{
	return PeerPortNum;
}

void WizFi2x0Class::SetDHCPFlag(uint8_t flag)
{
	DHCP_Flag = flag;
}

uint8_t WizFi2x0Class::GetDHCPFlag(void)
{
	return DHCP_Flag;
}

void WizFi2x0Class::SetConnectionType(uint8_t flag)
{
	Connection_Type = flag;
}

uint8_t WizFi2x0Class::GetConnectionType(void)
{
	return Connection_Type;
}

void WizFi2x0Class::SetSocketType(uint8_t flag)
{
	Socket_Type = flag;
}

uint8_t WizFi2x0Class::GetSocetType(void)
{
	return Socket_Type;
}

void WizFi2x0Class::SetSecurityType(uint8_t type)
{
	SecurityType = type;
}

uint8_t WizFi2x0Class::GetSecurityType(void)
{
	return SecurityType;
}

int WizFi2x0Class::SetIP(uint8_t* IP, byte * buf)
{
	uint8_t i = 0, index=0, pos=0;
	byte tmpBuf[5];
	int retval;
	
	memset(tmpBuf, 0, 5);
#ifdef DEBUG_ENABLE		
	Serial.print("GetPeerIPAddr() retval: ");
	Serial.println((char *)buf);
#endif

	while(1)
	{
#ifdef DEBUG_ENABLE		
		Serial.print(i);
		Serial.print(MsgBuf[Current_Ptr]);
		Serial.print(buf[index]);
		Serial.print("\t\t\t");
		Serial.println((char *)tmpBuf);
#endif
		if(buf[index] != '.' && buf[index] != '\0' )
		{
			tmpBuf[i++] = buf[index++];
		}
		else
		{
			if(buf[index] == '.' )
			{
				index++;
				tmpBuf[i] = '\0';
#ifdef DEBUG_ENABLE		
				Serial.println((char *)tmpBuf);
#endif
				retval = StrToByte(tmpBuf);
				memset(tmpBuf, 0, 5);
				i = 0;
				if(retval == -1)
					return retval;
				IP[pos++] = (uint8_t)retval;
			}else if(buf[index] == '\0' )
			{
				tmpBuf[i] = '\0';
				retval = StrToByte(tmpBuf);
				if(retval == -1)
					return retval;
				IP[pos] = (uint8_t)retval;
				return 1;
			}
		}
	}

	return -1;	
}
int WizFi2x0Class::StrToByte(byte *buf)
{
	uint8_t i = 0;
	int retval = 0;

	while(1)
	{
		if(i > 3)
			return -1;
		if(buf[i] >= '0' && buf[i] <= '9')
		{
			retval *= 10;
			retval += buf[i] - '0';
		}else if(buf[i] == '\0')
		{
			if(retval >= 0 && retval <= 255)
				return retval;
			else
				return -1;
		}else
			return -1;
		i++;
	}
}

long WizFi2x0Class::StrToInt(byte *buf)
{
	uint8_t i = 0;
	byte tmpstr[20];
	
	long retval = 0;

	while(1)
	{		
		if(i > 5)
			return -1;
		if(buf[i] >= '0' && buf[i] <= '9')
		{
			retval *= 10;
			retval += buf[i] - '0';
		}else if(buf[i] == '\0' || buf[i] == ']')
		{
			if(retval >= 0 && retval <= 65535)
				return retval;
			else
				return -1;
		}else
			return -1;
		i++;
	}
}

void TimeoutClass::init(void)
{
	TimerValue = 0;
	IsTimeout = false;
	TimerCount = 0;
	IsTimerStarted = false;
}


void TimeoutClass::TimerStart(uint16_t timevalue)
{
	TimerValue = timevalue;
	TimerStart();
}

void TimeoutClass::TimerStart(void)
{
	IsTimeout = false;
	TimerCount = 0;
	IsTimerStarted = true;
}

void TimeoutClass::TimerStop(void)
{
	IsTimerStarted = false;
}

boolean TimeoutClass::GetIsTimeout(void)
{
	return IsTimeout;
}

void TimeoutClass::SetIsTimeout(boolean flag)
{
	IsTimeout = flag;
}

void TimeoutClass::CheckIsTimeout(void)
{
	if(IsTimerStarted)
	{
		if(TimerCount++ >= TimerValue)
		{
			IsTimeout = true;
			TimerStop();
		}
	}
}


