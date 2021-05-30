#include "SPS30.h"

sps30_t* sps30_dev;
sps30_data_t* data;

bool init_sps30(void) {
    sps30_dev = malloc(sizeof(sps30_t));
    data = malloc(sizeof(sps30_data_t));
    sps30_params_t paramValues = SPS30_PARAMS;
    return sps30_init(sps30_dev, &paramValues) == SPS30_OK;
}

float read_sps30(void) {
    sps30_read_measurement(sps30_dev, data);
    return data->ps;
}

char* particulate_as_partial_json(void) {
    sps30_read_measurement(sps30_dev, data);
    char* buf = malloc(sizeof(char) * 1024);
    sprintf(buf, "\"sps30\": { \
                        \"mc_pm1\":{\"value\":%f,\"unit\":\"µg/m^3\"},\
                        \"mc_pm2_5\":{\"value\":%f,\"unit\":\"µg/m^3\"},\
                        \"mc_pm4\":{\"value\":%f,\"unit\":\"µg/m^3\"},\
                        \"mc_pm10\":{\"value\":%f,\"unit\":\"µg/m^3\"},\
                        \"nc_pm0_5\":{\"value\":%f,\"unit\":\"#/cm^3\"},\
                        \"nc_pm1\":{\"value\":%f,\"unit\":\"#/cm^3\"},\
                        \"nc_pm2_5\":{\"value\":%f,\"unit\":\"#/cm^3\"},\
                        \"nc_pm4\":{\"value\":%f,\"unit\":\"#/cm^3\"},\
                        \"nc_pm10\":{\"value\":%f,\"unit\":\"#/cm^3\"},\
                        \"ps\":{\"value\":%f,\"unit\":\"µm\"}\
                        }", data->mc_pm1, data->mc_pm2_5, data->mc_pm4, data->mc_pm10, data->nc_pm0_5, data->nc_pm1, data->nc_pm2_5, data->nc_pm4, data->nc_pm10, data->ps);
    return buf;
}