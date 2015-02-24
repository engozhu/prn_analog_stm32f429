#ifndef BOARD_ENCODER_EMULATION_H
#define BOARD_ENCODER_EMULATION_H 1


#include "stm32f4xx.h"
#include "board_system_type.h"
#include "board_NVIC.h"
#include "board_capture.h"
#include "board_gpio_table.h"
#include "board_motor.h"

#define ZERO_SPEED_PERIOD   35000U
#define PID_PROPORTIONAL    10   /* from 4000 to 50 around 4500mS */

/* AGP sensor possition definition. */
#define AGP_VALUE_00    0   /* UP   tooth 0 */
#define AGP_VALUE_09    9   /* UP   tooth 0 */

#define AGP_VALUE_10    10  /* DOWN */
#define AGP_VALUE_19    19  /* DOWN */

#define AGP_VALUE_20    20  /* UP   tooth 1 */
#define AGP_VALUE_29    29  /* UP   tooth 1 */

#define AGP_VALUE_30    30  /* DOWN */
#define AGP_VALUE_39    39  /* DOWN */

#define AGP_VALUE_40    40  /* UP   tooth 2 */
#define AGP_VALUE_49    49  /* UP   tooth 2 */

#define AGP_VALUE_50    50  /* DOWN */
#define AGP_VALUE_59    59  /* DOWN */

#define AGP_VALUE_60    60  /* UP   tooth 3 */
#define AGP_VALUE_69    69  /* UP   tooth 3*/

#define AGP_VALUE_70    70  /* DOWN */
#define AGP_VALUE_79    79  /* DOWN */


extern int32_t i32_board_encoder_rotation_dir;

       BOARD_ERROR board_encoder_emulation_init(void);
static BOARD_ERROR board_encoder_emulation_timer_init(void);
static        void board_encoder_emulation_set_period(uint32_t u32_period);

              void board_encoder_emulation_set_frequency(int32_t i32_freq);

static        void board_encoder_emulation_proccess(void);

void board_encoder_emulation_start(void);
              void board_encoder_emulation_stop(void);

static        void board_encoder_emulation_AGP_output(int8_t i8_printer_step);              
static        void board_encoder_emulation_output(int8_t i8_printer_step);


void TIM5_IRQHandler(void);






#endif