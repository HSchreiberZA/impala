#include "Device.h"

char* device_info_as_partial_json (void) {
    char* buf = malloc(sizeof(char)*1024);
    sprintf(buf, "\"device\": {\"node\": \"1\"}");
    return buf;
}