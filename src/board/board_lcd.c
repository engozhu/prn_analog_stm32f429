

#include "board_lcd.h"


/* Board encoder emulation init. */

BOARD_ERROR board_lcd_display_init(void)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    /* Initialize the LCD */
    LCD_Init();
    LCD_LayerInit();

    /* Enable the LTDC */
    LTDC_Cmd(ENABLE);

    /* Set LCD Background Layer  */
    LCD_SetLayer(LCD_BACKGROUND_LAYER);

    /* Clear the Background Layer */
    LCD_Clear(LCD_COLOR_WHITE);

    /* Configure the transparency for background */
    LCD_SetTransparency(0);

    /* Set LCD Foreground Layer  */
    LCD_SetLayer(LCD_FOREGROUND_LAYER);

    /* Configure the transparency for foreground */
    LCD_SetTransparency(200);

    /* Clear the Foreground Layer */
    LCD_Clear(LCD_COLOR_WHITE);

    /* Set the LCD Back Color and Text Color*/
    LCD_SetBackColor(LCD_COLOR_BLUE);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    /* Set the LCD Text size */
    LCD_SetFont(&FONTSIZE);

    /* Set the LCD Back Color and Text Color*/
    LCD_SetBackColor(LCD_COLOR_BLUE);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    LCD_DisplayStringLine(LINE(LINENUM), (uint8_t*)MESSAGE1);
    LCD_DisplayStringLine(LINE(LINENUM + 1), (uint8_t*)MESSAGE1_1);
    LCD_DisplayStringLine(LINE(0x17), (uint8_t*)"                               ");

    /* Set the LCD Text size */
    LCD_SetFont(&Font16x24);

    LCD_DisplayStringLine(LCD_LINE_0, (uint8_t*)MESSAGE2);
    LCD_DisplayStringLine(LCD_LINE_1, (uint8_t*)MESSAGE2_1);

    /* Set the LCD Back Color and Text Color*/
    LCD_SetBackColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    return(be_result);
}


BOARD_ERROR board_lcd_display(uint32_t u32_input)
{
    BOARD_ERROR be_result = BOARD_ERR_OK;

    uint32_t uwVoltage =0;
    uint32_t uwMVoltage=0;
    uint8_t aTextBuffer[50];

    uwVoltage  = (u32_input)/1000;
    uwMVoltage = (u32_input % 1000)/1;

    sprintf((char*)aTextBuffer, MESSAGE5, uwVoltage, uwMVoltage);
    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)aTextBuffer);

    return(be_result);

}


