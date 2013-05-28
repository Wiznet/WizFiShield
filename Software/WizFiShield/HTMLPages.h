#ifndef _HTMLPAGES_H_
#define _HTMLPAGES_H_

#include <WizFi2x0.h>
#include <Config.h>

extern WizFi2x0Class myWizFi;

class HTMLPages{
public:
	HTMLPages(); //constructor
	uint8_t GetNextLine(char *buf); //if no data, return 0. Otherwise, return 1
	void SetCurrentBatteryStatus(uint8_t status);
private:
	uint8_t CurrentIndex;
	uint8_t CurrentBatteryStatus;
};
#endif
