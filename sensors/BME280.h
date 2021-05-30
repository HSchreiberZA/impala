#ifndef BME280
#define BME280

#include <stdlib.h>
#include "periph/i2c.h"
#include <bmx280.h>
#include <bmx280_params.h>

bool BME280Init(i2c_t i2c_dev, uint8_t i2c_addr);
float readTemperature(void);
float readPressure(void);
float readHumidity(void);

#endif