
/*******************************************************************************

* Description  : Definitions and declarations supporting the RIIC driver API.
*******************************************************************************/


#ifndef RIIC_RX600_H /* Multiple inclusion protection. */
#define RIIC_RX600_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"

/* User specific RIIC configuration options */
#include "r_riic_rx600_config.h"

/* Private macro definitions and function prototypes. */
#include "r_riic_rx600_private.h" 

/* Type definitions for the RIIC driver API. */ 
#include "r_riic_rx600_types.h"


/*******************************************************************************
Macro definitions
*******************************************************************************/

/* Use only these channel definitions in application when specifying an       */ 
/* RIIC channel number: CHANNEL_0, CHANNEL_1, CHANNEL_2, CHANNEL_3            */
/* This translates physical channel to an index used by the driver            */


/*******************************************************************************
API functions Prototypes
*******************************************************************************/
riic_ret_t R_RIIC_Init(riic_config_t * settings);
riic_ret_t R_RIIC_ReleaseChannel(uint8_t channel);
riic_ret_t R_RIIC_Reset(uint8_t channel);   

#endif /* RIIC_RX600_H */



