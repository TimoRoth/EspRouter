# name of your application
APPLICATION = espnow_test_client

# Any esp8266 and esp32 board should work here
BOARD ?= wittycloud

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

USEMODULE += gnrc_netdev_default auto_init_gnrc_netif
USEMODULE += gnrc_sixlowpan_default
USEMODULE += gnrc_icmpv6_echo
USEMODULE += gnrc_udp gnrc_sock_udp
USEMODULE += gnrc_rpl auto_init_gnrc_rpl
USEMODULE += gnrc_pktdump
USEMODULE += gcoap
USEMODULE += gnrc_pktbuf_cmd

# ESP NOW
CFLAGS += -DESP_NOW_SOFT_AP_PASS=\"ThisistheRIOTporttoESP\" -DESP_NOW_CHANNEL=6
USEMODULE += esp_now

# Shell
USEMODULE += ps shell shell_commands

USEMODULE += xtimer

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1
CFLAGS += -DDEVELHELP -DDEBUG_ASSERT_VERBOSE

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Prefix is assumed to be /64
IPV6_PREFIX ?= fd19:ffff::
IPV6_DEFRTR ?= fd19:ffff::1
IPV6_AUTO ?= 0
ifeq (0,$(IPV6_AUTO))
  CFLAGS += -DBR_IPV6_PREFIX=\"$(IPV6_PREFIX)\" -DBR_IPV6_DEFRTR=\"$(IPV6_DEFRTR)\"
  # Required for non-link-local default route
  CFLAGS += -DGNRC_IPV6_NIB_CONF_ARSM=1
endif

include $(RIOTBASE)/Makefile.include
