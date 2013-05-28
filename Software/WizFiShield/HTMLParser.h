#ifndef _HTMLPARSER_H_
#define _HTMLPARSER_H_

#include <WizFi2x0.h>
#include <Config.h>

extern WizFi2x0Class myWizFi;

#define MAX_PARAM_COUNT	1
#define PARSEBUF_SIZE		150

enum PARSESTATE{
	NO_PARSING = 0,
	IN_PARSING = 1
};

enum PARAM_CMD{
	NO_CMD = 0,
	FW_CMD = 1,
	BW_CMD = 2,
	RT_CMD = 3,
	LF_CMD = 4
};

typedef struct PARAM_STRUCT_TAG{
	char NAME[16];
	PARAM_CMD VALUE;
}PARAM_STRUCT;

class HTMLParser {
public:
	HTMLParser();

	uint8_t Parsing(char * buf); //Parse HTML message transferred by parameter, buf. It returns 1 when it encounters to the EOF of HTML file, ohterwise 0
	uint8_t GetParam(void); //If we received a pair of name, which specified in param_name, and value, this function stores value to param_value and returns 1. Otherwise, return 0.  
	void SetParam(PARAM_CMD cmd);
	
	
private:
	//////////////////////////////////////////////////////////
	//As WizFi2i0 receive up to 128byte and HTMLParser process HTML message as sentence by sentence terminated with CR and LF. 
	//We have to retain some sentence until we get its following CR and LF
	//Following variables are for processing this.
	
	byte parserBuf[PARSEBUF_SIZE]; // a buffer for storing temporary HTML message
	PARSESTATE bInParsing; // a flag variable indicating whether HTML message is terminated with CR and LF or not.
	PARAM_STRUCT myParam;
	uint8_t myParamCount;

	bool IsFirstCRLF;
	uint8_t parserBufIndex;

	uint8_t Parsing_Get(char *buf);
};

#endif
