#include "lora.h"

#define ENABLE_DEBUG

semtech_loramac_t loramac;
static sx127x_t sx127x;

int lorawan (void) {
//    semtech_loramac_t* loramac = malloc(sizeof(semtech_loramac_t*));  /* The loramac stack device descriptor */
    /* define the required keys for OTAA, e.g over-the-air activation (the
    null arrays need to be updated with valid LoRa values) */
    static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = { 0x00, 0x00, 0x00, 0x00, \
                                                        0x00, 0x00, 0x00, 0x00 };
    static const uint8_t appeui[LORAMAC_APPEUI_LEN] = { 0x00, 0x00, 0x00, 0x00, \
                                                        0x00, 0x00, 0x00, 0x00 };
    static const uint8_t appkey[LORAMAC_APPKEY_LEN] = { 0x00, 0x00, 0x00, 0x00, \
                                                        0x00, 0x00, 0x00, 0x00, \
                                                        0x00, 0x00, 0x00, 0x00, \
                                                        0x00, 0x00, 0x00, 0x00 };
    

    sx127x_setup(&sx127x, &sx127x_params[0], 0);
    loramac.netdev = (netdev_t *)&sx127x;
    loramac.netdev->driver = &sx127x_driver;
    LOG_DEBUG("loramac init returned: %d\n", semtech_loramac_init(&loramac));

    /* set LoRaWAN keys */
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    LOG_DEBUG("Starting join procedure\n");
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        LOG_ERROR("Join procedure failed\n");
        return 1;
    }
    LOG_DEBUG("Join procedure succeeded\n");
    return 0;
}

/*
static void _semtech_loramac_event_cb(netdev_t *dev, netdev_event_t event)
{
    netdev_lora_rx_info_t packet_info;

    msg_t msg;
    msg.content.ptr = dev;

    switch (event) {
        case NETDEV_EVENT_ISR:
            msg.type = MSG_TYPE_ISR;
            if (msg_send(&msg, semtech_loramac_pid) <= 0) {
                DEBUG("[semtech-loramac] possibly lost interrupt.\n");
            }
            break;

        case NETDEV_EVENT_TX_COMPLETE:
            sx127x_set_sleep((sx127x_t *)dev);
            semtech_loramac_radio_events.TxDone();
            DEBUG("[semtech-loramac] Transmission completed\n");
            break;

        case NETDEV_EVENT_TX_TIMEOUT:
            msg.type = MSG_TYPE_TX_TIMEOUT;
            if (msg_send(&msg, semtech_loramac_pid) <= 0) {
                DEBUG("[semtech-loramac] TX timeout, possibly lost interrupt.\n");
            }
            break;

        case NETDEV_EVENT_RX_COMPLETE: {
            size_t len;
            uint8_t radio_payload[SX127X_RX_BUFFER_SIZE];
            len = dev->driver->recv(dev, NULL, 0, 0);
            dev->driver->recv(dev, radio_payload, len, &packet_info);
            semtech_loramac_radio_events.RxDone(radio_payload,
                                                len, packet_info.rssi,
                                                packet_info.snr);
            break;
        }
        case NETDEV_EVENT_RX_TIMEOUT:
            msg.type = MSG_TYPE_RX_TIMEOUT;
            if (msg_send(&msg, semtech_loramac_pid) <= 0) {
                DEBUG("[semtech-loramac] RX timeout, possibly lost interrupt.\n");
            }
            break;

        case NETDEV_EVENT_CRC_ERROR:
            DEBUG("[semtech-loramac] RX CRC error\n");
            semtech_loramac_radio_events.RxError();
            break;

        case NETDEV_EVENT_FHSS_CHANGE_CHANNEL:
            DEBUG("[semtech-loramac] FHSS channel change\n");
            if (semtech_loramac_radio_events.FhssChangeChannel) {
                semtech_loramac_radio_events.FhssChangeChannel((
                    (sx127x_t *)dev)->_internal.last_channel);
            }
            break;

        case NETDEV_EVENT_CAD_DONE:
            DEBUG("[semtech-loramac] test: CAD done\n");
            if (semtech_loramac_radio_events.CadDone) {
                semtech_loramac_radio_events.CadDone((
                    (sx127x_t *)dev)->_internal.is_last_cad_success);
            }
            break;

        default:
            DEBUG("[semtech-loramac] unexpected netdev event received: %d\n",
                  event);
    }
}*/
