#include "GPSUtils.h"

char COMPLETE_MESSAGE[MINMEA_MAX_LENGTH];
char PARTIAL_MESSAGE[MINMEA_MAX_LENGTH];

void parseGPSMessage (char* line) {
    switch (minmea_sentence_id(line, false)) {
        case MINMEA_SENTENCE_RMC: {
            struct minmea_sentence_rmc frame;
            if (minmea_parse_rmc(&frame, line)) {
                printf("$RMC: raw coordinates and speed: (%d/%d,%d/%d) %d/%d\n",
                        frame.latitude.value, frame.latitude.scale,
                        frame.longitude.value, frame.longitude.scale,
                        frame.speed.value, frame.speed.scale);
                printf("$RMC fixed-point coordinates and speed scaled to three decimal places: (%d,%d) %d\n",
                        minmea_rescale(&frame.latitude, 1000),
                        minmea_rescale(&frame.longitude, 1000),
                        minmea_rescale(&frame.speed, 1000));
                printf("$RMC floating point degree coordinates and speed: (%f,%f) %f\n",
                        minmea_tocoord(&frame.latitude),
                        minmea_tocoord(&frame.longitude),
                        minmea_todouble(&frame.speed));
            }
        } break;

        case MINMEA_SENTENCE_GGA: {
            struct minmea_sentence_gga frame;
            if (minmea_parse_gga(&frame, line)) {
                printf("$GGA: fix quality: %d\n", frame.fix_quality);
            }
        } break;

        case MINMEA_SENTENCE_GSV: {
            struct minmea_sentence_gsv frame;
            if (minmea_parse_gsv(&frame, line)) {
                printf("$GSV: message %d of %d\n", frame.msg_nr, frame.total_msgs);
                printf("$GSV: sattelites in view: %d\n", frame.total_sats);
                for (int i = 0; i < 4; i++)
                    printf("$GSV: sat nr %d, elevation: %d, azimuth: %d, snr: %d dbm\n",
                        frame.sats[i].nr,
                        frame.sats[i].elevation,
                        frame.sats[i].azimuth,
                        frame.sats[i].snr);
            }
        } break;

        case MINMEA_SENTENCE_GLL: {
            struct minmea_sentence_gll frame;
            if (minmea_parse_gll(&frame, line)) {
                printf("$GLL: mode %c\n", frame.mode);
                printf("$GLL: status %c\n", frame.status);
                printf("$GLL: latitude %f\n", minmea_tocoord(&frame.latitude));
                printf("$GLL: longitude %f\n", minmea_tocoord(&frame.longitude));
            }
        } break;
        default:
        break;
    }
}


void receiveGPSChar (char c) {

    size_t len = strlen(PARTIAL_MESSAGE);
    snprintf(PARTIAL_MESSAGE + len, sizeof PARTIAL_MESSAGE - len, "%c", c);

    if (c == '\n') {
        strcpy(COMPLETE_MESSAGE, PARTIAL_MESSAGE);
        strcpy(PARTIAL_MESSAGE, "");
        parseGPSMessage(COMPLETE_MESSAGE);
    }
}

void gps_callback (void *arg, uint8_t data) {
    char character = (char)data;
    receiveGPSChar(character);
}