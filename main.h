#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif
#define UIP_BUFSIZE     400
#define UIP_CONNS       10
#include "libraries/WiShield/uip-conf.h"
#include "libraries/WiShield/uipopt.h"
#include "libraries/WiShield/psock.h"
// uip tracks state so you have to define a container for your state
typedef struct hello_state {
  struct psock p;
  char inputbuffer[10];
  char name[40];
} uip_tcp_appstate_t;
void hello_appcall(void);
// define name of callback on network activity
#ifndef UIP_APPCALL
#define UIP_APPCALL hello_appcall
#endif /* UIP_APPCALL */

#include "libraries/WiShield/uip.h"

#ifdef __cplusplus
}
#endif
#endif
