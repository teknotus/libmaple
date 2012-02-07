#include "main.h"
#include "libmaple.h"
#include "wirish.h"
#include "libraries/WiShield/WiShield.h"
#include "libraries/WiShield/psock.h"
#include <string.h>


/* Begin gank from asynclabs  */
#define WIRELESS_MODE_INFRA    1
#define WIRELESS_MODE_ADHOC    2

/* Wireless configuration parameters */
// unsigned char local_ip[]    = {10, 230, 168, 246};         // IP address of WiShield
// unsigned char gateway_ip[]  = {10, 128, 128, 128};         // router or gateway IP address
// unsigned char subnet_mask[] = {255, 0, 0, 0};         // subnet mask for the local network
// const char    ssid[]        = {"545meraki"};     // max 32 bytes

unsigned char local_ip[]    = {192, 168, 2,   2};         // IP address of WiShield
unsigned char gateway_ip[]  = {192, 168, 2,   1};         // router or gateway IP address
unsigned char subnet_mask[] = {255, 255, 255, 0};         // subnet mask for the local network
const char    ssid[]        = {"linksys"};     // max 32 bytes


unsigned char security_type = 0;                           // 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

/* WPA/WPA2 passphrase */
const char security_passphrase[] = {""};           // max 64 characters

/* WEP 128-bit keys */
const char wep_keys[] = {
   0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, // Key 0
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Key 1
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Key 2
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // Key 3
};

// setup the wireless mode
// infrastructure - connect to AP
// adhoc - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;
/* End gank  */
extern "C" {
const unsigned char hello_str[] = {"hello world!\n"};

static int handle_connection(struct hello_state *s)
{
 PSOCK_BEGIN(&s->p);
 PSOCK_SEND(&s->p, hello_str, 13);
 PSOCK_CLOSE(&s->p);
 PSOCK_END(&s->p);
}

void hello_appcall(void)
{
 struct hello_state *s = &(uip_conn->appstate);
 if(uip_connected()) {
  PSOCK_INIT(&s->p, (unsigned char*)s->inputbuffer, sizeof(s->inputbuffer));
 }
 handle_connection(s);
}
}// extern "C"

void setup()
{
   Serial2.begin(115200);
   WiFi.init();
   uip_listen(HTONS(1000));
}

void loop()
{
   WiFi.run();
}

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated object that need libmaple may fail.
__attribute__(( constructor )) void premain() {
   init();
}

int main(void)
{
   setup();

   while (1) {
      loop();
   }
   return 0;
}
