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

int coapTest (void) {
    coap_pkt_t* pdu = malloc(sizeof(coap_pkt_t));
    uint8_t* buf = malloc(sizeof(uint8_t) * 1024);

    sock_udp_ep_t* remote = malloc(sizeof(sock_udp_ep_t));

    uint8_t addr[] = {0x2a, 0x04, 0x45, 0x40, 0x66, 0x09, 0xa9, 0x00, 0xa4, 0xc5, 0x7f, 0xce, 0x6a, 0x63, 0xa6, 0xe1};

    remote->family = AF_INET6;
    remote->netif = SOCK_ADDR_ANY_NETIF;
    remote->port = 5683;

    memcpy(remote->addr.ipv6, addr, sizeof(addr));

    printf("size: %d\n", gcoap_request(pdu, buf, 1024, COAP_METHOD_GET, "/time"));

    printf("size: %d\n", gcoap_req_send(buf, 11, remote, NULL, NULL));

    return 1;
}

