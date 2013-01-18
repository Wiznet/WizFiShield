#ifndef _WIZFISERVER_H
#define _WIZFISERVER_H

#include <WizFi2x0.h>
#include <Config.h>

extern WizFi2x0Class myWizFi;

class WizFiServer {
public:
	WizFiServer(uint16_t port);
	uint8_t begin();
//	boolean IsConnected();

//	void write(byte value);
//	void write(byte *buf);
//	void write(byte *buf, size_t size);

//	uint8_t read(void);
//	uint8_t read(byte *buf);
//	uint8_t read(byte* buf, size_t size);

	void GetCIDstr(byte * strCID);
	uint8_t GetCID(void);

//	void SetIsConnected(boolean flag);
	
private:
	uint16_t src_port;

	uint8_t CID;
	boolean bOpen;
	
	WizFi2x0Class* WizFi;
};

#endif
