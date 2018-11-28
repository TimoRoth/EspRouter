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
USEMODULE += gnrc_ipv6_nib_6lbr gnrc_ipv6_router_default
USEMODULE += fib
USEMODULE += gnrc_icmpv6_echo

EXTERNAL_MODULE_DIRS += $(CURDIR)/uhcpd
USEMODULE += uhcpd

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

IPV6_PREFIX ?= fd19:f3f4:cd51:3af7::
IPV6_PREFIX_LEN ?= 64
CFLAGS += -DBR_IPV6_PREFIX=\"$(IPV6_PREFIX)\" -DBR_IPV6_PREFIX_LEN=$(IPV6_PREFIX_LEN)

include $(RIOTBASE)/Makefile.include
