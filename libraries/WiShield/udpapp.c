
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

#include "libmaple.h"
#include "gpio.h"
#include "uip.h"
#include <string.h>
#include "udpapp.h"
#include "config.h"
#include "lights.h"

#include <lo/lo.h>

#define STATE_INIT              0
#define STATE_LISTENING         1
#define STATE_QUIT              2

static struct udpapp_state s;

void dummy_app_appcall(void)
{
}

void udpapp_init(void)
{
   uip_ipaddr_t addr;
   struct uip_udp_conn *c;

   uip_ipaddr(&addr, 192, 168, 1, 116);
   c = uip_udp_new(&addr, HTONS(12345));
   if(c != NULL) {
      uip_udp_bind(c, HTONS(12344));
   }

   s.state = STATE_INIT;

   PT_INIT(&s.pt);
}

typedef struct {
   const char *path;
   float r;
   float g;
   float b;
} LibloPacket __attribute__ ((packed));

static unsigned char parse_msg(void)
{
    int i;
    LibloPacket *p;
    int bytes_available = uip_datalen();
    int argc;
    lo_arg **argv;

    unsigned char* pData = uip_appdata;
    p = uip_appdata;

    int result = 0;
    lo_message message = lo_message_deserialise(uip_appdata, bytes_available, &result);
    if (!message) {
        return 1;
    }

    lights_set_rgb(lo_message_get_argv(message)[0]->f,
                   lo_message_get_argv(message)[1]->f,
                   lo_message_get_argv(message)[2]->f);

    lo_message_free(message);

    return 1;
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
