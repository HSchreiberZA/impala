#ifndef SPS30
#define SPS30

#include <stdlib.h>
#include "periph/i2c.h"
#include <sps30.h>
#include "sps30_params.h"

int init_sps30(void);
int read_sps30(sps30_data_t *data);

#endif