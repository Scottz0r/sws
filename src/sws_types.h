#ifndef _SWS_TYPES_H_INCLUDE_GUARD
#define _SWS_TYPES_H_INCLUDE_GUARD

#include <inttypes.h>

typedef uint32_t u32;

typedef enum SWS_rc_t
{
    SWS_OK = 0,
    SWS_ERROR = 1
} SWS_rc_t;

typedef enum SWS_gpio_state_t
{
    SWS_GPIO_HIGH = 1,
    SWS_GPIO_LOW = 0
} SWS_gpio_state_t;

typedef struct SWS_schedule_t
{
    int day;
    int start_time;
    int end_time;
} SWS_schedule_t;

#endif // _SWS_TYPES_H_INCLUDE_GUARD
