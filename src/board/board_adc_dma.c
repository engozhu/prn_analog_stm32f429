

#include "board_adc_dma.h"

uint16_t uhADC3ConvertedValue[4];
uint32_t uwADC3ConvertedVoltage;


/* Board encoder emulation init. */

BOARD_ERROR board_adc_dma_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    ADC_InitTypeDef             ADC_InitStructure;
    ADC_CommonInitTypeDef       ADC_CommonInitStructure;
    DMA_InitTypeDef             DMA_InitStructure;
    GPIO_InitTypeDef            GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
    NVIC_InitTypeDef            NVIC_InitStructure;

    /* Enable ADC3, DMA2 and GPIO clocks ****************************************/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

    /* DMA2 Stream0 channel2 configuration **************************************/
    DMA_InitStructure.DMA_Channel               = DMA_Channel_2;
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)ADC3_DR_ADDRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr       = (uint32_t)&uhADC3ConvertedValue;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize            = 2;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;//DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode              = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold         = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst           = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);



    /* Configure TIM2 for triggering ADC3 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_Period = 200;
    TIM_TimeBaseStructure.TIM_Prescaler = 44; /* 180000000/2 => (90000000/(Prescaler + 1))/ Period = (90000000/(44+1))/200 = 10kHz */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

    NVIC_InitStructure.NVIC_IRQChannel                      = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = TIMER2_PERIOD_INTERUPT_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = TIMER2_PERIOD_INTERUPT_SUB_PRIORITY_GROUP;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DBGMCU_APB1PeriphConfig(DBGMCU_TIM2_STOP, ENABLE); /* stop counting during debud breakpoint. */

    TIM_Cmd(TIM2, ENABLE);

    NVIC_EnableIRQ(TIM2_IRQn);


    /* Configure ADC3 Channel13 pin as analog input ******************************/
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* Configure ADC3 Channel4 pin as analog input ******************************/
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    /* ADC Common Init **********************************************************/
    ADC_CommonInitStructure.ADC_Mode                = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler           = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode       = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay    = ADC_TwoSamplingDelay_20Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    /* ADC3 Init ****************************************************************/
    ADC_InitStructure.ADC_Resolution                = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode              = ENABLE;  // DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode        = DISABLE; // ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge      = ADC_ExternalTrigConvEdge_Rising; //ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv          = ADC_ExternalTrigConv_T2_TRGO; //ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign                 = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion           = 2;
    ADC_Init(ADC3, &ADC_InitStructure);

    /* ADC3 regular channel13 configuration *************************************/
    /*
       In this example, the system clock is 180MHz, APB2 = 90MHz and ADC clock = APB2/2.
       Since ADC3 clock is 45 MHz and sampling time is set to 3 cycles, the conversion
       time to 12bit data is 12 cycles so the total conversion time is (12+7)/45= 0.42us(2.37Msps).
       12 is 12 bit, so 1 cycles for 1 bit + minimum 7 cycles to reach maximum 2.4Msps conversion speed.
    */

    ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 1, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC3, ADC_Channel_4,  2, ADC_SampleTime_480Cycles); /* sampling time - время одного семпла. */
    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

    /* Enable ADC3 DMA */
    ADC_DMACmd(ADC3, ENABLE);

    /* Enable ADC3 */
    ADC_Cmd(ADC3, ENABLE);

    return(be_result);
}

void TIM2_IRQHandler()
{
    float f_A_ch_value = 0;
    float f_B_ch_value = 0;

    static uint32_t u32_speed_counter = 0;

    if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)                    /* If compare capture has occured. */
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);                 /* Clear Update counter, because we had interrupt from input pin. */
    }

    if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET)                    /* If compare capture has occured */
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }

    if(TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET)                    /* If compare capture has occured */
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }

    if(TIM_GetITStatus(TIM2, TIM_IT_CC4) == SET)                    /* If compare capture has occured */
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }

    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        /*
            This part call, in according with u32_speed_counter value,
            board_table_init() function for providing init table possition if it is neccessary.
        */
        if(u32_speed_counter < INIT_CHECK_TIMEOUT )
        {
            u32_speed_counter++;
        }
        else
        {
            u32_speed_counter = 0;
            board_table_init(0);
            board_table_init(0);
            board_table_init(0);
            board_table_init(0);
            be_board_buttons_process();
            board_table_set_PE_state(1);
        }

        /* Pass ADC value through filters. */
        uhADC3ConvertedValue[2] = (uint32_t)board_filter_A_channel_lp3kHz_iir((float)uhADC3ConvertedValue[0]);
        uhADC3ConvertedValue[3] = (uint32_t)board_filter_B_channel_lp3kHz_iir((float)uhADC3ConvertedValue[1]);
//        uhADC3ConvertedValue[2] = (uint32_t)board_filter_A_channel_lp1Hz_iir((float)uhADC3ConvertedValue[0]);
//        uhADC3ConvertedValue[2] = uhADC3ConvertedValue[2]/3;
        /* Calculation of input ADC channels in volts. */
        f_A_ch_value = uhADC3ConvertedValue[2] * 3.3 / 0x0FFF ;
        f_B_ch_value = uhADC3ConvertedValue[3] * 3.3 / 0x0FFF ;
        {
            static float sf_omega_zero   = 0;
          volatile float sf_omega        = 0;
          volatile float sf_force        = 0;
                   float sf_force_coeff  = 3300000.0;
                   float sf_break_coeff  = 185.0;
                   float sf_moment_coeff = 2.0;
                   float sf_delta_time   = 0.0001;/* 10000kHz, or 100uSec.*/

                sf_force = (f_A_ch_value - f_B_ch_value) * sf_force_coeff;  /* Calculation input rotation force. */

                sf_omega = sf_omega_zero + ((sf_force - sf_break_coeff * sf_omega_zero)/sf_moment_coeff)*sf_delta_time; /* Calculation rotation speed. */

                sf_omega_zero = sf_omega; /* Save current rotation speed. */

                /* Calculation of direction state. */
                if(  (sf_omega <= THRESHOLD_VALUE) && (sf_omega >= -(THRESHOLD_VALUE)) )
                {
                    i32_board_encoder_rotation_dir = 0;
                }
                else if( sf_omega > THRESHOLD_VALUE )
                {
                    i32_board_encoder_rotation_dir = 1;
                }
                else if( sf_omega < -(THRESHOLD_VALUE) )
                {
                    i32_board_encoder_rotation_dir = -1;
                }

                if( sf_omega < 0)
                {
                    sf_omega = -(sf_omega);
                }

                //sf_omega = board_filter_A_channel_lp1Hz_iir(sf_omega);
                //sf_omega = sf_omega/3.0;
                board_encoder_emulation_set_frequency((int32_t) sf_omega);
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);                 /* Counter overflow, reset interrupt */
    }
}

