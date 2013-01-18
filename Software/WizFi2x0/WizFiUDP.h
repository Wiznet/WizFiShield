#ifndef _WIZFIUDP_H
#define _WIZFIUDP_H

#include <WizFi2x0.h>
#include <Config.h>

extern WizFi2x0Class myWizFi;

class WizFiUDP {
	
public:
	WizFiUDP(void);
	WizFiUDP(uint8_t *ip, uint16_t port, uint16_t src_port);//, WizFi2x0Class WizFi);
	WizFiUDP(const char* domain, uint16_t port, uint16_t src_port);//, WizFi2x0Class WizFi);

	uint8_t open(void);
	uint8_t close(void);
	void write(byte value);
	void write(byte *buf);
	void write(byte *buf, size_t size);

	uint8_t read(void);
	uint8_t read(byte *buf);
	uint8_t read(byte* buf, size_t size);

	void GetCIDstr(byte* str);
	uint8_t GetCID(void);

	void SetIsConnected(boolean flag);

	boolean available();

	void GetCurrentDestInfo(byte *ipaddr, uint16_t *portnum);
	void SetCurrentDestInfo(byte *ipaddr, uint16_t portnum);


private:
	uint8_t *SrcIP;
	uint16_t SrcPort;

	const char *DstDomain;
	uint8_t DstIP[4];
	uint16_t DstPort;

	uint8_t CID;
	uint8_t Mode;

	boolean bOpen;
	boolean bDNSQuery;

	WizFi2x0Class* WizFi;
	
};


#endif
