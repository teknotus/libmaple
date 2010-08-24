
/******************************************************************************

Filename:		udpapp.h
Description:	UDP app for the WiShield 1.0

 ******************************************************************************

 TCP/IP stack and driver for the WiShield 1.0 wireless devices

 Copyright(c) 2009 Async Labs Inc. All rights reserved.

 This program is free software; you can redistribute it and/or modify it
 under the terms of version 2 of the GNU General Public License as
 published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 more details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 59
 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 Contact Information:
 <asynclabs@asynclabs.com>

 Author               Date        Comment
 ---------------------------------------------------------------
 AsyncLabs			07/11/2009	Initial version

 *****************************************************************************/

#include "uip.h"
#include <string.h>
#include "udpapp.h"
#include "config.h"

#include <lo/lo.h>

#define STATE_INIT				0
#define STATE_LISTENING         1
#define STATE_QUIT	2

static struct udpapp_state s;

void dummy_app_appcall(void)
{
}

void udpapp_init(void)
{
   uip_ipaddr_t addr;
   struct uip_udp_conn *c;

   uip_ipaddr(&addr, 192,168,1,125);
   c = uip_udp_new(&addr, HTONS(12345));
   if(c != NULL) {
      uip_udp_bind(c, HTONS(12344));
   }

   s.state = STATE_INIT;

   PT_INIT(&s.pt);
}

static void set_light_rgb(unsigned char r, unsigned char g, unsigned char b) {
    // TODO
}

static unsigned char parse_msg(void)
{
    int i;
    int bytes_available = uip_datalen();

    // TODO Parse OSC message

    unsigned char* pData = uip_appdata;

    int result = 0;
    lo_message message = lo_message_deserialise(uip_appdata, bytes_available, &result);


    /* unsigned char checksum = 0; */
    /* for (i = 0; i < 4; ++i) { */
    /*     checksum += pData[i]; */
    /* } */
    /* if (checksum != pData[4]) { */
    /*     // Checksum doesn't match */
    /* } */

    /* char r, g, b; */
    /* switch(pData[0]) { */
    /*     case 'S': */
    /*         r = pData[1]; */
    /*         g = pData[2]; */
    /*         b = pData[3]; */
    /*         set_light_rgb(r,g,b); */
    /*         break; */
    /*     case 'Q': */
    /*         // quit */
    /*         break; */
    /* } */

    return 0;
}

static PT_THREAD(handle_connection(void))
{
   PT_BEGIN(&s.pt);

   s.state = STATE_LISTENING;

   do {
      PT_WAIT_UNTIL(&s.pt, uip_newdata());

      if(uip_newdata() && parse_msg()) {

         uip_flags &= (~UIP_NEWDATA);
         break;
      }
   } while(s.state != STATE_QUIT);

   s.state = STATE_INIT;

   PT_END(&s.pt);
}

void udpapp_appcall(void)
{
   handle_connection();
}
