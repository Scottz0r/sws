#include <stdio.h>

#include "sws_types.h"
#include "sws_config.h"
#include "sws_time.h"
#include "sws_gpio.h"

#define SWS_EXIT_OK 0
#define SWS_EXIT_ERROR 1
#define SWS_EXIT_BAD_ARG 2
#define SWS_EXIT_BAD_CFG 3  

static SWS_rc_t process();
static SWS_rc_t is_within_schedule(int* is_within_schedule);

// Reserve enough space for a schedule entry for each hour every day of the week. This is an
// unlikely scenario because blocks should be used to span multiple hours.
#define SWS_MAX_SCHEDULE (24 * 7)
SWS_schedule_t s_schedules[SWS_MAX_SCHEDULE];
int s_count_schedules;

int main(int argc, const char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid arguments. Expected [config file]\n");
        return SWS_EXIT_BAD_ARG;
    }

    const char* cfg_path = argv[1];

    if (SWS_gpio_init() != SWS_OK)
    {
        fprintf(stderr, "Failed to initialize GPIO interface\n");
        return SWS_EXIT_ERROR;
    }

    s_count_schedules = SWS_parse_config(cfg_path, s_schedules, SWS_MAX_SCHEDULE);
    if (s_count_schedules < 0)
    {
        fprintf(stderr, "Failed to parse config\n");
        return SWS_EXIT_BAD_CFG;
    }
    else if(s_count_schedules == 0)
    {
        fprintf(stderr, "No values configured\n");
        return SWS_EXIT_BAD_CFG;
    }

    if (process() != SWS_OK)
    {
        return SWS_EXIT_ERROR;
    }

    fprintf(stdout, "SWS execution successful\n");
    return SWS_EXIT_OK;
}

/**
 * Run main program processing. Assumes s_schedules has been loaded.
 */
static SWS_rc_t process()
{
    int within_schedule = 0;
    if (is_within_schedule(&within_schedule) != SWS_OK)
    {
        return SWS_ERROR;
    }

    int gpio_state = SWS_gpio_get();

    if (within_schedule && gpio_state == SWS_GPIO_LOW)
    {
        SWS_gpio_set(SWS_GPIO_HIGH);
    }
    else if(!within_schedule && gpio_state == SWS_GPIO_HIGH)
    {
        SWS_gpio_set(SWS_GPIO_LOW);
    }

    return SWS_OK;
}

/**
 * Tests if the current local system time is within a configured schedule.
 * 
 * Assumes s_schedules has been loaded.
 */
static SWS_rc_t is_within_schedule(int* is_within_schedule)
{
    *is_within_schedule = 0;

    int now_day;
    int now_time_hhmm;

    if (SWS_get_now(&now_day, &now_time_hhmm) != SWS_OK)
    {
        fprintf(stderr, "Failed to get local time\n");
        return SWS_ERROR;
    }

    for (int i = 0; i < s_count_schedules; ++i)
    {
        if (s_schedules[i].day == now_day &&
            now_time_hhmm >= s_schedules[i].start_time &&
            now_time_hhmm < s_schedules[i].end_time)
        {
            *is_within_schedule = 1;
            break;
        }
    }

    return SWS_OK;
}
