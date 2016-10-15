

/*******************************************************************************
* File Name     : power_modes_demo.c
* Description   : api to enter in sleep mode
*******************************************************************************/


/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <machine.h>
#include <stdbool.h>
#include "platform.h"
#include "mcu_mode.h"


/*******************************************************************************
Macro definitions
*******************************************************************************/
#define PSW_USER_MODE_BIT  0x100000


extern long debounce;
long lastdebounce1 = 0;
long lastdebounce2 = 0;
long lastdebounce3 = 0;
volatile bool sw1 = false, sw2 = false, sw3 = false;
extern bool powerdown;
/*******************************************************************************
* Prototypes for local functions
*******************************************************************************/
void sleep_mode(void);
void pmdemo_delay();


/******************************************************************************
* Function name: power_modes_demo
* Description  :
* Arguments    : none
* Return value : none
******************************************************************************/
void power_modes_demo(void)
{
	/* Processor status word */
	uint32_t    psw_value;



	PORT2.PODR.BYTE &= ~(0x02);

	lcd_clear();

	/* To switch to low power modes, we need to:
	1. Change to supervisor mode. The function to do this is defined in mcu_mode.src
	2. Disable interrupts globally (they'll be re-enabled by the wait instruction
	3. Clear pending interrupts for all switches
	4. Enable interrupts for switches in ICU
	5. Setup for entry into sleep/standby/deep standby mode
	6. Enter the low power state
	*/
	psw_value = get_psw();      /* Save the current Processor Status Word. */

								/* If currently in user mode then switch to supervisor mode. */
	if (psw_value & PSW_USER_MODE_BIT)
	{
		chg_pmsuper();          /* 1. Change to supervisor mode */
	}

	clrpsw_i();                 /* 2. Disable interrupts globally (intrinsic function)*/




	sleep_mode();            /* Sleep   mode exited by IRQ interrupt on SW1-3 */





	wait();                     /* 6. Enter the selected low power state */

								/* When the wait instruction is executed, the global interrupt flag is set
								to enable interrupts that were disabled in step 2 above. The MCU is then
								switched into low power mode. It can be returned to the run state by a
								number of interrupt sources depending on the lowpower mode.
								The code below this point executes after the MCU has woken up from the
								low power mode.
								*/

								/* If previous mode was user mode then switch back to it. */
	if (psw_value & PSW_USER_MODE_BIT)
	{
		chg_pmusr();            /* Switch back to user mode (intrinsic function) */
	}

	PORT2.PODR.BYTE |= (0x02);










} /* End of function power_modes_demo() */


  /******************************************************************************
  * Function name: sleep_mode
  *
  * Description  : Configure the MCU for sleep mode
  *
  *                In sleep mode, the CPU stops operating but the contents of its
  *                internal registers are retained. Other peripheral functions do
  *                not stop. When the WDT is used, the WDT stops counting when
  *                sleep mode is entered.
  *                Sleep mode is entered after the next WAIT instruction is executed.
  *
  *                Sleep mode is canceled by:
  *                    any interrupt,
  *                    the reset signal from the RES# pin,
  *                    a power-on reset,
  *                    a voltage monitoring reset,
  *                    or a reset caused by an IWDT underflow.
  *
  * Arguments    : none
  * Return value : none
  ******************************************************************************/
void sleep_mode(void)
{
	/* Shifts to sleep mode or all-module clock stop mode after the WAIT
	instruction is executed. */
	SYSTEM.SBYCR.BIT.SSBY = 0;

	/* Force a read of the register to allow operation to complete before moving on. */
	if (SYSTEM.SBYCR.WORD)
	{
		nop();
	}
} /* End of function () */






/******************************************************************************
* Function name: sw3_callback
* Description  : Callback function that is executed when SW3 is pressed.
*                Called by sw3_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/

void sw3_callback(void)
{
	if (powerdown != 1)
		if (debounce - lastdebounce3>2)
		{
			sw3 = true;
			lastdebounce3 = debounce;
		}

}



/******************************************************************************
* Function name: sw2_callback
* Description  : Callback function that is executed when SW12 is pressed.
*                Called by sw2_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw2_callback(void)
{
	if (powerdown != 1)
		if (debounce - lastdebounce2>2)
		{
			sw2 = true;
			lastdebounce2 = debounce;
		}
}

/******************************************************************************
* Function name: sw1_callback
* Description  : Callback function that is executed when SW1 is pressed.
*                Called by sw1_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/

void sw1_callback(void)
{
	if (powerdown != 1)
		if (debounce - lastdebounce1>2) //wait for debounce period
		{
			sw1 = true;
			lastdebounce1 = debounce;
		}

}

