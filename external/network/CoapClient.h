#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <stdio.h>
#include "net/af.h"
#include "net/gcoap.h"
#include "od.h"

int perform_get (void);
int perform_put (char* payload);

#endif