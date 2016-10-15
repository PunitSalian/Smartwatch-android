
/*******************************************************************************
* File Name     : topgun.c
* Description   : api for the topgun game.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdbool.h>
#include <stdlib.h>
#include "platform.h"
#include "CMTTIMERsource/cmt_periodic_multi.h"
#include "topgun.h"



/*******************************************************************************
* Prototypes for global variables
*******************************************************************************/
volatile int valx_1 = 0; //move the enemy jet
volatile int valy_1 = 0;//move the enemy jet
volatile int valx = 0; //move the user  jet
volatile int valy = 0; //move the user  jet
volatile bool enemy_flag = 0; //variable for creating enemy plane
volatile bool hit_flag = 0; //variable for tracking hits and hit animation
volatile bool gameoverflag = 0;

/*************external function prototypes and variables**************/
extern volatile bool sw1, sw2, sw3;




/******************************************************************************
* Function name: Create_Obstacle
* Description  : spawns enemy jet and checks for the switch state to move the user jet
* Arguments    : none
* Return value : none
******************************************************************************/
void Create_Obstacle(void)
{

	Set_Font_Bitmap();
	lcd_clear();

	/*--------------USER JET----START------------*/
	if ((sw2 == true) && (sw3 != true))
	{
		valy = valy - 8;					// move up
											//lcd_clear();
		sw2 = false;
		if (valy < 0)
			valy = 0;
	}
	else if ((sw2 != true) && (sw3 == true))
	{
		valy = valy + 8;					// move down
											//lcd_clear();
		sw3 = false;
		if (valy > 0x32)
			valy = 0x32;
	}




	/*---------------ENEMY AND JET SPAWNING----START------------*/

	if (enemy_flag == 0)
	{ 	//create enemy plane
		valx_1 = 80; //spawn on the other side of the screen
		valy_1 = 8 * (rand() % 8);
		enemy_flag = 1;
	}
	else
	{
		if (valx_1 <= 23) //check for collision
		{
			if ((valy_1 < (valy + 16)) && (valy_1 >= valy))
			{
				enemy_flag = 0; //respawn enemy
				hit_flag = 1;
			}
		}
		if (valx_1 == 0)
			enemy_flag = 0;
		else
		{
			valx_1 = valx_1 - 5;
			if (hit_flag == 0)
			{
				Set_LCD_Pos(valx_1, valy_1);
				Set_LCD_Char(7); //draw enemy jet
			}
			else
			{
				Set_LCD_Pos(valx_1, valy_1);
				Set_LCD_Char(7);
				Set_LCD_Pos(valx, valy);
				Set_LCD_Char(1);
				hit_flag = 1;
			}
		}
	}
	/*---------------ENEMY AND JET SPAWNING ROUTINE----END------------*/


	if (hit_flag == 0)
	{
		Set_LCD_Pos(valx, valy);
		Set_LCD_Char(3); //draw player jet
	}
	else
	{
		Set_LCD_Pos(valx, valy);
		Set_LCD_Char(3);
		Set_LCD_Pos(valx, valy);
		Set_LCD_Char(1);
		hit_flag = 1;
	}

	if (hit_flag == 1)
	{
		lcd_clear();
		Set_Font_8_by_8();
		lcd_display(LCD_LINE3, "    GAME     ");
		lcd_display(LCD_LINE4, "    OVER!    ");
		Set_Font_8_by_8();
		valx_1 = 0;
		valy_1 = 0;
		valx = 0;
		valy = 0;
		enemy_flag = 0; //variable for creating enemy plane
		hit_flag = 0;
		cmt_stop(CHANNEL_0);
	}// end if - game over



} /* End of Create_Obstacle() */