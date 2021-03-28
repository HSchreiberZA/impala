APPLICATION = impala
BOARD = esp32-ttgo-t-beam
RIOTBASE ?= $(CURDIR)/../RIOT/

# a minimal application Makefile
# BME280 connected via I2C
USEMODULE += bme280_i2c
USEMODULE += esp32_ttgo_t_beam_v1_0
USEMODULE += sensors
EXTERNAL_MODULE_DIRS += $(CURDIR)/sensors

USEMODULE += utils
EXTERNAL_MODULE_DIRS += $(CURDIR)/utils


# When using I2C, specify the default I2C device to use,
# and the BME280's address (see datasheet).
# The values below are the defaults:
CFLAGS += -DBMX280_PARAM_I2C_DEV=I2C_DEV\(0\)
CFLAGS += -DBMX280_PARAM_I2C_ADDR=0x77

include $(RIOTBASE)/Makefile.include