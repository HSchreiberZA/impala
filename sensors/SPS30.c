#include "SPS30.h"

sps30_t *SPS30Dev;

int init_sps30(void) {
    SPS30Dev = malloc(sizeof(sps30_t*));
    sps30_params_t paramValues = SPS30_PARAMS;
    return sps30_init(SPS30Dev, &paramValues);
}

int read_sps30(sps30_data_t *data) {
    return sps30_read_measurement(SPS30Dev, data);
}