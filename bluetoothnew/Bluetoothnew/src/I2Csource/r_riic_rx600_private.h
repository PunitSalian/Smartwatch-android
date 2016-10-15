
/*******************************************************************************
* File Name    : r_riic_rx600_private.h
 
* Description  : Private macro definitions for the RIIC driver API.
*******************************************************************************/


#ifndef RIIC_RX600_PRIVATE_H /* Multiple inclusion protection. */
#define RIIC_RX600_PRIVATE_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include "platform.h"
#include "r_riic_rx600_types.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* Check whether user has correctly specified an MCU group */
#if (MCU_RX62N + MCU_RX630 + MCU_RX63N) < 1
    #error "ERROR: You must specify an RX6xx Device in r_riic_rx600_config.h"		
#endif

#if (MCU_RX62N + MCU_RX630 + MCU_RX63N) > 1
    #error "ERROR: Only one RX6xx Device can be defined at a time in r_riic_rx600_config.h"		
#endif

/* Check whether user has correctly specified an MCU package */
#if (PACKAGE_LFBGA176 + PACKAGE_LQFP176 + PACKAGE_LQFP144 + PACKAGE_LQFP100 + PACKAGE_TFLGA85 + PACKAGE_LQFP80) < 1
    #error "ERROR: You Must specify a valid package in r_riic_rx600_config.h"		
#endif

#if (PACKAGE_LFBGA176 + PACKAGE_LQFP144 + PACKAGE_LQFP100 + PACKAGE_TFLGA85 + PACKAGE_LQFP80) > 1
    #error "ERROR: Only one device package can be defined at a time in r_riic_rx600_config.h"		
#endif


/* Use only these channel definitions when specifying an RIIC channel number */
/* This translates physical channel to an index used by the driver           */
/* CHANNEL_0, CHANNEL_1, CHANNEL_2, CHANNEL_3                                */
#define CHANNEL_0 0  /* Valid for all configurations */
#if defined(MCU_RX62N)
    #if defined(PACKAGE_LFBGA176) || defined(PACKAGE_LQFP144) || defined(PACKAGE_TFLGA85)
        #define RIIC_NUM_CHANNELS   2  /* Number of RIIC channels this MCU has. */
        #define CHANNEL_1           1  /* Channel arrays index */ 
          
    #elif defined(PACKAGE_LQFP100)
        #define RIIC_NUM_CHANNELS   1  /* Number of RIIC channels this MCU has. */
        
    #else
        #error "Must specify a valid package!"    
    #endif
                   
#elif defined(MCU_RX630) || defined(MCU_RX63N)
    #if defined(PACKAGE_LQFP176) || defined(PACKAGE_LQFP144)
        #define RIIC_NUM_CHANNELS   4   /* Number of RIIC channels this MCU has. */
        #define CHANNEL_1           1   /* Channel arrays index. */
        #define CHANNEL_2           2   /* Channel arrays index. */
        #define CHANNEL_3           3   /* Channel arrays index. */
         
    #elif defined(PACKAGE_LQFP100) || defined(PACKAGE_LQFP80)
        #define RIIC_NUM_CHANNELS   2   /* Number of RIIC channels this MCU has. */
        #define CHANNEL_2           1   /* Channel arrays index. */
        
    #else
        #error "Must specify a valid package!"             
    #endif    
#endif
/* Because iodefines.h with different MCUs may use different labels, 
   re-map these iodefines macros as needed*/
#define X_IR( x , y ) IR( ## x , y )
#define X_IEN( x , y ) IEN( ## x , y )

#define X_IPR( x , y ) IPR( ## x , y )
#define X_VECT( x , y ) VECT( ## x , y )

#if defined(MCU_RX62N)
    #define EEI0    ICEEI0
    #define	RXI0    ICRXI0
    #define	TXI0    ICTXI0
    #define	TEI0    ICTEI0
    #define	EEI1    ICEEI1
    #define	RXI1    ICRXI1
    #define	TXI1    ICTXI1
    #define	TEI1    ICTEI1         
#endif

/*******************************************************************************
Private declarations of global variables to be imported by API functions.
*******************************************************************************/
/* Used to prevent having duplicate code for each channel. This only works
   if the channels are identical (just at different locations in memory). This
   is easy to tell by looking in iodefine.h and seeing if the same structure
   was used for all channels. */
#if defined(MCU_RX62N)
extern volatile struct st_riic __evenaccess* g_riic_channels[];
#elif defined(MCU_RX630) || defined(MCU_RX63N)
extern volatile struct st_riic0 __evenaccess* g_riic_channels[];
#endif

/* Global RIIC mode and state variables. */
extern volatile i2c_mode_t  g_riic_mode[];
extern volatile i2c_state_t g_riic_state[];


/*******************************************************************************
Private functions Prototypes
*******************************************************************************/
/* Intended to be used by driver only to prevent reentrancy problems. */
riic_ret_t riic_unlock(uint8_t channel); /* Do not call from user application. */
riic_ret_t riic_lock(uint8_t channel);   /* Do not call from user application. */

#endif /* RIIC_RX600_PRIVATE_H */



