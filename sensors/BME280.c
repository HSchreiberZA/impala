#include "BME280.h"

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

int BME280Init(bmx280_t* dev, i2c_t i2c_dev, uint8_t i2c_addr) {
    bmx280_params_t paramValues = BMX280Params(i2c_dev, i2c_addr);
    return bmx280_init(dev, &paramValues);
}

float readTemperature(bmx280_t* dev) {
    return (float)bmx280_read_temperature(dev) / 100.0f;
}

float readPressure(bmx280_t* dev) {
    return (float)bmx280_read_pressure(dev)/1000.0f;
}

float readHumidity(bmx280_t* dev) {
    return (float)bme280_read_humidity(dev)/100.0f;
}