#include "SPS30.h"

sps30_t* sps30_dev;

bool init_sps30(void) {
    sps30_dev = malloc(sizeof(sps30_t));
    sps30_params_t paramValues = SPS30_PARAMS;
    return sps30_init(sps30_dev, &paramValues) == SPS30_OK;
}

float read_sps30(void) {
    sps30_data_t* data = malloc(sizeof(sps30_data_t));
    sps30_read_measurement(sps30_dev, data);
    //printf("SPS30 read success: %d\n", status == SPS30_OK);
    return data->ps;
}