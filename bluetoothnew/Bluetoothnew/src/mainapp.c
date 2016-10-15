
/*******************************************************************************
* File Name     : mainapp.c
* Description   : main application program
*
*******************************************************************************/
/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/

#include "includes.h"

/*******************************************************************************
* Prototypes for global variables
*******************************************************************************/
volatile  char new_char[25];
volatile char* pointer = &new_char[0];
message m;
volatile bool powerdown = false;
volatile  int state = 0;
long debounce = 0;
volatile int delaytime = 0;
volatile bool alarm = false;
volatile bool delaycomplete = false;
void(*fpoint)(void) = 0;
volatile bool taskcontinue = false;

/*************external function prototypes and variables**************/

extern volatile int16_t g_accel_y_zero;
extern volatile bool sw1, sw2, sw3;
extern void acceltempsensorinit();
extern void readheartsensor();
extern void power_modes_demo(void);
extern volatile unsigned int BPM;
extern void  displaybpm();
extern void parsedetail(char*);
extern volatile bool notif;
extern void Speaker_Init();
extern void Speaker_ON();
extern void Speaker_OFF();


/******************************************************************************
* Function name: delay
* Description  : temp delay callback
* Arguments    : none
* Return value : none
******************************************************************************/
void delay()
{

	delaytime++;

	if (delaytime == 5)
	{
		delaytime = 0;
		delaycomplete = true;
		cmt_stop(CHANNEL_0);
	}

}




/******************************************************************************
* Function name: serialinp
* Description  :this is timer task of 100ms which checks inclination and for notification
first char @=message notification,!=call notification,&= time notification,#= detail notification

* Arguments    : none
* Return value : none
******************************************************************************/

void serialinp()
{
	debounce++;//used for debounce delay


	if ((accel_axis_read(0x34) - g_accel_y_zero)<-20)//this is used to check the inclination if the y<-20 that is vertically down so now power down
	{
		if (!((notif == true) ^ (alarm == true)))
			powerdown = 1;

	}

	do {


		if (!sci_read_count_get())
		{

			return;
		}



		LED4 = ~LED4;

		char tempchar = sci_get_char();

		*pointer = tempchar;
		pointer++;
		if (tempchar == '|')
		{
			pointer = &new_char[0];
			if (*pointer == '@')
			{
				state = 6;
				m.messageflag = true;
			}
			else
				if (*pointer == '!')
				{
					state = 6;
					m.callflag = true;
				}
				else
					if (*pointer == '&')
					{
						m.timeflag = true;

					}
					else
						if (*pointer == '#')
						{
							m.detailflag = true;

						}

		}

	} while (!sci_read_count_get());

}



/******************************************************************************
* Function name: Initialize
* Description  : Initializes peripheral sets up sensor
* Arguments    : none
* Return value : none
******************************************************************************/
void Initialize()
{
	/* Initialize LCD */
	lcd_initialize();

	/* Clear LCD */
	lcd_clear();

	/* Initialize the 12 bit ADC */
	S12ADC_init();

	/* Initialize the SCI channel for asynchronous UART communications. */
	sci_uart_init();
	sci_tx_int_enable();
	sci_rx_int_enable();


	/* Initialize the accelometer and temp sensor */
	acceltempsensorinit();

	/* Prepare the switch interrupt  */
	R_SWITCHES_Init();



}



/******************************************************************************
* Function name: checkbuttonstate
* Description  : changes the button state based on the switch state
* Arguments    : none
* Return value : none
******************************************************************************/


void checkbuttonstate()
{

	if (sw2 == true)
	{
		lcd_clear();
		sw2 = false;
		state--;
		if (state<0 || state>5)
			state = 0;
	}
	else
		if (sw3 == true)
		{
			lcd_clear();
			sw3 = false;
			state++;
			if (state>5)
			{
				state = 5;
				Speaker_OFF();

				notif = false;
				alarm = false;
			}

		}

}
/******************************************************************************
* Function name: main
* Description  : Main program function initializes peripheral runs the statemachine
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{  //temp variables
	unsigned int tdelay = 0;
	char lcd_char[25];
	char*tempchar = &lcd_char[0];


	Initialize();


	/* Display welcome message on LCD */
	lcd_display(LCD_LINE1, "  RENWATCH   ");
	lcd_display(LCD_LINE2, "  WELCOME  ");

	lcd_display(LCD_LINE4, "OPEN APP ");

	lcd_display(LCD_LINE6, "PLEASE PRESS ");
	lcd_display(LCD_LINE7, " SYN TIME ");




	/*timer init*/
	cmt_init();
	cmt_callback_set(CHANNEL_3, &serialinp);//create a timer task to check notification and inclination
	cmt_start(CHANNEL_3, TIMER_COUNT_100MS);//evry 100ms

	while (m.timeflag == false) //wait for time to be synced by the app
	{
	}

	notif = false;
	lcd_clear();
	parsertc(new_char); // initalize the hardware with the the time 

	memset(new_char, 0, 25);
	lcd_display(LCD_LINE7, " THANK YOU ");


	cmt_callback_set(CHANNEL_0, &delay);
	cmt_start(CHANNEL_0, TIMER_COUNT_700MS); //create a temproray delay  to make the Thank you message visible
	while (!delaycomplete)
	{

	}
	delaycomplete = false;

	lcd_clear();
	m.timeflag = false;  //clear the time flag






	lcd_display(LCD_LINE1, "  PLEASE  ");
	lcd_display(LCD_LINE2, "  SYN  ");

	lcd_display(LCD_LINE4, "YOUR DETAILS");


	while (m.detailflag == false) //wait for detail notification to be synced by the app
	{
	}

	notif = false;
	lcd_clear();
	parsedetail(new_char);
	memset(new_char, 0, 25);

	lcd_display(LCD_LINE7, " THANK YOU ");
	cmt_start(CHANNEL_0, TIMER_COUNT_700MS); //create a temproray delay  to make the Thank you message visible

	while (!delaycomplete)
	{

	}
	delaycomplete = false;
	lcd_clear();
	m.detailflag = false;  //clear the time flag



	while (1)
	{

		if (powerdown == 0)  // a statemachine which checks for power down due to inclination and various task
		{

			if (sw1 == false)
			{
				checkbuttonstate();
				switch (state)
				{
				case 0:
					lcd_display(LCD_LINE1, ">TIME & DATE");
					lcd_display(LCD_LINE2, "TEMPERATURE");
					lcd_display(LCD_LINE3, "STOPWATCH");
					lcd_display(LCD_LINE4, "GAME");
					lcd_display(LCD_LINE5, "SET ALARM");
					lcd_display(LCD_LINE6, "PULSE RATE");
					tdelay = TIMER_COUNT_100MS;
					fpoint = &DisplayTime;
					break;
				case 1:
					lcd_display(LCD_LINE1, "TIME & DATE");
					lcd_display(LCD_LINE2, ">TEMPERATURE");
					lcd_display(LCD_LINE3, "STOPWATCH");
					lcd_display(LCD_LINE4, "GAME");
					lcd_display(LCD_LINE5, "SET ALARM");
					lcd_display(LCD_LINE6, "PULSE RATE");
					tdelay = TIMER_COUNT_100MS;
					fpoint = &temperature_display;
					break;
				case 2:
					lcd_display(LCD_LINE1, "TIME & DATE");
					lcd_display(LCD_LINE2, "TEMPERATURE");
					lcd_display(LCD_LINE3, ">STOPWATCH");
					lcd_display(LCD_LINE4, "GAME");
					lcd_display(LCD_LINE5, "SET ALARM");
					lcd_display(LCD_LINE6, "PULSE RATE");
					tdelay = TIMER_COUNT_100MS;
					fpoint = &StopWatch_Display;
					break;

				case 3:
					lcd_display(LCD_LINE1, "TIME & DATE");
					lcd_display(LCD_LINE2, "TEMPERATURE");
					lcd_display(LCD_LINE3, "STOPWATCH");
					lcd_display(LCD_LINE4, ">GAME");
					lcd_display(LCD_LINE5, "SET ALARM");
					lcd_display(LCD_LINE6, "PULSE RATE");
					tdelay = TIMER_COUNT_100MS;
					fpoint = &Create_Obstacle;
					break;


				case 4:
					lcd_display(LCD_LINE1, "TIME & DATE");
					lcd_display(LCD_LINE2, "TEMPERATURE");
					lcd_display(LCD_LINE3, "STOPWATCH");
					lcd_display(LCD_LINE4, "GAME");
					lcd_display(LCD_LINE5, ">SET ALARM");
					lcd_display(LCD_LINE6, "PULSE RATE");
					tdelay = TIMER_COUNT_100MS;
					fpoint = &Set_Alarm;
					break;

				case 5:
					lcd_display(LCD_LINE1, "TIME & DATE");
					lcd_display(LCD_LINE2, "TEMPERATURE");
					lcd_display(LCD_LINE3, "STOPWATCH");
					lcd_display(LCD_LINE4, "GAME");
					lcd_display(LCD_LINE5, "SET ALARM");
					lcd_display(LCD_LINE6, ">PULSE RATE");

					tdelay = TIMER_COUNT_2MS;
					fpoint = &readheartsensor;
					break;


				case 6:
					state = 8;
					Speaker_Init();
					lcd_clear();
					lcd_display(LCD_LINE1, " ALERT ");
					if (m.messageflag == true)
					{
						m.messageflag = false;
						lcd_display(LCD_LINE4, "MESSAGE FROM");
						substring(tempchar, new_char, 2, 13);
						lcd_display(LCD_LINE7, tempchar);
						memset(new_char, 0, 25);
					}
					else
						if (m.callflag == true)
						{
							m.callflag = false;
							lcd_display(LCD_LINE4, "CALL FROM");
							substring(tempchar, new_char, 2, 13);
							lcd_display(LCD_LINE7, tempchar);
							memset(new_char, 0, 25);
						}
					break;

				case 7:
					state = 8;
					alarm = true;
					Speaker_Init();
					lcd_clear();
					lcd_display(LCD_LINE1, "ALARM ALARM");

					break;

				case 8:
					Speaker_ON();
					break;


				}

				for (unsigned int i = 0; i < 20000; i++);



			}
			else
			{
				lcd_clear();
				if (fpoint == &readheartsensor)  //readheartsensor task runs at 2ms very high refresh
				{                             // rate for LCD so  run another task for display pulse rate
					cmt_callback_set(CHANNEL_1, displaybpm);
					cmt_start(CHANNEL_1, TIMER_COUNT_50MS);

				}
				if (fpoint == &Create_Obstacle)  //game task intially display logo of top gun damn that looks cool
				{
					cmt_callback_set(CHANNEL_0, delay);
					cmt_start(CHANNEL_0, TIMER_COUNT_700MS);
					Set_Font_Bitmap();
					Set_LCD_Pos(0, 0);
					Set_LCD_Char(0);// top gun logo

					while (!delaycomplete);
					delaycomplete = false;
					lcd_clear();
				}
				sw1 = false;
				cmt_callback_set(CHANNEL_0, fpoint); // start the respective module 
				cmt_start(CHANNEL_0, tdelay);
				while ((sw1 == false) && (state != 6) && (state != 7) && (powerdown == false));//break if menu button is pressed or powerdown or alarm or notification occurs
				cmt_stop(CHANNEL_0);
				cmt_stop(CHANNEL_1);
				lcd_clear();
				Set_Font_8_by_8();
				sw1 = false;
				if (powerdown == 1) //if powerdown while task is running continue the task after wake up
				{
					taskcontinue = true;
				}

			}

		}

		else {

			cmt_stop(3);  // switch off the compare match timer and go to sleep

			power_modes_demo(); //power down ie go to sleep 
			powerdown = 0;
			cmt_start(CHANNEL_3, TIMER_COUNT_100MS);

			if ((taskcontinue == true) && (!((notif == true) ^ (alarm == true))))//if woken by alarm or notification then sound alarm until user dismisses the alarm
			{
				cmt_start(CHANNEL_0, tdelay);
				taskcontinue = false;
				sw1 = true;
			}

		}
	}
} /* End of function main() */







