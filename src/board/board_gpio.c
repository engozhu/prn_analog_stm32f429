

#include "board_gpio.h"



/* This function should set direction and mode for all pin working like GPIO. */
BOARD_ERROR be_board_gpio_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    /* AGP output pin . */
    be_result |= be_board_pin_init( GPIOC, GPIO_C_OUT_APG_SENSOR, GPIO_Speed_100MHz, GPIO_Mode_OUT);   /* AGP out. */
                      GPIO_SetBits( GPIOC, GPIO_C_OUT_APG_SENSOR);

    /* Encoder output pins. */
    be_result |= be_board_pin_init( GPIOG, GPIO_G_OUT_ENCODER_A, GPIO_Speed_100MHz, GPIO_Mode_OUT);   
                      GPIO_SetBits( GPIOG, GPIO_G_OUT_ENCODER_A);    
    be_result |= be_board_pin_init( GPIOG, GPIO_G_OUT_ENCODER_B, GPIO_Speed_100MHz, GPIO_Mode_OUT);   
                      GPIO_SetBits( GPIOG, GPIO_G_OUT_ENCODER_B);
    
    /* Motor "Enable" and "Direction" pins. */
    be_result |= be_board_pin_init( GPIOA, GPIO_A_OUT_MOTOR_DIR, GPIO_Speed_100MHz, GPIO_Mode_OUT);  
                    GPIO_ResetBits( GPIOA, GPIO_A_OUT_MOTOR_DIR);   
    be_result |= be_board_pin_init( GPIOC, GPIO_C_OUT_MOTOR_ENABLE, GPIO_Speed_100MHz, GPIO_Mode_OUT);
                    GPIO_ResetBits( GPIOC, GPIO_C_OUT_MOTOR_ENABLE);
    
    /* Buttons. */
    be_result |= be_board_pin_init( GPIOE, GPIO_Pin_2,  GPIO_Speed_2MHz, GPIO_Mode_IN); /* button B1. */
    be_result |= be_board_pin_init( GPIOE, GPIO_Pin_3,  GPIO_Speed_2MHz, GPIO_Mode_IN); /* button B2. */
    be_result |= be_board_pin_init( GPIOE, GPIO_Pin_5,  GPIO_Speed_2MHz, GPIO_Mode_IN); /* button B3. */
    be_result |= be_board_pin_init( GPIOE, GPIO_Pin_6,  GPIO_Speed_2MHz, GPIO_Mode_IN); /* button B4. */

 
     /* PE output pin . */
    be_result |= be_board_pin_init( GPIOC, GPIO_C_OUT_PE_SENSOR, GPIO_Speed_100MHz, GPIO_Mode_OUT);   /* AGP out.            */
                      GPIO_SetBits( GPIOC, GPIO_C_OUT_PE_SENSOR);
 
 
    /* Head gear sensor pin. */
    be_result |= be_board_pin_init( GPIOE, GPIO_E_IN_HEAD_GEAR_SENSOR,  GPIO_Speed_2MHz, GPIO_Mode_IN);     /* HEAD_Gear_sensor. */  


    /* Table sensors pins. */
    be_result |= be_board_pin_init( GPIOG, GPIO_G_IN_ENCODER_SIDE_END_SENSOR,  GPIO_Speed_2MHz, GPIO_Mode_IN);  /* Ensoder side sensor. */
    be_result |= be_board_pin_init( GPIOG, GPIO_G_IN_MOTOR_SIDE_END_SENSOR,  GPIO_Speed_2MHz, GPIO_Mode_IN);    /* Motor side sensor. */ 
    be_result |= be_board_pin_init( GPIOG, GPIO_G_IN_PE_SENSOR,  GPIO_Speed_2MHz, GPIO_Mode_IN);                /* PE table sensor. */

    return(be_result);
    
    //if(GPIO_ReadInputDataBit(GPIOE, GPIO_E_IN_HEAD_GEAR_SENSOR) == 0)   /* If head in right position*/
    //{
    //}
}




/*
    This function is setting GPIO pin on required mode and direction.
    GPIO_Mode_IN    GPIO Input Mode
    GPIO_Mode_OUT   GPIO Output Mode
    GPIO_Mode_AF    GPIO Alternate function Mode
    GPIO_Mode_AN    GPIO Analog Mode
*/

BOARD_ERROR be_board_pin_init(
                                GPIO_TypeDef *gpio_board_port,
                                uint16_t u16_port_pin,
                                GPIOSpeed_TypeDef gpio_speed_pin_speed,
                                GPIOMode_TypeDef gpio_mode_pin_mode
                              )
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    /* Create structure for initialisation. */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable PORT Peripheral clock. */
    if(gpio_board_port == GPIOA)
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA , ENABLE);
    }
    else if(gpio_board_port == GPIOB)
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB , ENABLE);
    }
    else if(gpio_board_port == GPIOC)
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC , ENABLE);
    }
    else if(gpio_board_port == GPIOD)
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD , ENABLE);
    }
    else if(gpio_board_port == GPIOE)
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE , ENABLE);
    }
    else if(gpio_board_port == GPIOF)
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF , ENABLE);
    }
    else if(gpio_board_port == GPIOG)
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOG , ENABLE);
    }
    else if(gpio_board_port == GPIOH)
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOH , ENABLE);
    }
    else if(gpio_board_port == GPIOI)
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOI , ENABLE);
    }
    else if(gpio_board_port == GPIOJ)
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOJ , ENABLE);
    }
    else if(gpio_board_port == GPIOK)
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOK , ENABLE);
    }
    else
    {
        be_result = BOARD_ERR_ERROR;
    }

    /* Configurate the GPIO pin structure. */
    GPIO_InitStructure.GPIO_Pin   = u16_port_pin;
    GPIO_InitStructure.GPIO_Speed = gpio_speed_pin_speed;
    GPIO_InitStructure.GPIO_Mode  = gpio_mode_pin_mode;

    /* Call GPIO init function. */
    GPIO_Init( gpio_board_port, &GPIO_InitStructure);
    return(be_result);
}
