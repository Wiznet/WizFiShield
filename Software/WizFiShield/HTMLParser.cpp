#include "HTMLParser.h"
#include <avr/pgmspace.h>

#ifdef CODEMEM
prog_char Get_Msg[] PROGMEM = "GET";   // "String 0" etc are strings to store - change to suit.
prog_char URI_Msg[] PROGMEM = "/?command";   // "String 0" etc are strings to store - change to suit.
prog_char Forward_CMD_Msg[] PROGMEM = "FW_CMD";   // "String 0" etc are strings to store - change to suit.
prog_char Backward_CMD_Msg[] PROGMEM = "BW_CMD";   // "String 0" etc are strings to store - change to suit.
prog_char Right_CMD_Msg[] PROGMEM = "RIGHT_CMD";   // "String 0" etc are strings to store - change to suit.
prog_char Left_CMD_Msg[] PROGMEM = "LEFT_CMD";


PROGMEM const char *CMDMsg_table[] = 
{
	Get_Msg,
	URI_Msg,
	Forward_CMD_Msg,
	Backward_CMD_Msg,
	Right_CMD_Msg,
	Left_CMD_Msg
};

#endif
//////////////////////////
// Constructor
HTMLParser::HTMLParser()
{
	bInParsing = NO_PARSING; 
	
	IsFirstCRLF = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//Parse HTML message transferred by parameter, buf. It returns 1 when it encounters to the EOF of HTML file, ohterwise 0
uint8_t HTMLParser::Parsing(char * buf)
{
	uint8_t i, retval, index = 0;
	char msgbuf[10];
	char *tmpbuf, *tmpbuf2;


	if(bInParsing == NO_PARSING)
	{

		memset(parserBuf, 0, PARSEBUF_SIZE); 
		parserBufIndex = 0;
	}
		
	while(buf[index] != '\0')
	{
		if(buf[index] == 0x0a)
		{
			if(parserBuf[parserBufIndex - 1] == 0x0d)
			{
				if(parserBufIndex > 1)
				{					
					/////////////////////////
					// Do Parsing a sentence terminated by CR and LF
					Serial.println((char *)parserBuf);

					tmpbuf = strtok((char *)parserBuf, " ");

					Serial.println((char *)tmpbuf);
					
					memset(msgbuf, 0, 10);
				       strcpy_P((char *)msgbuf, (char*)pgm_read_word(&(CMDMsg_table[0]))); // Necessary casts and dereferencing, just copy. 
				//	Serial.println((char *)msgbuf);
					
					if(!strcmp((const char*)tmpbuf, (const char*)msgbuf)) 	// GET
					{
						tmpbuf = strtok(NULL, " ");
						memset(msgbuf, 0, 10);
						Serial.println((char *)tmpbuf);
						
				       	strcpy_P((char *)msgbuf, (char*)pgm_read_word(&(CMDMsg_table[1]))); // Necessary casts and dereferencing, just copy. 
						if(strstr((char const*)tmpbuf, (const char *)msgbuf) != NULL)
						{
							if((tmpbuf2 = strchr((char const*)tmpbuf, '=')) != NULL)
							{
								memset(msgbuf, 0, 10);
				       			strcpy_P((char *)msgbuf, (char*)pgm_read_word(&(CMDMsg_table[2]))); // Necessary casts and dereferencing, just copy. 
								if(strstr((char const*)tmpbuf2, (const char *)msgbuf) != NULL){
									Serial.println((char *)msgbuf);
									SetParam(FW_CMD);
								}
								
								memset(msgbuf, 0, 10);
				       			strcpy_P((char *)msgbuf, (char*)pgm_read_word(&(CMDMsg_table[3]))); // Necessary casts and dereferencing, just copy. 
								if(strstr((char const*)tmpbuf2, (const char *)msgbuf) != NULL){
									SetParam(BW_CMD);
									Serial.println((char *)msgbuf);
								}

								memset(msgbuf, 0, 10);
				       			strcpy_P((char *)msgbuf, (char*)pgm_read_word(&(CMDMsg_table[4]))); // Necessary casts and dereferencing, just copy. 
								if(strstr((char const*)tmpbuf2, (const char *)msgbuf) != NULL){
									Serial.println((char *)msgbuf);
									SetParam(RT_CMD);
								}
								

								memset(msgbuf, 0, 10);
				       			strcpy_P((char *)msgbuf, (char*)pgm_read_word(&(CMDMsg_table[5]))); // Necessary casts and dereferencing, just copy. 
								if(strstr((char const*)tmpbuf2, (const char *)msgbuf) != NULL){
									SetParam(LF_CMD);
									Serial.println((char *)msgbuf);
								}
							}
								
						}

						tmpbuf = strtok(NULL, " ");
						Serial.println((char *)tmpbuf);
					}
					memset(parserBuf, 0, PARSEBUF_SIZE);
					parserBufIndex = 0;
				}else
				{
					bInParsing = NO_PARSING;
					return 1; // Parsing was finished.
				}
			}
			index++;
		}else 
		{			
			parserBuf[parserBufIndex++] = buf[index++];
		}
	}

	if(parserBufIndex > 0)
		bInParsing = IN_PARSING;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//If we received a pair of name, which specified in param_name, and value, this function stores value to param_value and returns 1. Otherwise, return 0.
uint8_t HTMLParser::GetParam(void)
{
	return (uint8_t)myParam.VALUE;
}

void HTMLParser::SetParam(PARAM_CMD cmd)
{
	myParam.VALUE = cmd;
}

uint8_t HTMLParser::Parsing_Get(char *buf)
{
	uint8_t i, index = 0;
	uint8_t tmpbuf[16], msgbuf[10];
	uint8_t retval = 0;

	memset(tmpbuf, 0, 16);
	
	for(i=0; i<6; i++)
		tmpbuf[i] = buf[i];

	Serial.println((char *)tmpbuf);
	
	memset(msgbuf, 0, 10);
       strcpy_P((char *)msgbuf, (char*)pgm_read_word(&(CMDMsg_table[0]))); // Necessary casts and dereferencing, just copy. 
//	Serial.println((char *)msgbuf);
	
	if(!strcmp((const char*)tmpbuf, (const char*)msgbuf))
	{
		memset(tmpbuf, 0, 16);
		index = 0;
		
		for(; buf[i] != '='; i++)
		{
			Serial.print((char)buf[i]);
			tmpbuf[index++] = buf[i];
		}
		i++;
		
		Serial.println((char *)tmpbuf);
	
		memset(msgbuf, 0, 10);
	       strcpy_P((char *)msgbuf, (char*)pgm_read_word(&(CMDMsg_table[1]))); // Necessary casts and dereferencing, just copy. 
		
		if(!strcmp((const char*)tmpbuf, (const char*)msgbuf))
		{
			memset(tmpbuf, 0, 16);
			index = 0;
			
			for(; buf[i] != ' '; i++)
			{
				Serial.print((char)buf[i]);
				tmpbuf[index++] = buf[i];
			}

			Serial.println((char *)tmpbuf);

			memset(msgbuf, 0, 10);
		       strcpy_P((char *)msgbuf, (char*)pgm_read_word(&(CMDMsg_table[2]))); // Necessary casts and dereferencing, just copy. 
			if(!strcmp((const char*)tmpbuf, (const char*)msgbuf))
			{
				myParam.VALUE = FW_CMD;
				Serial.println((char *)msgbuf);
				return 1;
			}

			memset(msgbuf, 0, 10);
		       strcpy_P((char *)msgbuf, (char*)pgm_read_word(&(CMDMsg_table[3]))); // Necessary casts and dereferencing, just copy. 
			if(!strcmp((const char*)tmpbuf, (const char*)msgbuf))
			{
				myParam.VALUE = BW_CMD;
				Serial.println((char *)msgbuf);
				return 1;
			}

			memset(msgbuf, 0, 10);
		       strcpy_P((char *)msgbuf, (char*)pgm_read_word(&(CMDMsg_table[4]))); // Necessary casts and dereferencing, just copy. 
			if(!strcmp((const char*)tmpbuf, (const char*)msgbuf))
			{
				myParam.VALUE = RT_CMD;
				Serial.println((char *)msgbuf);
				return 1;
			}

			memset(msgbuf, 0, 10);
		       strcpy_P((char *)msgbuf, (char*)pgm_read_word(&(CMDMsg_table[5]))); // Necessary casts and dereferencing, just copy. 
			if(!strcmp((const char*)tmpbuf, (const char*)msgbuf))
			{
				myParam.VALUE = LF_CMD;
				Serial.println((char *)msgbuf);
				return 1;
			}

			myParam.VALUE = NO_CMD;
		}
	}else
		return 0;
}

