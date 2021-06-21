APPLICATION = impala
BOARD = esp32-ttgo-t-beam
RIOTBASE ?= $(CURDIR)/../RIOT/

# a minimal application Makefile
# BME280 connected via I2C
USEMODULE += periph_i2c
USEMODULE += bme280_i2c
USEMODULE += sps30
USEMODULE += esp32_ttgo_t_beam_v1_0
# Modules to include:
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

# networking
USEMODULE += esp_wifi
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6_default
USEMODULE += gnrc_icmpv6_error
USEMODULE += gnrc_ipv6
USEMODULE += gnrc_rpl
USEMODULE += gnrc_netif_events
USEMODULE += gnrc_icmpv6_echo
USEMODULE += gnrc_udp
USEMODULE += gnrc_sock_udp
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += gcoap

DEVELHELP ?= 1

USEMODULE += sensors
EXTERNAL_MODULE_DIRS += $(CURDIR)/sensors

USEMODULE += utils
EXTERNAL_MODULE_DIRS += $(CURDIR)/utils

USEMODULE += network
EXTERNAL_MODULE_DIRS += $(CURDIR)/network

# When using I2C, specify the default I2C device to use,
# and the BME280's address (see datasheet).
# The values below are the defaults:
CFLAGS += -DBMX280_PARAM_I2C_DEV=I2C_DEV\(0\)
CFLAGS += -DBMX280_PARAM_I2C_ADDR=0x76
CFLAGS += -DI2C0_SPEED=I2C_SPEED_LOW

CFLAGS += -DSOCK_HAS_IPV6
CFLAGS += -DENABLE_GPS=0
CFLAGS += -DNODE_ID=1


include secrets/Makefile
include $(RIOTBASE)/Makefile.include