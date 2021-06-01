#include "CoapClient.h"

/* Retain request path to re-request if response includes block. User must not
 * start a new request (with a new path) until any blockwise transfer
 * completes or times out. */
#define _LAST_REQ_PATH_MAX (64)
static char _last_req_path[_LAST_REQ_PATH_MAX];

/*
 * Response callback.
 */
void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t* pdu,
                          const sock_udp_ep_t *remote) {

    (void)remote;       /* not interested in the source currently */

    puts("Message received");

    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("gcoap: timeout for msg ID %02u\n", coap_get_id(pdu));
        return;
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        printf("gcoap: error in response\n");
        return;
    }

    coap_block1_t block;
    if (coap_get_block2(pdu, &block) && block.blknum == 0) {
        puts("--- blockwise start ---");
    }

    char *class_str = (coap_get_code_class(pdu) == COAP_CLASS_SUCCESS)
                            ? "Success" : "Error";
    printf("gcoap: response %s, code %1u.%02u", class_str,
                                                coap_get_code_class(pdu),
                                                coap_get_code_detail(pdu));
    if (pdu->payload_len) {
        unsigned content_type = coap_get_content_type(pdu);
        if (content_type == COAP_FORMAT_TEXT
                || content_type == COAP_FORMAT_LINK
                || coap_get_code_class(pdu) == COAP_CLASS_CLIENT_FAILURE
                || coap_get_code_class(pdu) == COAP_CLASS_SERVER_FAILURE) {
            /* Expecting diagnostic payload in failure cases */
            printf(", %u bytes\n%.*s\n", pdu->payload_len, pdu->payload_len,
                                                          (char *)pdu->payload);
        }
        else {
            printf(", %u bytes\n", pdu->payload_len);
            printf("%s\n", pdu->payload);
            //od_hex_dump(pdu->payload, pdu->payload_len, OD_WIDTH_DEFAULT);
        }
    }
    else {
        printf(", empty payload\n");
    }

    /* ask for next block if present */
    if (coap_get_block2(pdu, &block)) {
        if (block.more) {
            unsigned msg_type = coap_get_type(pdu);
            if (block.blknum == 0 && !strlen(_last_req_path)) {
                puts("Path too long; can't complete blockwise");
                return;
            }
            gcoap_req_init(pdu, (uint8_t *)pdu->hdr, CONFIG_GCOAP_PDU_BUF_SIZE,
                               COAP_METHOD_GET, _last_req_path);

            if (msg_type == COAP_TYPE_ACK) {
                coap_hdr_set_type(pdu->hdr, COAP_TYPE_CON);
            }
            block.blknum++;
            coap_opt_add_block2_control(pdu, &block);
            int len = coap_opt_finish(pdu, COAP_OPT_FINISH_NONE);
            gcoap_req_send((uint8_t *)pdu->hdr, len, remote,
                           _resp_handler, memo->context);
        }
        else {
            puts("--- blockwise complete ---");
        }
    }
}

int coapGETTest (void) {
    coap_pkt_t* pdu = malloc(sizeof(coap_pkt_t));
    uint8_t* buf = malloc(sizeof(uint8_t) * 1024);

    sock_udp_ep_t* remote = malloc(sizeof(sock_udp_ep_t));

    uint8_t addr[] = {0x2a,0x04,0x45,0x40,0x66,0x0a,0xd3,0x00,0xc1,0x59,0x8b,0xb1,0x1b,0x69,0xfb,0xfc};

    remote->family = AF_INET6;
    remote->netif = SOCK_ADDR_ANY_NETIF;
    remote->port = 5683;

    memcpy(remote->addr.ipv6, addr, sizeof(addr));

    gcoap_req_init(pdu, buf, 1024, COAP_METHOD_GET, "/time");
    coap_hdr_set_type(pdu->hdr, COAP_TYPE_CON);
    ssize_t pduSize = coap_opt_finish(pdu, COAP_OPT_FINISH_NONE);

    //ssize_t pduSize = gcoap_request(pdu, buf, 1024, COAP_METHOD_GET, "/time");

    printf("size: %d\n", gcoap_req_send(buf, pduSize, remote, _resp_handler, NULL));

    return 1;
}

int coapPutTest (char* payload) {
    coap_pkt_t* pdu = malloc(sizeof(coap_pkt_t));
    uint8_t* buf = malloc(sizeof(uint8_t) * 1024);

    pdu->payload = malloc(sizeof(char) * 2);

    sock_udp_ep_t* remote = malloc(sizeof(sock_udp_ep_t));

    uint8_t addr[] = {0x2a,0x04,0x45,0x40,0x66,0x0a,0xd3,0x00,0xc1,0x59,0x8b,0xb1,0x1b,0x69,0xfb,0xfc};

    remote->family = AF_INET6;
    remote->netif = SOCK_ADDR_ANY_NETIF;
    remote->port = 5683;

    memcpy(remote->addr.ipv6, addr, sizeof(addr));

    gcoap_req_init(pdu, buf, 1024, COAP_METHOD_PUT, "/reading");
    coap_hdr_set_type(pdu->hdr, COAP_TYPE_CON);
    ssize_t pduSize = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);
    ssize_t payloadSize = coap_payload_put_bytes(pdu, payload, strlen(payload));

    printf("size: %d\n", gcoap_req_send(buf, pduSize + payloadSize, remote, _resp_handler, NULL));

    return 1;
}