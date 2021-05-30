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

bool SPS30_INIT = false;
bool BMX280_INIT = false;
bool GPS_INIT = false;

void gps_callback (void *arg, uint8_t data) {
    char character = (char)data;
    receiveGPSChar(character);
}

void init_hardware (void) {
    i2c_init(BMX280_PARAM_I2C_DEV);
    SPS30_INIT = init_sps30();
    BMX280_INIT = BME280Init(BMX280_PARAM_I2C_DEV, BMX280_PARAM_I2C_ADDR);
    
    if (ENABLE_GPS) {
        GPS_INIT = uart_init(UART_DEV(1), 9600, gps_callback, NULL) == 0;
    }

    printf("SPS30 %s\n", SPS30_INIT ? "true" : "false");
    printf("BMX280 %s\n", BMX280_INIT ? "true" : "false");
    printf("GPS %s\n", GPS_INIT ? "true" : "false");
}

int main(void)
{
    puts("Hello World!");

    board_init();
    init_hardware();
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    //xtimer_sleep(20);
    
    esp_wifi_connect();

    //coapPutTest();
    
    if (SPS30_INIT) {
        float readSPS30Value = read_sps30();
        printf("SPS30 read value: %f\n", readSPS30Value);
    }
    
    if (BMX280_INIT) {
        printf("Temperature reading %f°C\n", readTemperature());
        printf("Pressure reading %fkPa\n", readPressure());
        printf("Humidity reading %f%%\n", readHumidity());
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
