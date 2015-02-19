#ifndef BOARD_FILTER_LP3KHZ_H
#define BOARD_FILTER_LP3KHZ_H


#include "stm32f4xx.h"
#include "board_system_type.h"


float board_filter_lp3kHz_iir(float NewSample);


#endif