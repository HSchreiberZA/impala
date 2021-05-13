APPLICATION = impala
BOARD = esp32-ttgo-t-beam
RIOTBASE ?= $(CURDIR)/../RIOT/

# a minimal application Makefile
# BME280 connected via I2C
USEMODULE += bme280_i2c
USEMODULE += esp32_ttgo_t_beam_v1_0
# Modules to include:
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += esp_wifi
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6_default
USEMODULE += gnrc_icmpv6_echo
USEMODULE += gnrc_lorawan
USEMODULE += gnrc_pktdump
USEMODULE += gnrc_neterr

DEVELHELP ?= 1

USEMODULE += sensors
EXTERNAL_MODULE_DIRS += $(CURDIR)/sensors

USEMODULE += utils
EXTERNAL_MODULE_DIRS += $(CURDIR)/utils

USEMODULE += network
EXTERNAL_MODULE_DIRS += $(CURDIR)/network

LORA_DRIVER ?= sx1276
LORA_REGION ?= EU868

#USEPKG += semtech-loramac
USEMODULE += $(LORA_DRIVER)

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += fmt

LOG_LEVEL = LOG_DEBUG

# When using I2C, specify the default I2C device to use,
# and the BME280's address (see datasheet).
# The values below are the defaults:
CFLAGS += -DBMX280_PARAM_I2C_DEV=I2C_DEV\(0\)
CFLAGS += -DBMX280_PARAM_I2C_ADDR=0x76

CFLAGS += -DCONFIG_LORAMAC_DEFAULT_JOIN_PROCEDURE_OTAA
CFLAGS += -DCONFIG_LORAMAC_REGION_EU_868
CFLAGS += -DCONFIG_LORAMAC_DEFAULT_TX_MODE_UNCNF
CFLAGS += -DCONFIG_LORAMAC_DEFAULT_RX2_DR_3

include secrets/Makefile
include $(RIOTBASE)/Makefile.include