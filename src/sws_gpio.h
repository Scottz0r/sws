#ifndef _SWS_GPIO_H_INCLUDE_GUARD
#define _SWS_GPIO_H_INCLUDE_GUARD

#define SWS_GPIO_HIGH 1
#define SWS_GPIO_LOW 0

#include "sws_types.h"

SWS_rc_t SWS_gpio_init();

int SWS_gpio_get();

void SWS_gpio_set(int value);

#endif //_SWS_GPIO_H_INCLUDE_GUARD
