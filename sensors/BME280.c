#include "BME280.h"

bmx280_t* bmx280_dev;

bmx280_params_t BMX280Params(i2c_t i2c_dev, uint8_t i2c_addr) {
    bmx280_params_t params;
    params = (bmx280_params_t) {
        .i2c_dev = i2c_dev,
        .i2c_addr = i2c_addr,
        .t_sb = BMX280_SB_1000,
        .filter = BMX280_FILTER_OFF,
        .run_mode = BMX280_MODE_NORMAL,
        .temp_oversample = BMX280_OSRS_X1,
        .press_oversample = BMX280_OSRS_X1,
        .humid_oversample = BMX280_OSRS_X1
    };
    return params;
}

bool BME280Init(i2c_t i2c_dev, uint8_t i2c_addr) {
    bmx280_dev = malloc(sizeof(bmx280_t));
    bmx280_params_t paramValues = BMX280Params(i2c_dev, i2c_addr);
    return bmx280_init(bmx280_dev, &paramValues) == BMX280_OK;
}

float readTemperature(void) {
    return (float)bmx280_read_temperature(bmx280_dev) / 100.0f;
}

float readPressure(void) {
    return (float)bmx280_read_pressure(bmx280_dev)/1000.0f;
}

float readHumidity(void) {
    return (float)bme280_read_humidity(bmx280_dev)/100.0f;
}