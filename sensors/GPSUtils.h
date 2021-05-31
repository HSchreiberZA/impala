#ifndef GPSUTILS
#define GPSUTILS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "debug.h"
#include "../utils/minmea.h"

void gps_callback (void *arg, uint8_t data);
char* gps_as_partial_json(void);

#endif