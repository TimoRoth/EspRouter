# name of your application
APPLICATION = esp32_border_router

# This program is specifically for the esp32-olimex-gateway
BOARD := esp32-olimex-evb
USEMODULE += olimex_esp32_gateway
GNRC_NETIF_NUMOF := 2

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# Router modules and settings
USEMODULE += gnrc_netdev_default auto_init_gnrc_netif
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_rpl auto_init_gnrc_rpl
USEMODULE += fib netopt
USEMODULE += gnrc_icmpv6_echo

# ESP NOW
CFLAGS += -DESP_NOW_SOFT_AP_PASS=\"ThisistheRIOTporttoESP\" -DESP_NOW_CHANNEL=6
USEMODULE += esp_now

# Shell
USEMODULE += ps shell shell_commands

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1
CFLAGS += -DDEBUG_ASSERT_VERBOSE

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Prefix has to be /64
IPV6_PREFIX ?= fd19:ffff::
# Outer address of the router and the length of its prefix
IPV6_AUTO ?= 0
IPV6_ADDR ?= fd19:aaaa::1
IPV6_ADDR_LEN ?= 64
IPV6_DEF_RT ?= fd19:aaaa::2
# Pass as CFLAGS to program
CFLAGS += -DBR_IPV6_PREFIX=\"$(IPV6_PREFIX)\"
ifeq (0,$(IPV6_AUTO))
  CFLAGS += -DBR_IPV6_ADDR=\"$(IPV6_ADDR)\" -DBR_IPV6_ADDR_LEN=$(IPV6_ADDR_LEN) -DBR_IPV6_DEF_RT=\"$(IPV6_DEF_RT)\"
endif

# Might need more than the default of 2 addresses.
CFLAGS += -DGNRC_NETIF_IPV6_ADDRS_NUMOF=4

include $(RIOTBASE)/Makefile.include
