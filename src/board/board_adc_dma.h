#ifndef BOARD_ADC_DMA_H
#define BOARD_ADC_DMA_H 1


#include "stm32f4xx.h"
//#include <stdio.h>
//#include "stm32f429i_discovery.h"
//#include "stm32f429i_discovery_lcd.h"
#include "board_system_type.h"
#include "board_NVIC.h"
#include "board_filter_lp3kHz.h"
#include "board_encoder_emulation.h"
#include "board_table.h"


#define THRESHOLD_VALUE  800


#define ADC3_DR_ADDRESS     ((uint32_t)0x4001224C)

extern uint16_t uhADC3ConvertedValue[4];
extern uint32_t uwADC3ConvertedVoltage;


BOARD_ERROR board_adc_dma_init(void);
void TIM3_IRQHandler(void);



#endif