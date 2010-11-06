# Standard things
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
BUILDDIRS       += $(BUILD_PATH)/$(d)
BUILDDIRS       += $(BUILD_PATH)/$(d)/src
BUILDDIRS       += $(BUILD_PATH)/$(d)/src/api
BUILDDIRS       += $(BUILD_PATH)/$(d)/src/core
BUILDDIRS       += $(BUILD_PATH)/$(d)/src/core/ipv4
BUILDDIRS       += $(BUILD_PATH)/$(d)/port
BUILDDIRS       += $(BUILD_PATH)/$(d)/port/arch

# Local flags
LWIP_INCLUDES := -I$(d)/src/include -I$(d)/src/include/ipv4 -I$(d)/port -I$(d)/port/arch 

CFLAGS_$(d) := $(WIRISH_INCLUDES) $(LIBMAPLE_INCLUDES) $(LWIP_INCLUDES)

# Local rules and targets
cSRCS_$(d) := src/api/netdb.c                       \
              src/api/netifapi.c                    \
              src/api/api_msg.c                     \
              src/api/api_lib.c                     \
              src/api/err.c                         \
              src/api/netbuf.c                      \
              src/api/sockets.c                     \
              src/api/tcpip.c                       \
              src/core/sys.c                        \
              src/core/stats.c                      \
              src/core/netif.c                      \
              src/core/tcp.c                        \
              src/core/udp.c                        \
              src/core/tcp_out.c                    \
              src/core/dhcp.c                       \
              src/core/raw.c                        \
              src/core/tcp_in.c                     \
              src/core/dns.c                        \
              src/core/pbuf.c                       \
              src/core/ipv4/ip_addr.c               \
              src/core/ipv4/icmp.c                  \
              src/core/ipv4/ip_frag.c               \
              src/core/ipv4/autoip.c                \
              src/core/ipv4/inet_chksum.c           \
              src/core/ipv4/ip.c                    \
              src/core/ipv4/igmp.c                  \
              src/core/ipv4/inet.c                  \
              src/core/mem.c                        \
              src/core/init.c                       \
              port/g2100.c                          \
              port/g2100if.c                        \
              port/httpd.c

cppSRCS_$(d) :=
 
cFILES_$(d) := $(cSRCS_$(d):%=$(d)/%)
cppFILES_$(d) := $(cppSRCS_$(d):%=$(d)/%)

OBJS_$(d)     := $(cFILES_$(d):%.c=$(BUILD_PATH)/%.o) \
                 $(cppFILES_$(d):%.cpp=$(BUILD_PATH)/%.o)
DEPS_$(d)     := $(OBJS_$(d):%.o=%.d)

$(OBJS_$(d)): TGT_CFLAGS := $(CFLAGS_$(d))

TGT_BIN += $(OBJS_$(d))

# Standard things
-include        $(DEPS_$(d))
d               := $(dirstack_$(sp))
sp              := $(basename $(sp))
