
/***********************************************************************************************************************
* File Name	   : lcd.c
* Description  : LCD Module utility functions. Written for RDK Okaya graphic LCD.
***********************************************************************************************************************/


/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Processor-specific details */
#include <machine.h>
/* Standard string manipulation & formatting functions */
#include <stdio.h>
#include <string.h>
/* Defines standard variable types used in this function */
#include <stdint.h>
/* Board includes. */
#include "platform.h"
/* Following header file provides function prototypes for LCD controlling functions & macro defines */
#include "lcd.h"
/* Graphics library support */
#include "glyph.h"
/* RSPI package. */
#include "r_rspi_rx600.h"

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
T_glyphHandle lcd_handle;


/***********************************************************************************************************************
* Function name : lcd_initialize
* Description   : Initializes the LCD display. 
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void lcd_initialize(void)
{
    /* Initialize RSPI channel */
    R_RSPI_Init(RSPI_CHANNEL_0);    
    
    if (GlyphOpen(&lcd_handle, 0) == GLYPH_ERROR_NONE)
    {
		GlyphWrite(lcd_handle, GLYPH_FRAME_RATE, 137);
		GlyphWrite(lcd_handle, GLYPH_CONTRAST, 255);
        /* use Glyph full access direct functions */
        GlyphNormalScreen(lcd_handle);
        GlyphSetFont(lcd_handle, GLYPH_FONT_8_BY_8);
        GlyphClearScreen(lcd_handle);
    }
}

/***********************************************************************************************************************
* Function name : lcd_clear
* Description   : Clears the LCD
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void lcd_clear(void)
{
    GlyphClearScreen(lcd_handle);
}

/***********************************************************************************************************************
* Function name : lcd_display
* Description   : This function controls the LCD writes.
*                 The display supports 8 lines with up to 12 characters per line. Use the defines LCD_LINE1 to 
*                 LCD_LINE8 to specfify the starting position.
*                 For example, to start at the 4th position on line 1:
*                     lcd_display(LCD_LINE1 + 4, "Hello")
* Arguments     : position - 
*                     Line number of display
*                 string - 
*                     Pointer to null terminated string
* Return Value  : none
***********************************************************************************************************************/
void lcd_display(uint8_t position, const uint8_t * string)
{
    uint8_t y = position - (position % 8);
    uint8_t xOffset = (position % 8)<<3;

    /* Draw text lines, 8 pixels high, 96 pixels wide */
    /* Clear the rectangle of this line */
    GlyphEraseBlock(lcd_handle, xOffset, y, (95 - xOffset), y+7);
    GlyphSetXY(lcd_handle, xOffset, y);
    GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));

}


/***********************************************************************************************************************
* Function name : Set_Font_8_by_8
* Description   : 
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void Set_Font_8_by_8(void)
{
    GlyphSetFont(lcd_handle, GLYPH_FONT_8_BY_8);
}


/***********************************************************************************************************************
* Function name : Set_Font_Bitmap
* Description   : 
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void Set_Font_Bitmap(void)
{
    GlyphSetFont(lcd_handle, GLYPH_FONT_BITMAP);
}


/***********************************************************************************************************************
* Function name : Set_LCD_Pos(int x, int y)
* Description   : 
* Arguments     : x and y co-ordinate
* Return Value  : none
***********************************************************************************************************************/
void Set_LCD_Pos(int x, int y)
{
    GlyphSetX(lcd_handle,x);
	GlyphSetY(lcd_handle,y);
}


/***********************************************************************************************************************
* Function name : Set_LCD_Char(char value)
* Description   : 
* Arguments     : value
* Return Value  : none
***********************************************************************************************************************/
void Set_LCD_Char(char value)
{
    GlyphChar(lcd_handle,value);
}