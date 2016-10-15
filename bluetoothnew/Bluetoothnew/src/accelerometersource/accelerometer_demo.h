
/******************************************************************************
* File Name    : accelerometer_demo.h

* Description  : API for accelerometer
******************************************************************************/


#include "I2Csource/r_riic_rx600.h"

#ifndef ACCELEROMETER_DEMO_H   /* Multiple inclusion prevention. */
#define ACCELEROMETER_DEMO_H


/******************************************************************************
Public functions prototypes
******************************************************************************/
riic_ret_t accelerometer_init(void);
int16_t  accel_axis_read (uint8_t axis);
#endif /* Multiple inclusion prevention. */