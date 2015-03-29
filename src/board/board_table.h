#ifndef BOARD_TABLE_H
#define BOARD_TABLE_H 1


#include "stm32f4xx.h"
#include "board_system_type.h"
#include "board_motor.h"

#define STEP_BACK_VALUE     100
#define INIT_CHECK_TIMEOUT  1     /* It change initialisation and manusl speed. 0 is maximum speed value (It can be too high.) */

BOARD_ERROR board_table_init(uint32_t u32_reset);
       void board_table_set_init_flag(uint8_t u8_flag);
       void board_table_set_init_state(uint8_t u8_state);
       void board_table_set_PE_state(uint8_t u8_state);
    uint8_t board_table_get_init_state(void);




#endif