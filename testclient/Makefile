# name of your application
APPLICATION = espnow_test_client

# Any esp8266 and esp32 board should work here
BOARD ?= wittycloud

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

USEMODULE += gnrc_netdev_default auto_init_gnrc_netif
USEMODULE += gnrc_ipv6_nib_6ln
USEMODULE += gnrc_icmpv6_echo
USEMODULE += gnrc_udp gnrc_sock_udp
USEMODULE += gnrc_rpl auto_init_gnrc_rpl
USEMODULE += gnrc_pktdump
USEMODULE += gcoap

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
CFLAGS += -DDEBUG_ASSERT_VERBOSE

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
