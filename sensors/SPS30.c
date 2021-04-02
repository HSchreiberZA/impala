#include "SPS30.h"

int SPS30Init(sps30_t* dev) {
    sps30_params_t paramValues = SPS30_PARAMS;
    return sps30_init(dev, &paramValues);
}