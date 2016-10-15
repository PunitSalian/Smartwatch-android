
/***********************************************************************************************************************
* File Name	   : r_glyph_lcd_register.c
* Description  : This ties the R_GLYPH_XxxXxx functions to the Glyph code.
***********************************************************************************************************************/


/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Glyph code header file. */
#include "Glyph\Glyph.h"
/* Used for LCD. */
#include "Glyph\Drivers\ST7579_LCD.h"
/* Used for communicating with LCD. */
#include "r_glyph.h"

/***********************************************************************************************************************
* Function Name: GlyphCommOpen
* Description  : Assign the communications workspace.
* Arguments    : a_handle -
*                    The Glyph handle to setup for the communications.
*                a_address -
*                    Which LCD to communicate to.
* Return Value : GLYPH_ERROR_NONE - 
*                    Success
*                GLYPH_ERROR_ILLEGAL_OPERATION - 
*                    Error
* ID           : 58.0
***********************************************************************************************************************/
T_glyphError GlyphCommOpen(T_glyphHandle aHandle, int32_t aAddress)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;

    switch (aAddress) {
        case 0:
            /* Set function pointers for Glyph code. */
            p_gw->iCommAPI->iOpen = R_GLYPH_Open;
            p_gw->iCommAPI->iCommandSend = R_GLYPH_CommandSend ;
            p_gw->iCommAPI->iDataSend = R_GLYPH_DataSend ;		
            break ;
        default:
            return GLYPH_ERROR_ILLEGAL_OPERATION ;
    }

    return p_gw->iCommAPI->iOpen(aHandle) ;
}

/***********************************************************************************************************************
* Function Name: GlyphLCDOpen
* Description  : Assign ST7579 LCD to a_handle.
* Arguments    : a_handle -
*                    The Glyph handle to setup for the LCD.
*                a_address -
*                    Which LCD to communicate to.
* Return Value : GLYPH_ERROR_NONE - 
*                    Success
*                GLYPH_ERROR_ILLEGAL_OPERATION - 
*                    Error
* ID           : 59.0
***********************************************************************************************************************/
T_glyphError GlyphLCDOpen(T_glyphHandle aHandle, int32_t aAddress)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;
    
    switch (aAddress) {
        case 0:
            /* Assign ST7579 functions. */
            p_gw->iLCDAPI->iOpen = ST7579_Open ;
            p_gw->iLCDAPI->iWrite = ST7579_Write ;
            p_gw->iLCDAPI->iRead = ST7579_Read ;
            p_gw->iLCDAPI->iClose = ST7579_Close ;
            break ;
        default:
            return GLYPH_ERROR_ILLEGAL_OPERATION ;
    }

    return GLYPH_ERROR_NONE ;
}

