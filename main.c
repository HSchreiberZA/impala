#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "periph/uart.h"
#include "periph/i2c.h"
#include "sensors/GPSUtils.h"
#include "sensors/BME280.h"
#include "thread.h"
#include "msg.h"
#include "shell.h"
#include "xtimer.h"
#include "sensors/SPS30.h"
#include "sensors/Device.h"
#include "network/CoapClient.h"
#include "esp_wifi.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
char thread_stack[THREAD_STACKSIZE_MAIN];

bool SPS30_INIT = false;
bool BMX280_INIT = false;
bool GPS_INIT = false;

void init_hardware (void) {
    i2c_init(BMX280_PARAM_I2C_DEV);
    SPS30_INIT = init_sps30();
    BMX280_INIT = BME280Init(BMX280_PARAM_I2C_DEV, BMX280_PARAM_I2C_ADDR);
    
    if (ENABLE_GPS) {
        GPS_INIT = uart_init(UART_DEV(1), 9600, gps_callback, NULL) == 0;
    }

    printf("SPS30 %s\n", SPS30_INIT ? "enabled" : "disabled");
    printf("BMX280 %s\n", BMX280_INIT ? "enabled" : "disabled");
    printf("GPS %s\n", GPS_INIT ? "enabled" : "disabled");
}

int get_all_sensor_readings_as_json(int argc, char **argv) {
    if (SPS30_INIT && BMX280_INIT && GPS_INIT) {
        printf("{%s,%s,%s,%s}", device_info_as_partial_json(), particulate_as_partial_json(), environ_as_partial_json(), gps_as_partial_json());
    }
    return 0;
}

int send_to_server(int argc, char **argv) {
    char* buf = malloc(sizeof(char) * 2048);
    if (SPS30_INIT && BMX280_INIT && GPS_INIT) { 
        sprintf(buf, "{%s,%s,%s,%s}", device_info_as_partial_json(), particulate_as_partial_json(), environ_as_partial_json(), gps_as_partial_json());
        coapPutTest(buf);
    }
    return 0;
}

void *rcv_thread(void *arg) {
    char* buf = malloc(sizeof(char) * 1024);
    xtimer_ticks32_t last_wakeup = xtimer_now();
    uint32_t period = 300000000;

    while(true) {
        if (SPS30_INIT && BMX280_INIT && GPS_INIT) {
            char* device_info = device_info_as_partial_json();
            char* particulate_info = particulate_as_partial_json();
            char* environ_info = environ_as_partial_json();
            char* gps_info = gps_as_partial_json();
            sprintf(buf, "{%s,%s,%s,%s}", device_info, particulate_info, environ_info, gps_info);
            coapPutTest(buf);
            free(device_info);
            free(particulate_info);
            free(environ_info);
            free(gps_info);
        }
        xtimer_periodic_wakeup(&last_wakeup, period);
    }
    return 0;
}

int get_from_server(int argc, char **argv) {
    coapGETTest();
    return 0;
}

int get_all_sensor_readings(int argc, char **argv) {
    if (SPS30_INIT) {
        float readSPS30Value = read_sps30();
        printf("SPS30 read value: %f\n", readSPS30Value);
    }
    
    if (BMX280_INIT) {
        printf("Temperature reading %f°C\n", readTemperature());
        printf("Pressure reading %fkPa\n", readPressure());
        printf("Humidity reading %f%%\n", readHumidity());
    }
    return 0;
}

int start_thread(int argc, char **argv) {
    char *end;
    const long i = strtol(argv[1], &end, 10);
    printf("%ld\n", i);
    thread_wakeup(i);
    return 1;
}

static const shell_command_t shell_commands[] = {
    { "readings", "Get all sensor readings", get_all_sensor_readings },
    { "readingsJS", "Get all sensor readings in json format", get_all_sensor_readings_as_json },
    { "sendReadings", "Send all readings to coap server", send_to_server },
    { "get", "perform a get request coap server", get_from_server },
    { "startThread", "starts the messenger thread", start_thread },

    { NULL, NULL, NULL }
};

int main(void)
{
    board_init();
    init_hardware();
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    
    esp_err_t res = esp_wifi_connect();

    kernel_pid_t messenger =  thread_create(thread_stack, sizeof(thread_stack),
                  THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_SLEEPING ,
                  rcv_thread, NULL, "send_to_server");
    
    if (res == ESP_OK) {
        xtimer_t* timer = malloc(sizeof(xtimer_t));
        xtimer_set_wakeup64(timer, 500, messenger);
    }

    printf("messenger Thread id: %d\n", messenger);

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
   
    return 0;
}
