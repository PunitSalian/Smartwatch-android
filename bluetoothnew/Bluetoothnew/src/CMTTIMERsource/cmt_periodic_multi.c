
/*******************************************************************************
* File Name     : cmt_periodic_multi.c
* Description   : Functions to provide periodic timers using a selected CMT channel
*******************************************************************************/


/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <stdint.h>

/* I/O Register and board definitions */
#include "platform.h"  /* Brings in the appropriate iodefines file. */
#include "cmt_periodic_multi.h"


/******************************************************************************
Local function declarations and local global variables
******************************************************************************/
static void (*cmt_callback_0)(void) = 0;
static void (*cmt_callback_1)(void) = 0;
static void (*cmt_callback_2)(void) = 0;
static void (*cmt_callback_3)(void) = 0;
/******************************************************************************
* Function name : cmt_callback_set
* Description 	: Receives a pointer to a function and sets it as the callback
*				  function for the CMT interrupt. The callback is
*				  executed when the CMT interrupt ISR is invoked on a compare match.
*
* Notes         : This callback will be executing within an ISR, holding off interrupts;  
*                 Be aware of the timing impact of this on the rest of the system 
*                 and keep it short!
*
* Arguments     : channel -
*               :    Which channel number to set
*               : func -
*               :    The pointer to the callback function.
* Return Value  : none
******************************************************************************/
void cmt_callback_set(uint8_t channel, void(*func)(void))
{
    switch (channel)
    {
        case 0:
            cmt_callback_0 = func; /* Assign the pointer to the callback function. */
        break;
        
        case 1:
            cmt_callback_1 = func; /* Assign the pointer to the callback function. */
        break;
        
		
		
		
		      case 2:
            cmt_callback_2 = func; /* Assign the pointer to the callback function. */
        break;
		
		
		      case 3:
            cmt_callback_3 = func; /* Assign the pointer to the callback function. */
        break;
		
		
		
        default:
        break;
    }       

} /* End of function cmt_callback_set() */


/******************************************************************************
* Function name : cmt_init
* Description   : Create one-shot timers based on PCLK / 3
* Arguments     : None
* Return Value  : None
******************************************************************************/
void cmt_init(void)
{
    #ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
    #endif
    
    /* Cancel CMT module stop state. */
    MSTP(CMT0) = 0;  
    MSTP(CMT1) = 0; 
	MSTP(CMT2) = 0; 
	MSTP(CMT3) = 0;   
    #ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
    #endif  

    /* Compare Match Timer Control Register (CMCR) 
    b15:b8  reserved:   Read/Write value always 0
    b7      reserved:   Read undef. Write value always 1
    b6      CMIE:       Compare Match Interrupt Enable                
    b5:b2   reserved:   Read/Write value always 0
    b1:b0   CKS:        clock select   3 = count on PCLK / 512  
    */
    CMT0.CMCR.WORD = 0x0003; /* Just set clock divisor for now. */
    CMT1.CMCR.WORD = 0x0003; 
    CMT2.CMCR.WORD = 0x0003;     
	CMT3.CMCR.WORD = 0x0003;
    /* Compare Match Timer Counter (CMCNT)
    b15:b0  16-bit readable/writable up-counter to generate interrupt requests.
    */
    CMT0.CMCNT = 0x00;      /* Clear the count (default value). */
    CMT1.CMCNT = 0x00;      
    CMT2.CMCNT = 0x00;
	CMT3.CMCNT = 0x00;        
    /* Compare Match Timer Constant Register (CMCOR))
    b15:b0  16-bit register sets the value for a compare match with CMCNT.
    */ 
    CMT0.CMCOR = 0xFFFF;    /* Set to max (default value). */
    CMT1.CMCOR = 0xFFFF;    
    CMT2.CMCOR = 0xFFFF;
	CMT3.CMCOR = 0xFFFF;
    
	IR (CMT0, CMI0);        /* Interrupt reset.  */ 
    IR (CMT1, CMI1);
    IR (CMT2, CMI2);
	IR (CMT3, CMI3);
	
	
	
    IPR(CMT0, CMI0) = 4;    /* Interrupt priority set. */
    IPR(CMT1, CMI1) = 4; 
	IPR(CMT2, CMI2) = 4;
	IPR(CMT3, CMI3) = 4;
           
    IEN(CMT0, CMI0) = 1;    /* Interrupt enable. */
    IEN(CMT1, CMI1) = 1;        
     IEN(CMT2, CMI2) = 1; 
	  IEN(CMT3, CMI3) = 1; 
} /* End of function cmt_init() */


/******************************************************************************
* Function name : cmt_start
* Description   : Sets the compare match value then start the timer.
* Arguments     : channel -
*               :    Which channel number to start
*               : match_constant - 
*                     The counter compare match value.                                 .
* Return Value  : None
******************************************************************************/
void cmt_start(uint8_t channel, uint16_t match_constant)
{
    switch (channel)
    {
        case 0:
            CMT0.CMCNT          = 0x00;           /* Clear counter register. */
            CMT0.CMCOR          = match_constant; /* Set the value for count match interrupt. */
            CMT0.CMCR.BIT.CMIE  = 1;              /* Enable count match interrupt. */
            CMT.CMSTR0.BIT.STR0 = 1;              /* Start counting. */
        break;
        
        case 1:
            CMT1.CMCNT          = 0x00;           /* Clear counter register. */
            CMT1.CMCOR          = match_constant; /* Set the value for count match interrupt. */
            CMT1.CMCR.BIT.CMIE  = 1;              /* Enable count match interrupt. */
            CMT.CMSTR0.BIT.STR1 = 1;              /* Start counting. */
        break;
        
		
		
		 
        case 2:
            CMT2.CMCNT          = 0x00;           /* Clear counter register. */
            CMT2.CMCOR          = match_constant; /* Set the value for count match interrupt. */
            CMT2.CMCR.BIT.CMIE  = 1;              /* Enable count match interrupt. */
            CMT.CMSTR1.BIT.STR2 = 1;              /* Start counting. */
        break;
		
		 
        case 3:
            CMT3.CMCNT          = 0x00;           /* Clear counter register. */
            CMT3.CMCOR          = match_constant; /* Set the value for count match interrupt. */
            CMT3.CMCR.BIT.CMIE  = 1;              /* Enable count match interrupt. */
            CMT.CMSTR1.BIT.STR3 = 1;              /* Start counting. */
        break;
		
		
		
		
		
		
		
		
        default:
        break;
    }    

} /* End of function cmt_start() */


/******************************************************************************
* Function name : cmt_stop
* Description   : Stops the Compare match timer and disables its interrupt.
* Arguments     : channel -
*               :    Which channel number to stop
* Return Value  : None
******************************************************************************/
void cmt_stop (uint8_t channel)
{
    switch (channel)
    {
        case 0:
            CMT0.CMCR.BIT.CMIE  = 0;      /* Disable count match interrupt. */
            CMT.CMSTR0.BIT.STR0 = 0;      /* Stop counting. */
        break;
        
        case 1:
            CMT1.CMCR.BIT.CMIE  = 0;      /* Disable count match interrupt. */
            CMT.CMSTR0.BIT.STR1 = 0;      /* Stop counting. */
        break;
        
		
		 case 2:
            CMT2.CMCR.BIT.CMIE  = 0;      /* Disable count match interrupt. */
            CMT.CMSTR1.BIT.STR2 = 0;      /* Stop counting. */
        break;
		
		 case 3:
            CMT3.CMCR.BIT.CMIE  = 0;      /* Disable count match interrupt. */
            CMT.CMSTR1.BIT.STR3 = 0;      /* Stop counting. */
        break;
		
		
		
		
		
		
		
        default:
        break;
    }
} /* End of function cmt_stop() */


/******************************************************************************
* Function name: CMT0_isr
* Description  : ISR for CMT channel 0 interrupt. Optionally calls a callback 
*                function if one has been assigned.
* Arguments    : None
* Return value : None
******************************************************************************/
#pragma interrupt (CMT0_isr (vect = VECT(CMT0, CMI0)))
static void CMT0_isr (void) 
{
    if (cmt_callback_0)
    {
        cmt_callback_0();
    }
} /* End of CMT0_isr */

/******************************************************************************
* Function name: CMT1_isr
* Description  : ISR for CMT channel 1 interrupt. Optionally calls a callback 
*                function if one has been assigned.
* Arguments    : None
* Return value : None
******************************************************************************/
#pragma interrupt (CMT1_isr (vect = VECT(CMT1, CMI1)))
static void CMT1_isr (void) 
{
    if (cmt_callback_1)
    {
        cmt_callback_1();
    }
} /* End of CMT1_isr */




#pragma interrupt (CMT2_isr (vect = VECT(CMT2, CMI2)))
static void CMT2_isr (void) 
{
    if (cmt_callback_2)
    {
        cmt_callback_2();
    }
} /* End of CMT1_isr */



#pragma interrupt (CMT3_isr (vect = VECT(CMT3, CMI3)))
static void CMT3_isr (void) 
{
    if (cmt_callback_3)
    {
        cmt_callback_3();
    }
} /* End of CMT1_isr */




/*******************************************************************************
End of file: cmt_periodic.c
*******************************************************************************/