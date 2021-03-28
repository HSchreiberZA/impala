#include "GPSUtils.h"

char COMPLETE_MESSAGE[MINMEA_MAX_LENGTH];
char PARTIAL_MESSAGE[MINMEA_MAX_LENGTH];

/**
 * Convert a raw coordinate to a floating point DD.DDD... value.
 * Returns NaN for "unknown" values.
 */
float tocoord(int value, int scale) {

    printf("value: %d, scale: %d\n", value, scale);
    if (scale == 0) {
        return 0;
    }

    printf("value: %d, scale: %d\n", value, scale);

    int degrees = value / (scale * 100);
    int minutes = value % (scale * 100);

    printf("degrees: %f, minutes: %f\n", (float)degrees, (float)minutes);

    return (float) degrees + (float) minutes / (60 * scale);

}

void something (char* line) {
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
                /*printf("$RMC floating point degree coordinates and speed: (%f,%f) %f\n",
                        minmea_tocoord(&frame.latitude),
                        minmea_tocoord(&frame.longitude),
                        minmea_tofloat(&frame.speed));*/
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
                //printf("tocoord: %f\n", minmea_tocoord(&frame.latitude));


                printf("$GLL: mode %c\n", frame.mode);
                printf("$GLL: status %c\n", frame.status);
                printf("$GLL: latitude %d\n", frame.latitude.value);
                printf("$GLL: longitude %d\n", frame.longitude.value);
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
        //printf("%s", COMPLETE_MESSAGE);
        something(COMPLETE_MESSAGE);
    }
}