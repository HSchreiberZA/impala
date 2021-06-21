#include "SPS30.h"

sps30_t* sps30_dev;
sps30_data_t* data;

bool sleep_sps30(void) {
    return sps30_sleep(sps30_dev) == SPS30_OK;
}

bool wake_sps30(void) {
    return sps30_wakeup(sps30_dev) == SPS30_OK;
}

bool start_sps30_measurement(void) {
    return sps30_start_measurement(sps30_dev) == SPS30_OK;
}

bool stop_sps30_measurement(void) {
    return sps30_stop_measurement(sps30_dev) == SPS30_OK;
}

bool init_sps30(void) {
    sps30_dev = malloc(sizeof(sps30_t));
    data = malloc(sizeof(sps30_data_t));
    sps30_params_t paramValues = SPS30_PARAMS;
    bool init = sps30_init(sps30_dev, &paramValues) == SPS30_OK;
    if (init) {
        return sleep_sps30();
    }
    return init;
}

bool start_measurement_sps30(void) {
    if (wake_sps30()) {
        puts("SPS30 woke OK. Starting measurement...");
        return start_sps30_measurement();
    } 
    return false;
}

bool stop_measurement_sps30(void) {
    if(stop_sps30_measurement()) {
        puts("SPS30 measurement stoppped. Going to sleep");
        return sleep_sps30();
    }
    return false;
}

float read_sps30(void) {
    int* error = malloc(sizeof(int));
    if(start_measurement_sps30()) {
        // wait for sensor to power up
        xtimer_sleep(30);
        while (!sps30_data_ready(sps30_dev, error)){
            puts("Waiting for SPS30 measurement data...");
        }
        puts("SPS30 data received");
        sps30_read_measurement(sps30_dev, data);
        stop_measurement_sps30();
        return data->ps;
    }
    return -1;
}

char* particulate_as_partial_json(void) {
    int* error = malloc(sizeof(int));
    if(start_measurement_sps30()) {
        // wait for sensor to power up
        xtimer_sleep(30);
        while (!sps30_data_ready(sps30_dev, error)){
            puts("Waiting for SPS30 measurement data...");
        }
        puts("SPS30 data received");
        sps30_read_measurement(sps30_dev, data);
        stop_measurement_sps30();
        char* buf = malloc(sizeof(char) * 1024);
        sprintf(buf, "\"sps30\":{\"mc_pm1\":{\"value\":%f,\"unit\":\"µg/m^3\"},\"mc_pm2_5\":{\"value\":%f,\"unit\":\"µg/m^3\"},\"mc_pm4\":{\"value\":%f,\"unit\":\"µg/m^3\"},\"mc_pm10\":{\"value\":%f,\"unit\":\"µg/m^3\"},\"nc_pm0_5\":{\"value\":%f,\"unit\":\"#/cm^3\"},\"nc_pm1\":{\"value\":%f,\"unit\":\"#/cm^3\"},\"nc_pm2_5\":{\"value\":%f,\"unit\":\"#/cm^3\"},\"nc_pm4\":{\"value\":%f,\"unit\":\"#/cm^3\"},\"nc_pm10\":{\"value\":%f,\"unit\":\"#/cm^3\"},\"ps\":{\"value\":%f,\"unit\":\"µm\"}}", 
                data->mc_pm1, data->mc_pm2_5, data->mc_pm4, data->mc_pm10, data->nc_pm0_5, data->nc_pm1, data->nc_pm2_5, data->nc_pm4, data->nc_pm10, data->ps);
        return buf;
    }
    return "\"sps30\":{}";
}