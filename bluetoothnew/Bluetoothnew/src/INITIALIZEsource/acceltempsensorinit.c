/*******************************************************************************
* File Name     : acceltempsensorinit.c
* Description   : initialize I2C for accel and temp sensor
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/

#include <machine.h>
#include  <stdbool.h>
#include "I2Csource/r_riic_rx600.h"
#include "I2Csource/r_riic_rx600_master.h"
#include "accelerometersource/accelerometer_demo.h"
#include "I2Csource/riic_master_main.h"
#include "TEMPTsource/thermal_sensor_demo.h"



/*******************************************************************************
* Function name: riic_master_init
* Description  : Prepare an RIIC channel for master mode communications used in the application
* Arguments    : none
* Return value : riic_ret_t -
*                   RIIC result code
*******************************************************************************/
static riic_ret_t riic_master_init(void) 
{ 
    /* Initialize a data structure used by the R_RIIC_Init() API function. */
    /* See r_riic_rx600.h for definitions of this structure. */
    riic_config_t   riic_master_config = {RIIC_CHANNEL, RIIC_MASTER_CONFIG, 
                                          0, 
                                          0, 
                                          0, 
                                          0, 
                                          MASTER_IIC_ADDRESS_LO, 
                                          MASTER_IIC_ADDRESS_HI}; 

    /* Initialize the RIIC channel for communication with the accelerometer. */
    return R_RIIC_Init(&riic_master_config);
}

/******************************************************************************
  * Function Name	: acceltempsensorinit
  * Description	    : Initialization of I2C sensors acclemeter and temperature senor  .
  * Arguments       : None
  * Return value	: The byte count.
*******************************************************************************/

void acceltempsensorinit()
{
	/* Declare and initialize the IIC result code. */
	riic_ret_t iic_ret = RIIC_OK;	
	
		while (RIIC_OK != iic_ret)
	{
		nop(); /* Failure to initialize here means demo can not proceed. */
	}
	
		/* Some boards may not have the thermal sensor present. This sequence
	demonstrates an example of how to recover from some bus error conditions. */
	iic_ret |= thermal_sensor_init();    /* Initialize IIC thermal sensor */

	if (RIIC_OK == iic_ret)
	{
	nop();
	}
	/* Thermal sensor not present or malfunctioning. Remove it from the demo. */
	else
	{   /* Got a NACK. */
	

		iic_ret = R_RIIC_Reset(RIIC_CHANNEL); /* Do soft reset to clean up bus states. */

		if (RIIC_RESET_ERR & iic_ret) /* Check for successful IIC soft-reset. */
		{   /* Soft-reset failed. Need to do complete re-initialization. */
			/* Need to release the channel first berfore it can be re-initialized. */
			R_RIIC_ReleaseChannel(RIIC_CHANNEL);
			iic_ret = riic_master_init();
		}

		while (RIIC_OK != iic_ret)
		{
			nop(); /* Failure to initialize here means demo can not proceed. */
		}
	}
	
		/* Prepare the accelerometer  */
	accelerometer_init();
	

	
	
}