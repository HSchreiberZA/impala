#include <stdio.h>
#include "board.h"
#include "periph_conf.h"
#include "periph/uart.h"
#include "periph/i2c.h"
#include "sensors/GPSUtils.h"
#include "sensors/BME280.h"
//#include "network/lora.h"
#include "network/wifi.h"

#include "msg.h"
#include "shell.h"
#include "xtimer.h"

#include "net/gnrc/netapi.h"
#include "net/gnrc/netif.h"

#include "net/gnrc/pktbuf.h"
#include "net/gnrc/pktdump.h"
#include "net/gnrc/netreg.h"
#include "net/loramac.h"

#define LORAWAN_QUEUE_SIZE (4U)

void gps_callback (void *arg, uint8_t data) {
    char character = (char)data;
    receiveGPSChar(character);
}

static void _usage(void)
{
    puts("usage: send <if_pid> <payload> [port]");
}

int tx_cmd(int argc, char **argv)
{
    gnrc_pktsnip_t *pkt;
    uint8_t port = CONFIG_LORAMAC_DEFAULT_TX_PORT; /* Default: 2 */
    int interface;

    if (argc < 3) {
        _usage();
        return 1;
    }

    interface = atoi(argv[1]);
    /* handle optional parameters */
    if (argc > 3) {
        port = atoi(argv[3]);
        if (port == 0 || port >= 224) {
            printf("error: invalid port given '%d', "
                   "port can only be between 1 and 223\n", port);
            return 1;
        }
    }

    pkt = gnrc_pktbuf_add(NULL, argv[2], strlen(argv[2]), GNRC_NETTYPE_UNDEF);

    /* register for returned packet status */
    if (gnrc_neterr_reg(pkt) != 0) {
        puts("Can not register for error reporting");
        return 0;
    }

    gnrc_netapi_set(interface, NETOPT_LORAWAN_TX_PORT, 0, &port, sizeof(port));
    gnrc_netif_send(gnrc_netif_get_by_pid(interface), pkt);

    /* wait for packet status and check */
    msg_t msg;
    msg_receive(&msg);
    if ((msg.type != GNRC_NETERR_MSG_TYPE) ||
        (msg.content.value != GNRC_NETERR_SUCCESS)) {
        printf("Error sending packet: (status: %d\n)", (int) msg.content.value);
    }
    else {
        puts("Successfully sent packet");
    }

    return 0;
}

static const shell_command_t shell_commands[] = {
    { "send",       "Send LoRaWAN data",     tx_cmd },
    { NULL, NULL, NULL }
};

int main(void)
{
    board_init();

    //lorawan();
    puts("Initialization successful - starting the shell now");
    gnrc_netreg_entry_t dump = GNRC_NETREG_ENTRY_INIT_PID(CONFIG_LORAMAC_DEFAULT_TX_PORT,
                                                          gnrc_pktdump_pid);

    gnrc_netreg_register(GNRC_NETTYPE_LORAWAN, &dump);

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

/*
    int enabled = uart_init(UART_DEV(1), 9600, gps_callback, NULL);
    i2c_init(BMX280_PARAM_I2C_DEV);

    //int enabled = uart_init(UART_DEV(1), 9600, gps_callback, NULL);
    //i2c_init(BMX280_PARAM_I2C_DEV);

    //bmx280_t* dev = malloc(sizeof(bmx280_t*));
    //int initBMX280Value = BME280Init(dev, BMX280_PARAM_I2C_DEV, BMX280_PARAM_I2C_ADDR);

    puts("Hello World!");

    //printf("GPS init returned %d\n", enabled);
    //printf("BMX280 init value: %d\n", initBMX280Value);

    
    if (initBMX280Value == BMX280_OK) {
        printf("Temperature reading %f°C\n", readTemperature(dev));
        printf("Pressure reading %dPA\n", readPressure(dev));
        printf("Humidity reading %f\n", readHumidity(dev));
    }
    */
    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    while (true)
    {
        //printf("DHCPStatus %d\n", getDHCPStatus());
    }
    

    return 0;
}
