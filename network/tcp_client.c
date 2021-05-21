#include "tcp_client.h"

uint8_t bufs[CONNS][NBYTE];
uint8_t stacks[CONNS][THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF];

void *cli_thread(void *arg)
{
    /* Test program variables */
    int tid = (int) arg;

    /* Transmission control block */
    gnrc_tcp_tcb_t tcb;

    /* Target peer address information */
    //ipv6_addr_t* target_addr;
    uint16_t target_port;

    /* Initialize target information */
    //ipv6_addr_from_str(target_addr, TARGET_ADDR);
    target_port = TARGET_PORT;

    /* Initialize TCB */
    gnrc_tcp_tcb_init(&tcb);
    gnrc_tcp_ep_t remote;
    gnrc_tcp_ep_from_str(&remote, TARGET_ADDR);

    int status = gnrc_tcp_open_active(&tcb, &remote, target_port);
    do {
        switch (status) {
            case 0:
                DEBUG("TID=%d : gnrc_tcp_open_active() : 0 : ok\n", tid);
                break;

            case -EISCONN:
                printf("TID=%d : gnrc_tcp_open_active() : -EISCONN\n", tid);
                return 0;

            case -EINVAL:
                printf("TID=%d : gnrc_tcp_open_active() : -EINVAL\n", tid);
                return 0;

            case -EAFNOSUPPORT:
                printf("TID=%d : gnrc_tcp_open_active() : -EAFNOSUPPORT\n", tid);
                return 0;

            case -EADDRINUSE:
                printf("TID=%d : gnrc_tcp_open_active() : -EADDRINUSE\n", tid);
                return 0;

            case -ECONNREFUSED:
                printf("TID=%d : gnrc_tcp_open_active() : -ECONNREFUSED : retry after 10sec\n",
                       tid);
                xtimer_sleep(10);
                continue;

            case -ENOMEM:
                printf("TID=%d : gnrc_tcp_open_active() : -ENOMEM\n", tid);
                return 0;

            case -ETIMEDOUT:
                printf("TID=%d : gnrc_tcp_open_active() : -ETIMEDOUT : retry after 10sec\n", tid);
                xtimer_sleep(10);
                continue;

            default:
                printf("TID=%d : gnrc_tcp_open_active() : %d\n", tid, status);
                return 0;
        }
        status = gnrc_tcp_open_active(&tcb, &remote, target_port);
    } while (status != 0);

    printf("client thread terminating: TID=%d\n", tid);
    return 0;
}

