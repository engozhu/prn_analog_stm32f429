
#include "board_table.h"


uint8_t u8_init_flag = 0;



void board_table_set_init_flag(uint8_t u8_flag)
{
    u8_init_flag = u8_flag;
}




BOARD_ERROR board_table_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;
    static uint32_t u32_state_counter = 0;
    static uint32_t u32_step_counter = 0;

    if(u8_init_flag == 1)
    {

        switch (u32_state_counter)
        {
            case 0: /* Looking for motor side end position. */
                if( GPIO_ReadInputDataBit(GPIOG, GPIO_G_IN_MOTOR_SIDE_END_SENSOR) == END_SENSOR_OFF)
                {
                    board_motor_step(1);
                }
                else
                {
                    u32_state_counter = 1;
                }
                break;
            case 1: /* Looking for encoder side end position. */
                if( GPIO_ReadInputDataBit(GPIOG, GPIO_G_IN_ENCODER_SIDE_END_SENSOR) == END_SENSOR_OFF)
                {
                    board_motor_step(-1);
                }
                else
                {
                    u32_state_counter = 2;
                }
                break;
            case 2: /* Looking for PE end position. */
                if( GPIO_ReadInputDataBit(GPIOG, GPIO_G_IN_PE_SENSOR) == END_SENSOR_OFF)
                {
                    board_motor_step(1);
                }
                else
                {
                    u32_state_counter = 3;
                }
                break;
            case 3:
                if( u32_step_counter <= STEP_BACK_VALUE) /* 100 is amount of step to step back from PE sensor. */
                {
                    board_motor_step(-1);
                    u32_step_counter++;
                }
                else
                {
                    u32_step_counter    = 0;
                    u32_state_counter   = 4;
                }
                break;
            case 4:
                u32_state_counter = 0;  /* Set counter to start of state. */
                u8_init_flag = 0;       /* Reset initialisation flag. Now we can do one more init.  */
                break;
            default:
                be_result = BOARD_ERR_RANGE;
                break;
        }
    }
    else
    {
    }
    return(be_result);
}














