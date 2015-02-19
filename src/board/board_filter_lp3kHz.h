#ifndef BOARD_FILTER_LP3KHZ_H
#define BOARD_FILTER_LP3KHZ_H


#include "stm32f4xx.h"
#include "board_system_type.h"


float board_filter_A_channel_lp3kHz_iir(float NewSample);
float board_filter_B_channel_lp3kHz_iir(float NewSample);
float board_filter_A_channel_lp1Hz_iir(float NewSample);
#endif