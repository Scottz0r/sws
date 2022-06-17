#include "sws_time.h"

#include <time.h>

SWS_rc_t SWS_get_now(int* tm_day, int* tm_hhmm)
{
    time_t t = time(NULL);
    struct tm timeinfo;

    localtime_r(&t, &timeinfo);

    *tm_day = timeinfo.tm_wday;
    *tm_hhmm = (timeinfo.tm_hour * 100) + timeinfo.tm_min;

    return SWS_OK;
}
