

#include "board_encoder_emulation.h"


static uint16_t u16_target_period = 0U;
static uint16_t u16_current_period;

int32_t i32_board_encoder_rotation_dir;

/* Board encoder emulation init. */
BOARD_ERROR board_encoder_emulation_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    be_result = board_encoder_emulation_timer_init();

    return(be_result);
}

/* Start encoder emulation timer and interrupt. */
void board_encoder_emulation_start(void)
{
    /* here should be calculation of period. */


    /* Set start period. */
    /* board_encoder_emulation_set_period(ZERO_SPEED_PERIOD); start period should be set before function call. */

    /* Enable counter. */
    TIM_Cmd(TIM5, ENABLE);
    /* Enable update interrupt. */
    NVIC_EnableIRQ(TIM5_IRQn);
}

/* Stop encoder emulation timer and interrupt. */
void board_encoder_emulation_stop(void)
{
    TIM_Cmd(TIM5, DISABLE);
    NVIC_DisableIRQ(TIM5_IRQn);
}


/* input value is frequence in Hz, function calculate perion in uSec.   */
void board_encoder_emulation_set_frequency(int32_t i32_freq)
{
    float  f_freq;          /* Hz */
    float  f_period = 0;    /* Sec */
    uint32_t u32_period  = 0;
    if( i32_freq <= 0 )
    {
        i32_freq   = 0;
        u32_period = 0xFFFFFFFF;
        /* Here should be part for disabling timer */
    }
    else
    {   /* Here should be part for checking of enabling timer */
        f_freq      = i32_freq;
        f_period    = 1.0 / f_freq;          /* Period in seconds. */
        f_period    = 1000000.0 * f_period ; /* Period in uSec. */
        u32_period  = (uint32_t)f_period;
    }
    board_encoder_emulation_set_period(u32_period);
}

/* Set current timer period. For TIM2 and TIM5 ARR is 32 bits, for TIM3 and TIM4  ARR and CNT is 16 bits */
static void board_encoder_emulation_set_period(uint32_t u32_period)
{
    uint32_t u32_current_counter_value;

    TIM5->ARR = u32_period;
    u32_current_counter_value = TIM5->CNT;      /* Get current counter value*/
    if(u32_current_counter_value > u32_period)  /* If current value more than ARR, set CNT to ARR value to call TIM interrupt. */
    {
        TIM5->CNT = u32_period;
    }
}
/* Initialisation of timer for encoder emulation. */
static BOARD_ERROR board_encoder_emulation_timer_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    NVIC_InitTypeDef   NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* Enable the TIM5 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                      = (unsigned char)TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = TIMER5_PERIOD_INTERUPT_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = TIMER5_PERIOD_INTERUPT_SUB_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Init Timer1 like timer. */
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, ENABLE);

    /* Timer 5 work from 90MHz source clock. */
    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Period        = ZERO_SPEED_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler     = 89U;          /* Ftimer=fsys/(Prescaler+1),for Prescaler=89 ,Ftimer=1MHz */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0U;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);             /* Clear pending interrupt. */
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
    DBGMCU_APB1PeriphConfig(DBGMCU_TIM5_STOP, ENABLE); /* stop counting during debud breakpoint. */
    return(be_result);
}


/* Timer 5 update interrupt hundler. */
void TIM5_IRQHandler(void)
{
    static uint32_t u32_flag = 0;

    if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        board_encoder_emulation_proccess();                    /* Call function to emulate quadrature signals. */
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);            /* Counter overflow, reset interrupt */
    }
}

void board_encoder_emulation_proccess(void)
{
    /* Here is a place for generation of encoder signals. */
    if(i32_board_encoder_rotation_dir > 0)
    {
        /* ++ */
        board_encoder_emulation_output(1);
        board_encoder_emulation_AGP_output(1);
        board_motor_step(1);
    }
    else if(i32_board_encoder_rotation_dir < 0)
    {
        /* -- */
        board_encoder_emulation_output(-1);
        board_encoder_emulation_AGP_output(-1);
        board_motor_step(-1);
    }
    else
    {

    }
}

/* Function emulate AGP sensor output. */
static void board_encoder_emulation_AGP_output(int8_t i8_printer_step)
{
    static int32_t i32_current_possition;
    
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_E_IN_HEAD_GEAR_SENSOR) == 0)   /* If head in right position*/
    {
        i32_current_possition = i32_current_possition + (int32_t)(i8_printer_step); /* Add next step. */    
        
        if(i32_current_possition > AGP_VALUE_79)    /* Check round-robin value. */
        {
            i32_current_possition = 0;
        }
        else if(i32_current_possition < 0)
        {
            i32_current_possition = AGP_VALUE_79;
        }  
        else
        {        

        }

        /* Set right value of AGP sensor in depend of position value. */
        switch (i32_current_possition)
        {
            case AGP_VALUE_00:
            case AGP_VALUE_09:
            case AGP_VALUE_20:              
            case AGP_VALUE_29:              
            case AGP_VALUE_40:
            case AGP_VALUE_49:              
            case AGP_VALUE_60:              
            case AGP_VALUE_69:
                GPIO_SetBits( GPIOC, GPIO_Pin_12);                
                break;
            case AGP_VALUE_10:
            case AGP_VALUE_19:
            case AGP_VALUE_30:              
            case AGP_VALUE_39:              
            case AGP_VALUE_50:
            case AGP_VALUE_59:              
            case AGP_VALUE_70:              
            case AGP_VALUE_79:
                GPIO_ResetBits( GPIOC, GPIO_Pin_12);                
                break;
            default:
                break;
        }       
      
    } 
}

/* Function generate encoder signal A and B. Input parametr is 1 or -1 . */
static void board_encoder_emulation_output(int8_t i8_printer_step)
{
#if 1
    static int8_t i8_encoder_possition_counter = 0;

    /* Round encoder. */
    i8_encoder_possition_counter = i8_encoder_possition_counter + i8_printer_step;
    if(i8_encoder_possition_counter > 3)
    {
        i8_encoder_possition_counter = 0;
    }
    else if(i8_encoder_possition_counter < 0)
    {
        i8_encoder_possition_counter = 3;
    }
    else
    {
    }
    switch (i8_encoder_possition_counter)
    {
        case 0:
            GPIO_SetBits(   GPIOG, GPIO_G_OUT_ENCODER_A);
            GPIO_SetBits(   GPIOG, GPIO_G_OUT_ENCODER_B);
            break;
        case 1:
            GPIO_SetBits(   GPIOG, GPIO_G_OUT_ENCODER_A);
            GPIO_ResetBits( GPIOG, GPIO_G_OUT_ENCODER_B);
            break;
        case 2:
            GPIO_ResetBits( GPIOG, GPIO_G_OUT_ENCODER_A);
            GPIO_ResetBits( GPIOG, GPIO_G_OUT_ENCODER_B);
            break;
        case 3:
            GPIO_ResetBits( GPIOG, GPIO_G_OUT_ENCODER_A);
            GPIO_SetBits(   GPIOG, GPIO_G_OUT_ENCODER_B);
            break;
        default:
            break;
    }
#endif
}









