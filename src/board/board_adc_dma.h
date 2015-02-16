#ifndef BOARD_ADC_DMA_H
#define BOARD_ADC_DMA_H 1


#include "stm32f4xx.h"
//#include <stdio.h>
//#include "stm32f429i_discovery.h"
//#include "stm32f429i_discovery_lcd.h"
#include "board_system_type.h"

#define ADC3_DR_ADDRESS     ((uint32_t)0x4001224C)

extern uint16_t uhADC3ConvertedValue;
extern uint32_t uwADC3ConvertedVoltage;


BOARD_ERROR board_adc_dma_init(void);




#endif