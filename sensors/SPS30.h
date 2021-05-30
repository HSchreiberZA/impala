#ifndef SPS30
#define SPS30

#include <stdlib.h>
#include "periph/i2c.h"
#include <sps30.h>
#include "sps30_params.h"

bool init_sps30(void);
float read_sps30(void);

#endif