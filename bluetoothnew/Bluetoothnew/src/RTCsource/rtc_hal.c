/*******************************************************************************
* File Name     : rtc_hal.c
* Description   :hardware abstraction layer for RTC
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "MSSGvariable/rtcmsg.h"
#include "platform.h"
#include "rtc_hal.h"
#include <string.h>



/******************************************************************************
* Function name: clockset
* Description  : sets clock for the RTC that is the main clock as the source of clock
* Arguments    : none
* Return value : none
******************************************************************************/
void clockset()
{
	int i;
	RTC.RCR4.BIT.RCKSEL = 1;//Select the main clock
	for (i = 0; i< 0x10; i++)     //delay to pass atleast 6 clock cycle of the main clock        
	{
	}
	RTC.RCR2.BIT.START = 0; //Write 0 to start bit in RCR2
	while (RTC.RCR2.BIT.START == 1) {} //Wait for start bit to clear to 0
	RTC.RCR2.BIT.RESET = 1; //Write 0 to start bit in RCR2
	RTC.RFRH.WORD = 1;  //set frequency register for 128 Hz
	while (RTC.RCR2.BIT.RESET == 0) {} //Wait for start bit to clear to 0

}




/******************************************************************************
* Function name: Inittimer
* Description  : Initializes the RTC with initial month year time
* Arguments    : none
* Return value : none
******************************************************************************/
void Inittimer(rtclock* rt)
{
	//Set date and time on RTC
	RTC.RCR2.BIT.START = 0; //Write 0 to start bit in RCR2
	while (RTC.RCR2.BIT.START == 1) {} //Wait for start bit to clear to 0
	RTC.RCR2.BIT.RESET = 1; //Write 1 to Reset bit in RCR2
	RTC.RCR2.BIT.HR24 = 0; //RTC operates in 12 hour mode
	RTC.RYRCNT.WORD = (rt->Year10 << 4) | rt->Year1;
	RTC.RMONCNT.BYTE = (rt->Mon10 << 4) | rt->Mon1;

	RTC.RDAYCNT.BYTE = (rt->Day10 << 4) | rt->Day1;
	RTC.RWKCNT.BYTE = rt->Day;

	if (((rt->Hour10) == 1) && ((rt->Hour1) == 2))
	{
		rt->Hour10 = 0; rt->Hour1 = 0;
	}

	RTC.RHRCNT.BYTE = (rt->Hour10 << 4) | rt->Hour1;

	RTC.RMINCNT.BYTE = (rt->Min10 << 4) | rt->Min1;
	RTC.RSECCNT.BYTE = (rt->Sec10 << 4) | rt->Sec1;
	RTC.RHRCNT.BIT.PM = (strcmp(rt->dur, "AM")>0) ? 1 : 0;
	RTC.RCR2.BIT.START = 1; //Set start bit to 1 in RCR2
	while (RTC.RCR2.BIT.START == 0) {} //Wait for start bit to set to 1
									   //Set carry interrupt for use later when reading time from RTC
	IEN(RTC, COUNTUP) = 0; //Disable carry interrupt in ICU
	RTC.RCR1.BIT.CIE = 1; //Enable carry interrupt in the RTC
}

/******************************************************************************
* Function name: Gettime
* Description  : Read in the year month day time
* Arguments    : none
* Return value : none
******************************************************************************/

void GetTime(rtclock* rt)
{
	do {
		IR(RTC, COUNTUP) = 0; //Clear the Carry Flag
		rt->Year10 = RTC.RYRCNT.BIT.YR10;
		rt->Year1 = RTC.RYRCNT.BIT.YR1;
		rt->Mon10 = RTC.RMONCNT.BIT.MON10;
		rt->Mon1 = RTC.RMONCNT.BIT.MON1;
		rt->Day10 = RTC.RDAYCNT.BIT.DATE10;
		rt->Day1 = RTC.RDAYCNT.BIT.DATE1;
		rt->Day = RTC.RWKCNT.BYTE;
		rt->Hour10 = RTC.RHRCNT.BIT.HR10;
		rt->Hour1 = RTC.RHRCNT.BIT.HR1;
		rt->Min10 = RTC.RMINCNT.BIT.MIN10;
		rt->Min1 = RTC.RMINCNT.BIT.MIN1;
		rt->Sec10 = RTC.RSECCNT.BIT.SEC10;
		rt->Sec1 = RTC.RSECCNT.BIT.SEC1;
		rt->dur = RTC.RHRCNT.BIT.PM ? "PM" : "AM";
	} while (IR(RTC, COUNTUP) == 1);//if carry flag high means time has changed read again
}

/******************************************************************************
* Function name: rtcstopWatch_Init
* Description  : initalize the rtc hardware for stopwatch
* Arguments    : none
* Return value : none
******************************************************************************/

void rtcstopWatch_Init()
{


	//Use periodic interrupt to wait
	IR(RTC, PRD) = 0;
	RTC.RCR1.BYTE = 0xE5; //Set periodic interrupt 

	while (IR(RTC, PRD) == 0) //Wait for flag to be set
		;
	//Set up Periodic interrupt to trigger ISR every 1s
	RTC.RCR1.BYTE = 0xE5;//Set periodic interrupt to 1s
	IR(RTC, PRD) = 0; //Clear periodic interrupt flag
	IEN(RTC, PRD) = 1; //Enable periodic interrupt in ICU
	IPR(RTC, PRD) = 2; //Set priority level
}

/******************************************************************************
* Function name: rtcalarm
* Description  : initalize the rtc hardware for alarm feture
* Arguments    : none
* Return value : none
******************************************************************************/
void rtcalarm(rtclock* temp)
{
	IEN(RTC, ALARM) = 0;

	RTC.RSECAR.BIT.ENB = 0x00;
	RTC.RMINAR.BIT.ENB = 0x00;
	RTC.RHRAR.BIT.ENB = 0x00;
	RTC.RWKAR.BIT.ENB = 0x00;
	RTC.RDAYAR.BIT.ENB = 0x00;
	RTC.RMONAR.BIT.ENB = 0x00;
	RTC.RYRAREN.BIT.ENB = 0x00;

	RTC.RHRAR.BYTE = temp->Hour10 << 4 | temp->Hour1;
	RTC.RMINAR.BYTE = temp->Min10 << 4 | temp->Min1;
	RTC.RSECAR.BYTE = temp->Sec10 << 4 | temp->Sec1;
	RTC.RHRAR.BIT.PM = (strcmp(temp->dur, "AM")>0) ? 1 : 0;

	RTC.RSECAR.BIT.ENB = 0x01;
	RTC.RMINAR.BIT.ENB = 0x01;
	RTC.RHRAR.BIT.ENB = 0x01;


	RTC.RCR1.BIT.AIE = 1;
	while (RTC.RCR1.BIT.AIE == 0);

	for (unsigned int i = 15625; i > 0; i--)
	{
		for (unsigned int j = 150; j > 0; j--);
	}
	IR(RTC, ALARM) = 0;
	IEN(RTC, ALARM) = 1;
	IPR(RTC, ALARM) = 3;


}



/******************************************************************************
* Function name: alarmintdisable
* Description  : disable alarm interrupt
* Arguments    : none
* Return value : none
******************************************************************************/

void alarmintdisable()
{

	RTC.RCR1.BIT.AIE = 1;
	IEN(RTC, ALARM) = 0;
	RTC.RCR1.BIT.AIE = 0;
	while (RTC.RCR1.BIT.AIE == 1);
	IR(RTC, ALARM) = 0;

}





