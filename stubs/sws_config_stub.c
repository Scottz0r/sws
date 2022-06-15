/**
 * File for stubbing out functions for testing.
 */
#include "sws_config.h"

#include <stdio.h>

static void flush_in()
{
    int c;
    do
    {
        c = fgetc(stdin);
    } while (c != '\n' && c != EOF);
}

// sws_config.h
int SWS_parse_config(const char* filename, SWS_schedule_t* values, int values_size)
{
    int day, start_time, end_time;
    printf("[STUB] Config values. Blank line to stop.\n");

    int count = 0;
    int index = 0;
    char buffer[100];

    do
    {
        printf("[STUB] Config value (d hhmm hhmm) > ");
        fflush(stdout);

        fgets(buffer, sizeof(buffer), stdin);
        count = sscanf(buffer, "%d %d %d", &day, &start_time, &end_time);

        if (count == 3)
        {
            // One value case.
            values[index].day = day;
            values[index].start_time = start_time;
            values[index].end_time = end_time;
            ++index;
        }
    } while (count == 3 && index < values_size);

    return index;
}

