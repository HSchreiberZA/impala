#ifndef LORAWAN
#define LORAWAN

#include "net/loramac.h"     /* core loramac definitions */
#include "semtech_loramac.h" /* package API */
#include "sx127x.h"
#include "sx127x_params.h"
#include "sx127x_netdev.h"
#include "log.h"

#include <string.h>
#include <stdio.h>

int lorawan (void);

#endif