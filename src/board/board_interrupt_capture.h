#ifndef BOARD_INITERRUPT_CAPTURE_H
#define BOARD_INITERRUPT_CAPTURE_H 1

#include "stm32f4xx.h"
#include "board_system_type.h"
#include "board_NVIC.h"

#define AGP_ARRAY_SIZE  100

extern int32_t i32_AGP_distance[AGP_ARRAY_SIZE][2];


BOARD_ERROR be_board_int_cap_init(void);
void board_int_cap_configure_PE2(void);
void EXTI2_IRQHandler(void); 

#endif