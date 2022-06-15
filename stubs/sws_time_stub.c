#include "sws_time.h"

#include <stdio.h>

static void flush_in()
{
    int c;
    do
    {
        c = fgetc(stdin);
    } while (c != '\n' && c != EOF);
}

SWS_rc_t SWS_get_now(int* day, int* time_hhmm)
{
    printf("[STUB] Now (d hhmm) >");

    int count = scanf("%d %d", day, time_hhmm);
    flush_in();

    if (count != 2)
    {
        printf("[STUB] Bad input\n");
        *day = -1;
        *time_hhmm = -1;
        return SWS_ERROR;
    }

    return SWS_OK;
}
