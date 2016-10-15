
/***********************************************************************************************************************
* File Name    : r_switches.h
* Description  : Functions for using switches with callback functions. 
************************************************************************************************************************/

#ifndef SWITCHES_API_HEADER_FILE
#define SWITCHES_API_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Used for configuring the code */
#include "r_switches_config.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Version Number of API. */
#define SWITCHES_VERSION_MAJOR           (1)
#define SWITCHES_VERSION_MINOR           (0)

/***********************************************************************************************************************
Public Functions
***********************************************************************************************************************/
void R_SWITCHES_Init(void);

/* Callback prototypes. */
#if defined(SW1_CALLBACK_FUNCTION)
void SW1_CALLBACK_FUNCTION(void);
#endif

#if defined(SW2_CALLBACK_FUNCTION)
void SW2_CALLBACK_FUNCTION(void);
#endif 

#if defined(SW3_CALLBACK_FUNCTION)
void SW3_CALLBACK_FUNCTION(void);
#endif

#endif /* SWITCHES_API_HEADER_FILE */

