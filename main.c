/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Hello World application
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include "board.h"
#include "periph_conf.h"
#include "periph/uart.h"
#include "periph/i2c.h"
#include "sensors/GPSUtils.h"
#include "sensors/BME280.h"

void gps_callback (void *arg, uint8_t data) {
    char character = (char)data;
    receiveGPSChar(character);
}

int main(void)
{
    board_init();

    int enabled = uart_init(UART_DEV(1), 9600, gps_callback, NULL);
    i2c_init(BMX280_PARAM_I2C_DEV);

    bmx280_t* dev = malloc(sizeof(bmx280_t*));
    int initBMX280Value = BME280Init(dev, BMX280_PARAM_I2C_DEV, BMX280_PARAM_I2C_ADDR);

    puts("Hello World!");

    printf("GPS init returned %d\n", enabled);
    printf("BMX280 init value: %d\n", initBMX280Value);

    if (initBMX280Value == BMX280_OK) {
        printf("Temperature reading %f°C\n", readTemperature(dev));
        printf("Pressure reading %dPA\n", readPressure(dev));
        printf("Humidity reading %f\n", readHumidity(dev));
    }

    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    while (true)
    {
        /* code */
    }
    

    return 0;
}
