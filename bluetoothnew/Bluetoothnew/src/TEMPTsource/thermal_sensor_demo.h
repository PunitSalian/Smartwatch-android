
/******************************************************************************
* File Name    : thermal_sensor_demo.h
* Description  : API for thermal sensor
******************************************************************************/

#ifndef THERMAL_SENSOR_H   /* Multiple inclusion prevention. */
#define THERMAL_SENSOR_H

/******************************************************************************
Public functions prototypes
******************************************************************************/
riic_ret_t thermal_sensor_init(void);
int16_t thermal_sensor_read(void);
void temperature_display(void);

#endif /* Multiple inclusion prevention. */

