#include "libmaple.h"
#include "wirish.h"
#include "lumen/lumen.h"
#include "libraries/WiShield/WiShield.h"

/* Begin gank from asynclabs  */
#define WIRELESS_MODE_INFRA    1
#define WIRELESS_MODE_ADHOC    2

/* Wireless configuration parameters */
unsigned char local_ip[]    = {192, 168, 1,   7};           // IP address of WiShield
unsigned char gateway_ip[]  = {192, 168, 1,   1};           // router or gateway IP address
unsigned char subnet_mask[] = {255, 255, 255, 0};         // subnet mask for the local network
const char ssid[]           = {"ether"};               // max 32 bytes

unsigned char security_type = 0;                       // 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

/* WPA/WPA2 passphrase */
const char security_passphrase[] = {"12345678"};       // max 64 characters

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

volatile int watchdog_count;
/* in ms  */
#define WATCHDOG_TIME       30

void setup()
{
   int i;
   float h, s, v;
   float r, g, b;

   h = 0.0;
   s = 1.0;
   v = 1.0;

   pinMode(10, OUTPUT);
   digitalWrite(10, HIGH);

   Serial2.begin(115200);

   pinMode(3, OUTPUT);
   digitalWrite(3, HIGH);
   digitalWrite(3, LOW);

   pinMode(RED_PIN, PWM);
   pinMode(GREEN_PIN, PWM);
   pinMode(BLUE_PIN, PWM);

   lumen_set_rgb(1.0, 1.0, 1.0);
   while (millis() < 3000) {
      HSVtoRGB(&r, &g, &b, h, 1.0, 1.0);
      lumen_set_rgb(r, g, b);

      h += 0.01;
      if (h >= 360) {
         h = 0;
      }
   }

   WiFi.init();
}

uint32 lastMillis;

void loop()
{
//   if (watchdog_count > WATCHDOG_TIME) {
//      WiFi.init();
//   }
//
//   /* increment every 1000 ms  */
//   if ((millis() - lastMillis) > 10000) {
//      lastMillis = millis();
//      watchdog_count++;
//   }

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
