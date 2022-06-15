#include "sws_logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TIMESTAMP_BUFFER_MIN 32


static void get_timestamp(char* buffer)
{
    buffer[0] = 0;

    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        return;
    }

    int millis = (int)(ts.tv_nsec / 1e6);
    if (millis > 999)
    {
        millis = 999;
    }

    struct tm timeinfo;

    if (!localtime_r(&ts.tv_sec, &timeinfo))
    {
        return;
    }

    // C99 format specifies
    strftime(buffer, 32, "%F %T", &timeinfo);

    // Add milliseconds (null will be at index 19)
    sprintf(buffer + 19, ".%03d", millis);
}

void SWS_log(const char* format, ...)
{
    char time_buffer[TIMESTAMP_BUFFER_MIN];
   
    get_timestamp(time_buffer);
    printf("%s: ", time_buffer);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}
