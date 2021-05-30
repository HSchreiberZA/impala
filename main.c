#include <stdio.h>
#include "periph/uart.h"
#include "periph/i2c.h"
#include "sensors/GPSUtils.h"
#include "sensors/BME280.h"
#include "thread.h"
#include "msg.h"
#include "shell.h"
#include "xtimer.h"
#include "sensors/SPS30.h"
#include "network/CoapClient.h"
#include "esp_wifi.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void gps_callback (void *arg, uint8_t data) {
    char character = (char)data;
    receiveGPSChar(character);
}

int main(void)
{
    puts("Hello World!");

    board_init();

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    //xtimer_sleep(20);
    
    esp_wifi_connect();

    //coapPutTest();

    i2c_init(BMX280_PARAM_I2C_DEV);
    
    int initSPS30Value = init_sps30();
    printf("SPS30 init value: %d\n", initSPS30Value);
    sps30_data_t *data = malloc(sizeof(sps30_data_t));
    int readSPS30Value = read_sps30(data);
    printf("SPS30 read value: %d\n", readSPS30Value);
    printf("SPS30 read value ps: %f\n", data->ps);
    
    bmx280_t* BMX280Dev = malloc(sizeof(bmx280_t));
    int initBMX280Value = BME280Init(BMX280Dev, BMX280_PARAM_I2C_DEV, BMX280_PARAM_I2C_ADDR);
    printf("BMX280 init value: %d\n", initBMX280Value);
    
    //printf("GPS init returned %d\n", enabled);

    if (initBMX280Value == BMX280_OK) {
        printf("Temperature reading %f°C\n", readTemperature(BMX280Dev));
        printf("Pressure reading %fkPa\n", readPressure(BMX280Dev));
        printf("Humidity reading %f%%\n", readHumidity(BMX280Dev));
    }
    
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);
   
    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    while (true)
    {
        /* code */
    }
    

    return 0;
}
