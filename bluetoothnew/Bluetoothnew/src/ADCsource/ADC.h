
/*******************************************************************************
* File Name     : ADC.h
* Description  : Simple driver for S12ADC (12-bit A/D converter)
*******************************************************************************/

#ifndef _S12ADC_H_
#define _S12ADC_H_

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdbool.h>

/* Values for conversion of S12ADC counts to voltage */
#define MAX_COUNTS 4095.0
#define VREFH 3.3
#define VREFL 0.0

/******************************************************************************
Prototypes for exported functions
******************************************************************************/
void S12ADC_init (void) ;
void S12ADC_start (void) ;
bool S12ADC_conversion_complete (void) ;
unsigned int S12ADC_read (void) ;

#endif
