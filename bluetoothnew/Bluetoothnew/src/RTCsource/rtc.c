/*******************************************************************************
* File Name     : RTC.c
* Description   : api for the time and date module.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include"STRINGsource/strtok.h"
#include"MSSGvariable/rtcmsg.h"
#include <stdlib.h>
#include "platform.h"
#include "rtc_hal.h"
#include <stdio.h>


/*******************************************************************************
* Function name: DisplayTime
* Description  :display the date&time in YYYY MM/DD  HH:MM AM/PM Format
* Arguments    : none
* Return value : none
*
*******************************************************************************/
void DisplayTime(void) {

	rtclock rt;
	char buffer[30];
	const char *DayNames[7] = { "Sun", "Mon", "Tue", "Wed", "Thu","Fri", "Sat" };
	GetTime(&rt);
	if (rt.Hour10 == 0 && rt.Hour1 == 0)
	{
		rt.Hour10 = 1; rt.Hour1 = 2;
	}

	sprintf((char *)buffer, "20%d%d %d%d / %d%d", rt.Year10, rt.Year1, rt.Mon10, rt.Mon1, rt.Day10, rt.Day1);//convert to string

	lcd_display(LCD_LINE1, buffer);
	lcd_display(LCD_LINE3, DayNames[rt.Day]);
	sprintf((char *)buffer, "%d%d:%d%d %s", rt.Hour10, rt.Hour1, rt.Min10, rt.Min1, rt.dur);
	lcd_display(LCD_LINE4, buffer);

}


/*******************************************************************************
* Function name: fillstruct
* Description  :Fills rtc struct variable after parsing
* Arguments    : none
* Return value : none
*
*******************************************************************************/
void fillstruct(int index, void* value, rtclock* r)
{

	switch (index)
	{
	case 0:
		r->Mon1 = ((char)*((int*)value)) % 10;
		r->Mon10 = ((char)*((int*)value)) / 10;
		break;

	case 1:
		r->Day1 = ((char)*((int*)value)) % 10;
		r->Day10 = ((char)*((int*)value)) / 10;
		break;

	case 2:
		r->Year1 = ((unsigned short)*((int*)value)) % 10;
		r->Year10 = (((unsigned short)*((int*)value)) / 10) % 10;
		break;

	case 3:
		r->Hour1 = ((char)*((int*)value)) % 10;
		r->Hour10 = ((char)*((int*)value)) / 10;
		break;

	case 4:
		r->Min1 = ((char)*((int*)value)) % 10;
		r->Min10 = ((char)*((int*)value)) / 10;
		break;

	case 5:
		r->Sec1 = ((char)*((int*)value)) % 10;
		r->Sec10 = ((char)*((int*)value)) / 10;
		break;

	case 6:
		r->dur = ((char*)value);
		break;

	case 7:
		r->Day = (char)*((int*)value) - 1;
		break;

	}
}


/*******************************************************************************
* Function name:parsertc
* Description  :parse the time string recieved from the app int format &MM/DD/YYYY HH:MM:SS AM/PM DAY|
* Arguments    : none
* Return value : none
*
*******************************************************************************/
void parsertc(char*new_char)
{
	rtclock x;
	int count = 0;
	char * pch;
	pch = strtok(new_char, " &:|/");
	while (pch != NULL)
	{
		if (count != 6)
		{
			int y = atoi(pch);
			fillstruct(count, &y, &x);
		}
		else
			fillstruct(count, pch, &x); //the main function which parses the string

		pch = strtok(NULL, " &:|/");
		count++;
	}
	clockset();
	Inittimer(&x);

}







