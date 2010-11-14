#ifndef __G2100IF_H__
#define __G2100IF_H__


#include "lwip/err.h"
#include "lwip/netif.h"

err_t g2100if_init(struct netif *netif);
err_t g2100if_input(struct pbuf *p, struct netif *netif);
struct netif *g2100if_register(void);
int g2100if_poll(void);
void Set_MAC_Address(unsigned char* macadd);

#ifdef SERVER

#define MAC_ADDR0 0x00
#define MAC_ADDR1 0x00
#define MAC_ADDR2 0x00
#define MAC_ADDR3 0x00
#define MAC_ADDR4 0x00
#define MAC_ADDR5 0x01

#else

#define MAC_ADDR0 0x00
#define MAC_ADDR1 0x00
#define MAC_ADDR2 0x00
#define MAC_ADDR3 0x00
#define MAC_ADDR4 0x00
//#define MAC_ADDR5 0x02
#define MAC_ADDR5 0x03
//#define MAC_ADDR5 0x04

#endif

#endif 
