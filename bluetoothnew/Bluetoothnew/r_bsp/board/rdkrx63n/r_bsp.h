
/***********************************************************************************************************************
* File Name	   : r_bsp.h
* Description  : Has the header files that should be included for this platform.
***********************************************************************************************************************/

#ifndef PLATFORM_BOARD_RDKRX63N
#define PLATFORM_BOARD_RDKRX63N

/* Make sure that no other platforms have already been defined. Do not touch this! */
#ifdef  PLATFORM_DEFINED
#error  "Error - Multiple platforms defined in platform.h!"
#else
#define PLATFORM_DEFINED
#endif

/***********************************************************************************************************************
INCLUDE APPROPRIATE MCU AND BOARD FILES
***********************************************************************************************************************/
#include    ".\mcu\rx63n\iodefine.h"
#include    ".\board\rdkrx63n\yrdkrx63n.h"
#include    ".\board\rdkrx63n\mcu_info.h"
#include    ".\board\rdkrx63n\hwsetup.h"
#include    ".\board\rdkrx63n\lcd.h"
#include    ".\board\rdkrx63n\sbrk.h"

#endif /* PLATFORM_BOARD_RDKRX63N */


