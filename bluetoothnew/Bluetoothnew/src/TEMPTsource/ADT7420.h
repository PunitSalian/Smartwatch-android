
/******************************************************************************
* File Name     : ADT7420.h
* Description   : Declarations for ADT7420 temperature sensor.
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#ifndef IIC_ADT7420_H               /* Multiple inclusion prevention macro */
#define IIC_ADT7420_H

#define ADT7420_ADDR                0x90
#define ADT7420_DEVICE_ID           0xC8    // bits 7:3 of mfg identification number

#define ADT7420_TEMP_MSB_REG        0x00
#define ADT7420_TEMP_LSB_REG        0x01
#define ADT7420_STATUS_REG          0x02
#define ADT7420_CONFIG_REG          0x03
#define ADT7420_T_HIGH_MSB_REG      0x04
#define ADT7420_T_HIGH_LSB_REG      0x05
#define ADT7420_T_LOW_MSB_REG       0x06
#define ADT7420_T_LOW_LSB_REG       0x07
#define ADT7420_T_CRIT_MSB_REG      0x08
#define ADT7420_T_CRIT_LSB_REG      0x09
#define ADT7420_HIST_REG            0x0A
#define ADT7420_ID_REG              0x0B
#define ADT7420_RESET_REG           0x2F

/******************************************************************************
* Global Function Prototypes
******************************************************************************/



/* End of multiple inclusion prevention macro */
#endif
