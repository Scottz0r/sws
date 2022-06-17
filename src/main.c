#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"

#include "sws_types.h"
#include "sws_config.h"
#include "sws_time.h"
#include "sws_gpio.h"

#define SWS_EXIT_OK 0
#define SWS_EXIT_ERROR 1
#define SWS_EXIT_BAD_ARG 2
#define SWS_EXIT_BAD_CFG 3  

static void get_log_path();
static SWS_rc_t process();
static SWS_rc_t is_within_schedule(int* is_within_schedule);

// Reserve enough space for a schedule entry for each hour every day of the week. This is an
// unlikely scenario because blocks should be used to span multiple hours.
#define SWS_MAX_SCHEDULE (24 * 7)
SWS_schedule_t s_schedules[SWS_MAX_SCHEDULE];
int s_count_schedules;

// Static memory for the log file path.
#define SWS_DEFAULT_LOG_FILE "sws.log"
#define SWS_LOG_ENV "SWS_LOG_PATH"
#define LOG_FILE_PATH_SIZE 256
_Static_assert(sizeof(SWS_DEFAULT_LOG_FILE) < LOG_FILE_PATH_SIZE, "Invalid default log file name");
char s_log_file_path[LOG_FILE_PATH_SIZE];

int main(int argc, const char** argv)
{
    get_log_path();

    if (DBG_log_init(s_log_file_path) != DBG_LOG_OK)
    {
        fprintf(stderr, "Failed to open log file\n");
    }

    if (argc != 2)
    {
        fprintf(stderr, "Invalid arguments. Expected [config file]\n");
        DBG_log("Called with invalid arguments");
        return SWS_EXIT_BAD_ARG;
    }

    DBG_assert_continue(argv != NULL);
    const char* cfg_path = argv[1];

    if (SWS_gpio_init() != SWS_OK)
    {
        DBG_log("Failed to initialize GPIO interface");
        return SWS_EXIT_ERROR;
    }

    s_count_schedules = SWS_parse_config(cfg_path, s_schedules, SWS_MAX_SCHEDULE);
    if (s_count_schedules < 0)
    {
        DBG_log("Failed to parse config");
        return SWS_EXIT_BAD_CFG;
    }
    else if(s_count_schedules == 0)
    {
        DBG_log("No values configured");
        return SWS_EXIT_BAD_CFG;
    }

    if (process() != SWS_OK)
    {
        return SWS_EXIT_ERROR;
    }

    DBG_log("SWS execution successful");
    return SWS_EXIT_OK;
}

/**
 * Sets the log file path baesd on an environment variable or the default.
 */
static void get_log_path()
{
    const char* env_path = getenv(SWS_LOG_ENV);
    if (env_path != NULL)
    {
        // Do not copy if outside of bounds.
        size_t len = strlen(env_path);
        if (len < LOG_FILE_PATH_SIZE - 1)
        {
            // Safe to use strcpy becuase path will fit in buffer with null terminator.
            strcpy(s_log_file_path, env_path);
            return;
        }
    }

    // Fall back to default path.
    strcpy(s_log_file_path, SWS_DEFAULT_LOG_FILE);
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

    SWS_gpio_state_t gpio_state = SWS_gpio_get();

    if (within_schedule && gpio_state == SWS_GPIO_LOW)
    {
        DBG_log("Inside schedule and pin LOW. Toggling HIGH");
        SWS_gpio_set(SWS_GPIO_HIGH);
    }
    else if(!within_schedule && gpio_state == SWS_GPIO_HIGH)
    {
        DBG_log("Outside schedule and pin HIGH. Toggling LOW");
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
        DBG_log("Failed to get local time");
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
