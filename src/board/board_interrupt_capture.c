
#include "board_interrupt_capture.h"
/*  
    Instrumentall functions for counting distance between AGP sensor states. 
    This function should be added to init alone. 
*/

int32_t i32_AGP_distance[AGP_ARRAY_SIZE][2];

BOARD_ERROR be_board_int_cap_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    board_int_cap_configure_PE2(); 

    return(be_result);
}    

/* Configure pins to be interrupts */
void board_int_cap_configure_PE2(void) 
{
    /* Set variables used */
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    /* Enable clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    /* Enable clock for SYSCFG */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    /* Tell system that you will use PE2 for EXTI_Line2 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);

    EXTI_InitStruct.EXTI_Line       = EXTI_Line2;                   /* PE2 is connected to EXTI_Line2 */
    EXTI_InitStruct.EXTI_LineCmd    = ENABLE;                       /* Enable interrupt */
    EXTI_InitStruct.EXTI_Mode       = EXTI_Mode_Interrupt;          /* Interrupt mode */
    EXTI_InitStruct.EXTI_Trigger    = EXTI_Trigger_Rising;          /* Triggers on rising and falling edge */
    EXTI_Init(&EXTI_InitStruct);                                    /* Add to EXTI */
 
    /* Add IRQ vector to NVIC */
    NVIC_InitStruct.NVIC_IRQChannel                     = EXTI2_IRQn;   /* PE2 is connected to EXTI_Line2, which has EXTI2_IRQn vector */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = EXT_LINE_2_PERIOD_INTERUPT_PRIORITY_GROUP;             /* Set priority */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = EXT_LINE_2_PERIOD_INTERUPT_SUB_PRIORITY_GROUP;         /* Set sub priority */
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       /* Enable interrupt */
    NVIC_Init(&NVIC_InitStruct);                                        /* Add to NVIC */
}


/* Set interrupt handlers */
/* Handle PE2 interrupt. It handle rising edge of pulse from rotary encoder A. */
void EXTI2_IRQHandler(void) 
{
    static int32_t i32_encoder_A_counter;
    static uint32_t u32_filter_counter = 0;
    static uint32_t u32_AGP_state[3];
    static uint32_t u32_AGP_array_counter=0;
  
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line2) != RESET) 
    {
        if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)       /* If head in right position*/
        {  
            if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)   /* Get rotary direction. */
            {
                i32_encoder_A_counter ++;
            }
            else
            {
                i32_encoder_A_counter --;
            }  
            u32_AGP_state[u32_filter_counter] = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11);
            u32_filter_counter++;
            if(u32_filter_counter > 2)
            {
                u32_filter_counter = 0;
            }  
            if(u32_AGP_state[0] == u32_AGP_state[1])        /* Check if last two time AGP line has same value. */
            {
                if(u32_AGP_state[0] != u32_AGP_state[2])    /* If old value was chnged, rewrite it. */
                {
                    u32_AGP_state[2] = u32_AGP_state[0];
                    if(u32_AGP_array_counter < AGP_ARRAY_SIZE)  /* If save array is not full, save current state AGP line and current value of counter. */
                    {  
                        i32_AGP_distance[u32_AGP_array_counter][0] = u32_AGP_state[2];
                        i32_AGP_distance[u32_AGP_array_counter][1] = i32_encoder_A_counter;
                        u32_AGP_array_counter++;
                    }
                }  
            }  
        }
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}


#if 0 
void Configure_PB12(void) {
    /* Set variables used */
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    /* Enable clock for GPIOB */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    /* Enable clock for SYSCFG */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* Tell system that you will use PB12 for EXTI_Line12 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);
    
    /* PB12 is connected to EXTI_Line12 */
    EXTI_InitStruct.EXTI_Line = EXTI_Line12;
    /* Enable interrupt */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    /* Interrupt mode */
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    /* Triggers on rising and falling edge */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* Add to EXTI */
    EXTI_Init(&EXTI_InitStruct);
 
    /* Add IRQ vector to NVIC */
    /* PB12 is connected to EXTI_Line12, which has EXTI15_10_IRQn vector */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    /* Set priority */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    /* Set sub priority */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
    /* Enable interrupt */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStruct);
}
 
/* Set interrupt handlers */
/* Handle PD0 interrupt */
void EXTI0_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        /* Do your stuff when PD0 is changed */
        
        
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
 
/* Handle PB12 interrupt */
void EXTI15_10_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line12) != RESET) {
        /* Do your stuff when PB12 is changed */
        
        
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
}


#endif


















