#include "GPSUtils.h"

char COMPLETE_MESSAGE[MINMEA_MAX_LENGTH];
char PARTIAL_MESSAGE[MINMEA_MAX_LENGTH];

double latitude;
double longitude;
bool valid;

void parseGPSMessage (char* line) {
    switch (minmea_sentence_id(line, false)) {
        case MINMEA_SENTENCE_RMC: {
            struct minmea_sentence_rmc frame;
            if (minmea_parse_rmc(&frame, line)) {
                DEBUG("$RMC: raw coordinates and speed: (%d/%d,%d/%d) %d/%d\n",
                        frame.latitude.value, frame.latitude.scale,
                        frame.longitude.value, frame.longitude.scale,
                        frame.speed.value, frame.speed.scale);
                DEBUG("$RMC fixed-point coordinates and speed scaled to three decimal places: (%d,%d) %d\n",
                        minmea_rescale(&frame.latitude, 1000),
                        minmea_rescale(&frame.longitude, 1000),
                        minmea_rescale(&frame.speed, 1000));
                DEBUG("$RMC floating point degree coordinates and speed: (%f,%f) %f\n",
                        minmea_tocoord(&frame.latitude),
                        minmea_tocoord(&frame.longitude),
                        minmea_todouble(&frame.speed));
            }
        } break;

        case MINMEA_SENTENCE_GGA: {
            struct minmea_sentence_gga frame;
            if (minmea_parse_gga(&frame, line)) {
                DEBUG("$GGA: fix quality: %d\n", frame.fix_quality);
            }
        } break;

        case MINMEA_SENTENCE_GSV: {
            struct minmea_sentence_gsv frame;
            if (minmea_parse_gsv(&frame, line)) {
                DEBUG("$GSV: message %d of %d\n", frame.msg_nr, frame.total_msgs);
                DEBUG("$GSV: sattelites in view: %d\n", frame.total_sats);
                for (int i = 0; i < 4; i++)
                    DEBUG("$GSV: sat nr %d, elevation: %d, azimuth: %d, snr: %d dbm\n",
                        frame.sats[i].nr,
                        frame.sats[i].elevation,
                        frame.sats[i].azimuth,
                        frame.sats[i].snr);
            }
        } break;

        case MINMEA_SENTENCE_GLL: {
            struct minmea_sentence_gll frame;
            if (minmea_parse_gll(&frame, line)) {
                DEBUG("$GLL: mode %c\n", frame.mode);
                DEBUG("$GLL: status %c\n", frame.status);
                DEBUG("$GLL: latitude %f\n", minmea_tocoord(&frame.latitude));
                DEBUG("$GLL: longitude %f\n", minmea_tocoord(&frame.longitude));
                latitude = minmea_tocoord(&frame.latitude);
                longitude = minmea_tocoord(&frame.longitude);
                valid = frame.status == 'A';
            }
        } break;
        default:
        break;
    }
}

char* gps_as_partial_json(void) {
    char* buf = malloc(sizeof(char) * 1024);
    if (valid) {
        sprintf(buf, "\"gps\":{\"latitude\":%f,\"longitude\":%f,\"valid\":%d}",
                latitude, longitude, valid);
    } else {
        sprintf(buf, "\"gps\":{\"latitude\":%f,\"longitude\":%f,\"valid\":%d}",
                -100.0f, -200.0f, valid);
    }

    return buf;
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