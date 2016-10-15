
/***********************************************************************************************************************
* File Name    : r_switches.h
* Description  : Functions for using switches with callback functions. 
************************************************************************************************************************


/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Board and MCU support. */
#include "platform.h"
/* Switches prototypes. */
#include "r_switches.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* This helps reduce the amount of unique code for each supported board. */
#define X_IRQ( x )   XX_IRQ( x )
#define XX_IRQ( x )  _ICU_IRQ##x

/* These macros define which IRQ pins are used for the switches. Note that these defintions cannot have parentheses 
   around them. */
#if defined(PLATFORM_BOARD_RDKRX63N)
    #define SW1_IRQ_NUMBER     8
    #define SW2_IRQ_NUMBER     9
    #define SW3_IRQ_NUMBER     12
#elif defined(PLATFORM_BOARD_RSKRX63N)
    #define SW1_IRQ_NUMBER     2
    #define SW2_IRQ_NUMBER     8
    #define SW3_IRQ_NUMBER     15
#elif defined(PLATFORM_BOARD_RSKRX630)
    #define SW1_IRQ_NUMBER     2
    #define SW2_IRQ_NUMBER     12
    #define SW3_IRQ_NUMBER     15
#elif defined(PLATFORM_BOARD_RSKRX62N)
    #define SW1_IRQ_NUMBER     8
    #define SW2_IRQ_NUMBER     9
    #define SW3_IRQ_NUMBER     15
#elif defined(PLATFORM_BOARD_RDKRX62N)
    #define SW1_IRQ_NUMBER     8
    #define SW2_IRQ_NUMBER     9
    #define SW3_IRQ_NUMBER     10
#elif defined(PLATFORM_BOARD_RSKRX62T)
    #define SW1_IRQ_NUMBER     0
    #define SW2_IRQ_NUMBER     1
    #define SW3_IRQ_NUMBER     3
#elif defined(PLATFORM_BOARD_RSKRX610)
    #define SW1_IRQ_NUMBER     8
    #define SW2_IRQ_NUMBER     9
    #define SW3_IRQ_NUMBER     3
#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_SWITCHES_Init
* Description  : Initializes pins to be input and interrupt on switch presses.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SWITCHES_Init(void)
{      
    /* The SW#_XXX defintions are common macros amongst different boards. To see the definitions for these macros
       see the board defintion file. For example, this file for the RSKRX63N is rskrx63n.h. */

#if defined(MCU_RX62N) || defined(MCU_RX62T) || defined(MCU_RX621) || defined(MCU_RX610)

    /* Make switch pins inputs. */
    SW1_DDR = 0;
    SW2_DDR = 0;
    SW3_DDR = 0;

    /* Enable input buffer control registers. */
    SW1_ICR = 1;
    SW2_ICR = 1;
    SW3_ICR = 1;

#elif defined(MCU_RX63N) || defined(MCU_RX630) || defined(MCU_RX631)

    /* Unlock protection register */
    MPC.PWPR.BIT.B0WI = 0 ;     		
    /* Unlock MPC registers */    
    MPC.PWPR.BIT.PFSWE = 1 ;    		

    /* Make switch pins inputs. */
    SW1_PDR = 0;
    SW2_PDR = 0;
    SW3_PDR = 0;

    /* Set port mode registers for switches. */
    SW1_PMR = 0;
    SW2_PMR = 0;
    SW3_PMR = 0;

#endif

#if defined(PLATFORM_BOARD_RDKRX63N)

    /* The switches on the RDKRX63N are connected to the following pins/IRQ's    
    Switch  Port    IRQ
    ------  ----    ----
    SW1     P4.0    IRQ8
    SW2     P4.1    IRQ9
    SW3     P4.4    IRQ12
    */
    
    MPC.P40PFS.BYTE = 0x40;    /* P40 is used as IRQ pin */
    MPC.P41PFS.BYTE = 0x40;    /* P40 is used as IRQ pin */
    MPC.P44PFS.BYTE = 0x40;    /* P40 is used as IRQ pin */
    
#elif defined(PLATFORM_BOARD_RSKRX63N)

    /* The switches on the RSKRX63N are connected to the following pins/IRQ's    
    Switch  Port    IRQ
    ------  ----    ----
    SW1     P3.2    IRQ2
    SW2     P0.0    IRQ8
    SW3     P0.7    IRQ15
    */
    
    MPC.P32PFS.BYTE  = 0x40;    /* P32 is used as IRQ pin */
    MPC.P00PFS.BYTE  = 0x40;    /* P00 is used as IRQ pin */
    MPC.P07PFS.BYTE  = 0x40;    /* P07 is used as IRQ pin */

#elif defined(PLATFORM_BOARD_RSKRX630)

    /* The switches on the RSKRX630 are connected to the following pins/IRQ's    
    Switch  Port    IRQ
    ------  ----    ----
    SW1     P3.2    IRQ2
    SW2     P4.4    IRQ12
    SW3     P0.7    IRQ15
    */
    
    MPC.P32PFS.BYTE  = 0x40;    /* P32 is used as IRQ pin */
    MPC.P44PFS.BYTE  = 0x40;    /* P44 is used as IRQ pin */
    MPC.P07PFS.BYTE  = 0x40;    /* P07 is used as IRQ pin */

#elif defined(PLATFORM_BOARD_RSKRX62N)

    /* The switches on the RSKRX62N are connected to the following pins/IRQ's    
    Switch  Port    IRQ
    ------  ----    ----
    SW1     P0.0    IRQ8-A
    SW2     P0.1    IRQ9-A
    SW3     P0.7    IRQ15-A
    */
    
    IOPORT.PF8IRQ.BIT.ITS8  = 0;    /* IRQ8-A pin is used. */
    IOPORT.PF8IRQ.BIT.ITS9  = 0;    /* IRQ9-A pin is used. */
    IOPORT.PF8IRQ.BIT.ITS15 = 0;    /* IRQ15-A pin is used. */

#elif defined(PLATFORM_BOARD_RDKRX62N)

    /* The switches on the RDKRX62N are connected to the following pins/IRQ's    
    Switch  Port    IRQ
    ------  ----    ----
    SW1     P4.0    IRQ8
    SW2     P4.1    IRQ9
    SW3     P4.2    IRQ10
    */
    
    /* Nothing else needed to do here since RDK has 100-pin package and there are no alternate pins to choose. */

#elif defined(PLATFORM_BOARD_RSKRX62T)

    /* The switches on the RSKRX62T are connected to the following pins/IRQ's    
    Switch  Port    IRQ
    ------  ----    ----
    SW1     PE.5    IRQ0-B
    SW2     PE.4    IRQ1-B
    SW3     PB.4    IRQ3
    */
    
    IOPORT.PF8IRQ.BIT.ITS0  = 1;    /* IRQ0-B pin is used. */
    IOPORT.PF8IRQ.BIT.ITS1  = 1;    /* IRQ1-B pin is used. */    
    /* IRQ3 is only on 1 pin. */

#elif defined(PLATFORM_BOARD_RSKRX610)

    /* The switches on the RSKRX610 are connected to the following pins/IRQ's    
    Switch  Port    IRQ
    ------  ----    ----
    SW1     P0.0    IRQ8-A
    SW2     P0.1    IRQ9-A
    SW3     P1.3    IRQ3-B
    */
    
    IOPORT.PFCR8.BIT.ITS8  = 0;    /* IRQ8-A pin is used. */
    IOPORT.PFCR8.BIT.ITS9  = 0;    /* IRQ9-A pin is used. */    
    IOPORT.PFCR9.BIT.ITS3  = 1;    /* IRQ3-B pin is used. */

    /* Enable IRQ detection. */
    ICU.IRQER[SW1_IRQ_NUMBER].BIT.IRQEN = 1;
    ICU.IRQER[SW2_IRQ_NUMBER].BIT.IRQEN = 1;
    ICU.IRQER[SW3_IRQ_NUMBER].BIT.IRQEN = 1;

#endif


    /* Set IRQ type (falling edge) */
    ICU.IRQCR[SW1_IRQ_NUMBER].BIT.IRQMD  = 0x01; 
    ICU.IRQCR[SW2_IRQ_NUMBER].BIT.IRQMD  = 0x01; 
    ICU.IRQCR[SW3_IRQ_NUMBER].BIT.IRQMD  = 0x01; 
    
    /* Set interrupt priority to 3 */
    _IPR( X_IRQ(SW1_IRQ_NUMBER) ) = 3;
    _IPR( X_IRQ(SW2_IRQ_NUMBER) ) = 3;
    _IPR( X_IRQ(SW3_IRQ_NUMBER) ) = 3;
    
    /* Clear any pending interrupts */
    _IR( X_IRQ(SW1_IRQ_NUMBER) ) = 0;
    _IR( X_IRQ(SW2_IRQ_NUMBER) ) = 0;
    _IR( X_IRQ(SW3_IRQ_NUMBER) ) = 0;

    /* Enable the interrupts */
    _IEN( X_IRQ(SW1_IRQ_NUMBER) )  = 1; 
    _IEN( X_IRQ(SW2_IRQ_NUMBER) )  = 1; 
    _IEN( X_IRQ(SW3_IRQ_NUMBER) )  = 1;    
} 

#if defined(SW1_CALLBACK_FUNCTION)
/***********************************************************************************************************************
* Function name: sw1_isr
* Description  : Sample ISR for switch 1 input (must do hardware setup first!)
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
#pragma interrupt (sw1_isr (vect=_VECT(X_IRQ(SW1_IRQ_NUMBER))))
static void sw1_isr (void) 
{
    /* TODO: Add some debouncing! */

    /* Call callback function. */
    SW1_CALLBACK_FUNCTION();    
}    
#endif /* SW1_CALLBACK_FUNCTION */

#if defined(SW2_CALLBACK_FUNCTION)
/***********************************************************************************************************************
* Function name: sw2_isr
* Description  : Sample ISR for switch 2 input (must do hardware setup first!)
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
#pragma interrupt (sw2_isr (vect=_VECT(X_IRQ(SW2_IRQ_NUMBER))))
static void sw2_isr (void) 
{
    /* TODO: Add some debouncing! */

    /* Call callback function. */
    SW2_CALLBACK_FUNCTION();    
}    
#endif /* SW2_CALLBACK_FUNCTION */

#if defined(SW3_CALLBACK_FUNCTION)
/***********************************************************************************************************************
* Function name: sw3_isr
* Description  : Sample ISR for switch 3 input (must do hardware setup first!)
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
#pragma interrupt (sw3_isr (vect=_VECT(X_IRQ(SW3_IRQ_NUMBER))))
static void sw3_isr (void) 
{
    /* TODO: Add some debouncing! */
    
    /* Call callback function. */
    SW3_CALLBACK_FUNCTION();    
}
#endif /* SW3_CALLBACK_FUNCTION */

