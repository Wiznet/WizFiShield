#include "HTMLPages.h"
#include <avr/pgmspace.h>

#ifdef CODEMEM
prog_char Protocol_Msg[] PROGMEM = "HTTP/1.1 200 OK\r\n";   // "String 0" etc are strings to store - change to suit.
prog_char Page1_Line0[] PROGMEM = "<!DOCTYPE html PUBLIC \"-//WAPFORUM//DTD XHTML Mobile 1.2//EN\" ";   // "String 0" etc are strings to store - change to suit.
prog_char Page1_Line1[] PROGMEM = "\"http://www.wapforum.org/DTD/xhtml-mobile 12.dtd\">\r\n";   // "String 0" etc are strings to store - change to suit.
prog_char Html_start[] PROGMEM = "<html>\r\n";   // "String 0" etc are strings to store - change to suit.
prog_char head_start[] PROGMEM = "<head>\r\n";   // "String 0" etc are strings to store - change to suit.
prog_char Page1_Line2[] PROGMEM = "<meta http-equiv=\"Content-Type\" content=\"text/html/>\r\n";
prog_char Content_Msg[] PROGMEM = "Content-Type: text/html\r\n";
prog_char Page1_Line3[] PROGMEM = "<meta name=\"viewport\" content=\"user-scalable=no, initial-scale=5.0,";
prog_char Page1_Line4[] PROGMEM = " maximum-scale=5.0, minimum-scale=5.0, width=device-width\"/>\r\n";
prog_char head_end[] PROGMEM = "</head>\r\n";   // "String 0" etc are strings to store - change to suit.
prog_char Page1_Line5[] PROGMEM = "<body>\r\n";
prog_char new_Line[] PROGMEM = "\r\n";
prog_char Page1_Line6[] PROGMEM = "<h1 align=\"center\">Select </h1>\r\n";
prog_char Page1_Line7[] PROGMEM = "<form>\r\n";
prog_char Page1_Line8[] PROGMEM = "<table width=\"100%\"><tr><td colspan=3 align=\"center\"><h1>Press button you want</h1></td></tr><tr>";
prog_char Page1_Line8_cont[] PROGMEM = "<td width=\"40%\"></td><td width=\"20%\" bgcolor=\"silver\" align=\"center\"><h1><button ";
prog_char Page1_Line8_cont2[] PROGMEM = "name=\"command\" value=\"FW_CMD\">forward</button></h1></td><td width=\"40%\"></td></tr>\r\n";
prog_char Middle_Line[] PROGMEM = "<tr><td width= \"40%\" align=\"right\"><h1><button name=\"command\" value=\"LEFT_CMD\">LEFT</button></h1></td>";
prog_char Middle_Line_cont[] PROGMEM = "<td width=\"20%\"></td><td width= \"40%\" align=\"left\"><h1><button name=\"command\" value=\"RIGHT_CMD\">";
prog_char Middle_Line_cont2[] PROGMEM = "RIGHT</button></h1></td></tr>\r\n";
prog_char Page1_Line9[] PROGMEM = "<tr><td width=\"40%\"></td><td width= \"20%\" bgcolor=\"#c3c3c3\" align=\"center\"><h1><button name=\"command\"  ";
prog_char Page1_Line9_cont[] PROGMEM = "value=\"BW_CMD\">backward</button></h1></td><td width=\"40%\"></td></tr></table>\r\n";
prog_char Page1_Line10[] PROGMEM = "</form>\r\n";
prog_char Page1_Line11[] PROGMEM = "</body>\r\n";
prog_char Page1_Line12[] PROGMEM = "<table width=\"100%\"><tr><td colspan=3 align=\"center\"><h1>Press button you want. Battery is low</h1></td></tr><tr>";
prog_char Page1_Line13[] PROGMEM = "<table width=\"100%\"><tr><td colspan=3 align=\"center\"><h1>Alert! Battery is too low. Recharge!</h1></td></tr></table>\r\n";
prog_char Html_end[] PROGMEM = "</html>\r\n";   // "String 0" etc are strings to store - change to suit.


PROGMEM const char *HTML_table[] = 
{
	Protocol_Msg,
	Content_Msg,
	new_Line,				// 0
	Page1_Line7,			// 6
	Page1_Line8,				// 7
	Page1_Line8_cont,
	Page1_Line8_cont2,
	Middle_Line,
	Middle_Line_cont,
	Middle_Line_cont2,
	Page1_Line9,				// 8
	Page1_Line9_cont,				// 8
	Page1_Line10	// 9
};

PROGMEM const char *HTML_table2[] = 
{
	Protocol_Msg,
	Content_Msg,
	new_Line,				// 0
	Page1_Line7,			// 6
	Page1_Line12,				// 7
	Page1_Line8_cont,
	Page1_Line8_cont2,
	Middle_Line,
	Middle_Line_cont,
	Middle_Line_cont2,
	Page1_Line9,				// 8
	Page1_Line9_cont,				// 8
	Page1_Line10	// 9
};

PROGMEM const char *HTML_table3[] = 
{
	Protocol_Msg,
	Content_Msg,
	new_Line,				// 0
	Page1_Line7,			// 6
	Page1_Line13,				// 7
	Page1_Line10	// 9
};

#endif

HTMLPages::HTMLPages() //constructor
{
	CurrentIndex = 0;
	CurrentBatteryStatus = 1; //NORMAL_BATT : 1, WARNING_BATT : 2, ALERT_BATT : 3
	
}

uint8_t HTMLPages::GetNextLine(char *buf) //if no data, return 0. Otherwise, return 1
{

	if(CurrentBatteryStatus == 1){
		if(CurrentIndex == 13)
		{
			CurrentIndex = 0;
			return 0;
		}
		else
		{
			memset(buf, 0, 128);
		       strcpy_P((char *)buf, (char*)pgm_read_word(&(HTML_table[CurrentIndex++]))); // Necessary casts and dereferencing, just copy. 
		       return 1;
		}
	}else if(CurrentBatteryStatus == 2)
	{
		if(CurrentIndex == 13)
		{
			CurrentIndex = 0;
			return 0;
		}
		else
		{
			memset(buf, 0, 128);
		       strcpy_P((char *)buf, (char*)pgm_read_word(&(HTML_table2[CurrentIndex++]))); // Necessary casts and dereferencing, just copy. 
		       return 1;
		}
	}else if(CurrentBatteryStatus == 3)
	{
		if(CurrentIndex == 6)
		{
			CurrentIndex = 0;
			return 0;
		}
		else
		{
			memset(buf, 0, 128);
		       strcpy_P((char *)buf, (char*)pgm_read_word(&(HTML_table3[CurrentIndex++]))); // Necessary casts and dereferencing, just copy. 
		       return 1;
		}
	}
}

void HTMLPages::SetCurrentBatteryStatus(uint8_t status)
{
	CurrentBatteryStatus = status;
}

