/*******************************************************************************
* File Name     : alarm.c
* Description   : api to set alarm
*******************************************************************************/




/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include "platform.h"
#include "CMTTIMERsource/cmt_periodic_multi.h"
#include "MSSGvariable/rtcmsg.h"
#include "includes.h"

#define CHANNEL_0   0

/*******************************************************************************
* Prototypes for global variables
*******************************************************************************/
rtclock temp = { .Mon10 = 0,.Mon1 = 0,.Min10 = 0,.Min1 = 0,.Sec1 = 0,.Sec10 = 0,.dur = "AM" };
volatile bool alarmflag = 0;

/*************external function prototypes and variables**************/
extern volatile bool sw1, sw2, sw3;
extern volatile  int state;


/*******************************************************************************
* Function name: Set_Alarm
* Description  : State machine to set alarm by switches.
* Argument     : none
* Return value : none
*******************************************************************************/
void Set_Alarm()
{

	uint8_t buffer[30];
	if (alarmflag != 1)
	{
		static char smstate = 0;
		static bool x = 0;
		switch (smstate)
		{
		case 0:
			if (sw3 != true)
			{
				lcd_display(LCD_LINE3, "PLEASE SET");
				lcd_display(LCD_LINE4, "ALARM   ");
				lcd_display(LCD_LINE7, "¯¯    ");
				if (sw2 == true)
				{
					sw2 = false;
					temp.Hour1++;

					if (temp.Hour1 > 9)
					{
						temp.Hour10 = 1;
						temp.Hour1 = 0;

					}//end if3


					if ((temp.Hour10 == 1) && (temp.Hour1 > 2)) {
						temp.Hour10 = 0; temp.Hour1 = 0;
					}//end if 4

				}//end if2

			}//end if1 
			else
			{
				sw3 = false;
				smstate++;

			}
			break;

		case 1:
			if (sw3 != true)
			{
				lcd_display(LCD_LINE7, "   ¯¯ ");
				if (sw2 == true)
				{
					sw2 = false;
					temp.Min1++;

					if (temp.Min1 > 9)
					{
						if (temp.Min10 != 5)
						{
							temp.Min10++;
							temp.Min1 = 0;
						}
					}//end if3


					if ((temp.Min10 == 5) && (temp.Min1 > 9)) {
						temp.Min10 = 0; temp.Min1 = 0;
					}//end if 4

				}//end if2

			}//end if1 
			else
			{
				sw3 = false;
				smstate++;
			}
			break;

		case 2:
			if (sw3 != true)
			{
				lcd_display(LCD_LINE7, "      ¯¯");
				if (sw2 == true)
				{
					sw2 = false;
					temp.Sec1++;

					if (temp.Sec1 > 9)
					{
						if (temp.Sec10 != 5)
						{
							temp.Sec10++;
							temp.Sec1 = 0;

						}
					}//end if3


					if ((temp.Sec10 == 5) && (temp.Sec1 > 9)) {
						temp.Sec10 = 0; temp.Sec1 = 0;
					}//end if 4

				}//end if2

			}//end if1 
			else
			{
				sw3 = false;
				smstate++;
			}
			break;


		case 3:

			if (sw3 != true)
			{
				lcd_display(LCD_LINE7, "         ¯¯");
				if (sw2 == true)
				{
					sw2 = false;
					x = !x;
					temp.dur = x ? "PM" : "AM";

				}//end if2
			}//end if1 
			else
			{
				sw3 = false;
				smstate++;

			}
			break;


		case 4:
			smstate = 0;
			lcd_clear();
			lcd_display(LCD_LINE1, "TO RESET ALRM");
			lcd_display(LCD_LINE2, "PRESS SW3");
			lcd_display(LCD_LINE5, "ALARM SET TO ");

			rtcalarm(&temp);

			alarmflag = 1;
			cmt_stop(CHANNEL_0);
			break;

		}	// end switch

	}
	else
	{
    	lcd_display(LCD_LINE1, "TO RESET ALRM");
		lcd_display(LCD_LINE2, "PRESS SW3");
		lcd_display(LCD_LINE5, "ALARM SET TO ");

		if ((sw3 == true))
		{


			alarmintdisable();
			alarmflag = 0;
			sw3 = false;
			temp.Hour10 = 0; temp.Hour1 = 0; temp.Min10 = 0; temp.Min1 = 0; temp.Sec10 = 0; temp.Sec1 = 0;
			lcd_clear();
		}
	}

	sprintf((char *)buffer, "%d%d:%d%d:%d%d %s", temp.Hour10, temp.Hour1, temp.Min10, temp.Min1, temp.Sec10, temp.Sec1, temp.dur);
	lcd_display(LCD_LINE6, buffer);
}



/*******************************************************************************
* Function name: Excep_RTC_ALM
* Description  : ISR alarm, change mainapp statemachine state to display alarm
* Argument     : none
* Return value : none
*******************************************************************************/
#pragma interrupt (Excep_RTC_ALM(vect = VECT_RTC_ALARM))
void Excep_RTC_ALM(void)
{
	IR(RTC, ALARM) = 0;
	state = 7;

}
