/*******************************************************************************
* File Name     : stopwatch.c
* Description   :api for stopwatch display.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <machine.h>
#include <stdbool.h>
#include <stdio.h>
#include "stopwatch.h"
#include "platform.h"
#include "rtc_hal.h"
#include "MSSGvariable/rtcmsg.h"

/*******************************************************************************
* Prototypes for global variables
*******************************************************************************/
volatile bool stopwatchflag = false;
rtclock stopwatch = { .Hour10 = 0,.Hour1 = 0,.Min10 = 0,.Min1 = 0,.Sec1 = 0,.Sec10 = 0};
/*************external function prototypes and variables**************/
extern volatile bool sw1, sw2, sw3;



/******************************************************************************
* Function name: StopWatch_Display
* Description  : Displays the stopwatch on LCD
* Arguments    : none
* Return value : none
******************************************************************************/

void StopWatch_Display()
{
	char lcd_buffer[9];
	char*buffer = &lcd_buffer[0];

	if ((!stopwatchflag) && (sw3 == true)) //press sw3 to start only if stop watch hasn''t been started
	{
		stopwatch.Sec1 = 0, stopwatch.Sec10 = 0, stopwatch.Min1 = 0, stopwatch.Min10 = 0, stopwatch.Hour1 = 0, stopwatch.Hour10 = 0;
		sw3 = false;
		stopwatchflag = true;
		rtcstopWatch_Init();
	}
 
	if (sw2 == true)  //press sw2 to stop 
	{
		stopwatchflag = false;
		sw2 = false;

		IEN(RTC, PRD) = 0; //disable  periodic interrupt in ICU	
	}

	if (stopwatch.Sec1 > 9)
	{
		stopwatch.Sec1 = 0;
		stopwatch.Sec10 = stopwatch.Sec10 + 1;
	}
	if (stopwatch.Sec10 > 5)
	{
		stopwatch.Sec10 = 0;
		stopwatch.Min1 = stopwatch.Min1 + 1;
	}
	if (stopwatch.Min1 > 9)
	{
		stopwatch.Min1 = 0;
		stopwatch.Min10 = stopwatch.Min10 + 1;
	}
	if (stopwatch.Min10 > 5)
	{
		stopwatch.Min10 = 0;
		stopwatch.Hour1 = stopwatch.Hour1 + 1;
	}
	if (stopwatch.Hour1 > 9)
	{
		stopwatch.Hour1 = 0;
		stopwatch.Hour10 = stopwatch.Hour10 + 1;
	}

	lcd_display(LCD_LINE1, "Sw3 to start");
	lcd_display(LCD_LINE2, "Sw2 to stop");
	sprintf((char *)buffer, "%d%d:%d%d:%d%d", stopwatch.Hour10, stopwatch.Hour1, stopwatch.Min10, stopwatch.Min1, stopwatch.Sec10, stopwatch.Sec1);
	lcd_display(LCD_LINE6, buffer);
}


/******************************************************************************
* Function name: StopWatch_Display
* Description  : periodic RTC ISR of 1s 
* Arguments    : none
* Return value : none
******************************************************************************/
//RTC PRD
#pragma interrupt (Excep_RTC_PRD(vect = VECT_RTC_PRD))
void Excep_RTC_PRD(void);

//RTC_PRD_FUNC
void Excep_RTC_PRD(void)
{
	stopwatch.Sec1 = stopwatch.Sec1 + 1;
}
