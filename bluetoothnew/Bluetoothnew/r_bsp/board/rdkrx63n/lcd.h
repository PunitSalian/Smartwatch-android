
/***********************************************************************************************************************
* File Name	   : lcd.h
* Description  : Provides variable and function declarations for lcd.c file
***********************************************************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef LCD_H
#define LCD_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Defines standard integer variable types used in this file */
#include <stdint.h>

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define LCD_LINE1       0
#define LCD_LINE2       8
#define LCD_LINE3       16
#define LCD_LINE4       24
#define LCD_LINE5       32
#define LCD_LINE6       40
#define LCD_LINE7       48
#define LCD_LINE8       56

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
/* LCD initialisation function declaration */
void lcd_initialize (void);

/* Update display function declaration */
void lcd_display (uint8_t, const uint8_t *);

/* LCD write function delcaration */
void lcd_write (uint8_t, uint8_t);

/* Clear LCD function delcaration */
void lcd_clear (void);
void Set_Font_8_by_8(void);
void Set_Font_Bitmap(void);
void Set_LCD_Pos(int x, int y);
void Set_LCD_Char (char value);
/* End of multiple inclusion prevention macro */
#endif
