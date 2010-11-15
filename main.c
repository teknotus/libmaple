// lwip main.cpp

#include "wirish.h"
#include "libmaple.h"
#include "spi.h"
#include "gpio.h"

#include "port/witypes.h"
#include "port/g2100.h"
#include "port/g2100if.h"

#include "lwip/init.h"
#include "lwip/debug.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/sys.h"
#include "lwip/stats.h"

#include "lwip/ip_addr.h"
#include "lwip/ip.h"
#include "lwip/ip_frag.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"

#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2

#define CLOCKTICKS_PER_MS 1

// Wireless configuration parameters ----------------------------------------
//unsigned char local_ip[] = {192,168,0,103};	// IP address of WiShield
//unsigned char gateway_ip[] = {192,168,0,1};	// router or gateway IP address
//unsigned char subnet_mask[] = {255,255,255,0};	// subnet mask for the local network
const char ssid[] = {"Forest"};		// max 32 bytes
unsigned char security_type = 0;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// setup the wireless mode
// infrastructure - connect to AP
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;

// lwip
struct netif netif;

// (manual) host IP configuration
static ip_addr ipaddr, netmask, gw;



//---------------------------------------------------------------------------

void setup() {
     // Serial2.begin(115200);
     // WiFi.init();
	 /* startup defaults (may be overridden by one or more opts) */
	 IP4_ADDR(&gw, 192,168,0,1);
	 IP4_ADDR(&ipaddr, 192,168,0,2);
	 IP4_ADDR(&netmask, 255,255,255,0);
}

// This is the webpage that is served up by the webserver
const char webpage[]= {"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789</body></html>"};


byte buf[] = "Hello world!";

void main_init() {
	   zg_init();
	   lwip_init();

//	   netif_add(&netif, &ipaddr, &netmask, &gw, NULL, g2100if_init, g2100if_input);
	   netif_set_default(&netif);
	   netif_set_up(&netif);
/*
	   while(zg_get_conn_state() != 1) {
	      zg_drv_process();
	   }
*/
}

void loop() {

	   unsigned long last_time, last_arp_time;
	   last_time = 0;
	   last_arp_time = 0;

	   /*
	   struct pbuf *p;

	   if(poll_driver(netif) == PACKET_READY) {
	     p = get_packet(netif);
	     ip_input(p, netif);
	   }

	   */

	   if (millis() - last_arp_time >= ARP_TMR_INTERVAL * CLOCKTICKS_PER_MS) {
	       etharp_tmr();
	       last_arp_time = millis();
	   }
	   if(millis() - last_time >= TCP_TMR_INTERVAL * CLOCKTICKS_PER_MS) {
	     tcp_tmr();
	     last_time = millis();
	   }

//	   zg_drv_process();
}

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated object that need libmaple may fail.
__attribute__(( constructor )) void premain() {
   main_init();
}


int main(void) {
   setup();

   while (1) {
      loop();
   }
   return 0;
}

