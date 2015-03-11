#ifndef BOARD_TABLE_H
#define BOARD_TABLE_H 1


#include "stm32f4xx.h"
#include "board_system_type.h"
#include "board_motor.h"

#define STEP_BACK_VALUE     100
#define INIT_CHECK_TIMEOUT  7

BOARD_ERROR board_table_init(void);
       void board_table_set_init_flag(uint8_t u8_flag);







#endif