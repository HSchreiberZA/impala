#ifndef BME280
#define BME280

#include <stdlib.h>
#include "periph/i2c.h"
#include <bmx280.h>
#include <bmx280_params.h>

int BME280Init(bmx280_t* dev, i2c_t i2c_dev, uint8_t i2c_addr);
float readTemperature(bmx280_t* dev);
float readPressure(bmx280_t* dev);
float readHumidity(bmx280_t* dev);

#endif