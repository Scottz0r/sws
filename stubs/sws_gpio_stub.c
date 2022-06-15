#include "sws_gpio.h"

#include <stdio.h>

static void flush_in()
{
    int c;
    do
    {
        c = fgetc(stdin);
    } while (c != '\n' && c != EOF);
}


SWS_rc_t SWS_gpio_init()
{
    printf("[STUB] GPIO Init (1/0) > ");
    int val;
    int count = scanf("%d", &val);
    flush_in();

    if (count == 1)
    {
        if (val)
        {
            return SWS_OK;
        }
    }
    else
    {
        printf("[STUB] Bad input\n");
    }

    return SWS_ERROR;
}

SWS_gpio_state_t SWS_gpio_get()
{
    printf("[STUB] GPIO Get > ");

    int val;
    int count = scanf("%d", &val);
    flush_in();

    if (count == 1)
    {
        return val;
    }

    printf("[STUB] Bad input\n");
    return 0;
}

void SWS_gpio_set(SWS_gpio_state_t value)
{
    printf("Stub GPIO set (value = %d)\n", value);
}
