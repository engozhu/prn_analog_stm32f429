#ifndef BOARD_LCD_H
#define BOARD_LCD_H 1


#include "stm32f4xx.h"
#include <stdio.h>
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"

#include "board_system_type.h"


#define MESSAGE1        "ADC conversion w/DMA"
#define MESSAGE1_1      "continuouslyTransfer"
#define MESSAGE2        " ADC Ch13 Conv   "
#define MESSAGE2_1      "    2.4Msps      "

#define MESSAGE5   " ADC3 = %d.%1dV "

#define FONTSIZE        Font12x12
#define LINENUM         0x15

BOARD_ERROR board_lcd_display_init(void);
BOARD_ERROR board_lcd_display(uint32_t u32_input);



#endif