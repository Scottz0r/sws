#ifndef _SWS_GPIO_H_INCLUDE_GUARD
#define _SWS_GPIO_H_INCLUDE_GUARD

#include "sws_types.h"

SWS_rc_t SWS_gpio_init();

SWS_gpio_state_t SWS_gpio_get();

void SWS_gpio_set(SWS_gpio_state_t value);

#endif //_SWS_GPIO_H_INCLUDE_GUARD
