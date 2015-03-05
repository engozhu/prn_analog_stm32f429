#ifndef BOARD_BUTTONS_H
#define BOARD_BUTTONS_H 1



#include "stm32f4xx.h"
#include "board_system_type.h"
#include "board_gpio_table.h"
#include "board_motor.h"
#include "board_table.h"

BOARD_ERROR be_board_buttons_get_state(BOARD_BUTTONS bb_in, uint8_t *u8_state);
BOARD_ERROR be_board_buttons_pressed(BOARD_BUTTONS bb_in);
BOARD_ERROR be_board_buttons_process(void);








#endif