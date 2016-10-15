/*******************************************************************************
* File Name     : speaker.c
* Description   : api for speaker functions
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"


/******************************************************************************
* Function name: Speaker_Init
* Description  : speaker initializes
* Arguments    : none
* Return value : none
******************************************************************************/
void Speaker_Init(void)
{
	PORT1.PDR.BIT.B7 = 1;
	PORT1.PMR.BIT.B7 = 0;

	PORTA.PDR.BIT.B2 = 1;
	PORTA.PMR.BIT.B2 = 0;
	PORTA.PODR.BIT.B2 = 1;		//AMP ON

	PORTC.PDR.BIT.B1 = 1;
	PORTC.PMR.BIT.B1 = 0;
	PORTC.PODR.BIT.B1 = 1;		//SPEAKER ON	
}

/******************************************************************************
* Function name: Speaker_ON
* Description  : speaker turns on
* Arguments    : none
* Return value : none
******************************************************************************/
void Speaker_ON(void)
{


	PORT1.PODR.BIT.B7 = 0;
	for (unsigned int i = 0; i<100; i++)
		for (unsigned int j = 0; j<50; j++);
	PORT1.PODR.BIT.B7 = 1;
	for (unsigned int i = 0; i<100; i++)
		for (unsigned int j = 0; j<50; j++);

}

/******************************************************************************
* Function name: Speaker_OFF
* Description  : speaker turns off
* Arguments    : none
* Return value : none
******************************************************************************/
void Speaker_OFF(void)
{
	PORTA.PDR.BIT.B2 = 1;
	PORTA.PMR.BIT.B2 = 0;
	PORTA.PODR.BIT.B2 = 0;		//AMP OFF

	PORTC.PDR.BIT.B2 = 1;
	PORTC.PMR.BIT.B2 = 0;
	PORTC.PODR.BIT.B2 = 0;		//SPEAKER OFF	

	PORT1.PODR.BIT.B7 = 0;
}
