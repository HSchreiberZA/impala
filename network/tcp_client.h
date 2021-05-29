#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <stdio.h>
#include <errno.h>
#include "thread.h"
#include "net/af.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/tcp.h"
#include "net/gnrc/tcp/config.h"
#include "net/gcoap.h"

#include "debug.h"

/* Number of possible parallel connections */
#ifndef CONNS
#define CONNS (1)
#endif

/* Amount of data to transmit */
#ifndef NBYTE
#define NBYTE (2048)
#endif

/* Test pattern used by client application */
#ifndef TEST_PATERN_CLI
#define TEST_PATERN_CLI (0xF0)
#endif

/* Test pattern used by server application */
#ifndef TEST_PATERN_SRV
#define TEST_PATERN_SRV (0xA7)
#endif

extern uint8_t bufs[CONNS][NBYTE];
extern uint8_t stacks[CONNS][THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF];

void *cli_thread(void *arg);

int coapTest (void);

#endif