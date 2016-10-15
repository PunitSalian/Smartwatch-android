
/******************************************************************************
* File Name	   : ADXL345.h
* Description   : Declarations for ADXL345 accelerometer
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef IIC_ADXL345_H
#define IIC_ADXL345_H

#define ADXL345_ADDR                0x3A
#define ADXL345_DEVICE_ID           0xE5    

#define ADXL345_ID_REG              0x00
#define ADXL345_THRESH_TAP_REG      0x1D
#define ADXL345_OFSX_REG            0x1E
#define ADXL345_OFSY_REG            0x1F
#define ADXL345_OFSZ_REG            0x20
#define ADXL345_DUR_REG             0x21
#define ADXL345_LATENT_REG          0x22
#define ADXL345_WINDOW_REG          0x23
#define ADXL345_THRESH_ACT_REG      0x24
#define ADXL345_THRESH_INACT_REG    0x25
#define ADXL345_TIME_INACT_REG      0x26
#define ADXL345_ACT_INACT_CTL_REG   0x27
#define ADXL345_THRESH_FF_REG       0x28
#define ADXL345_TIME_FF_REG         0x29
#define ADXL345_TAP_AXES_REG        0x2A
#define ADXL345_ACT_TAP_STATUS_REG  0x2B
#define ADXL345_BW_RATE_REG         0x2C
#define ADXL345_POWER_CTL_REG       0x2D
#define ADXL345_INT_ENABLE_REG      0x2E
#define ADXL345_INT_MAP_REG         0x2F
#define ADXL345_INT_SOURCE_REG      0x30
#define ADXL345_DATA_FORMAT_REG     0x31
#define ADXL345_DATAX0_REG          0x32
#define ADXL345_DATAX1_REG          0x33
#define ADXL345_DATAY0_REG          0x34
#define ADXL345_DATAY1_REG          0x35
#define ADXL345_DATAZ0_REG          0x36
#define ADXL345_DATAZ1_REG          0x37
#define ADXL345_FIFO_CTL_REG        0x38
#define ADXL345_FIFO_STATUS_REG     0x39


// scale factor 3.3V for self test
#define SCALE_X(x)                (113*x)/64    // 1.77
#define SCALE_Y(y)                (113*y)/64    // 1.77
#define SCALE_Z(z)                ( 94*z)/64    // 1.47

/******************************************************************************
* Global Function Prototypes
******************************************************************************/

/* End of multiple inclusion prevention macro */
#endif
