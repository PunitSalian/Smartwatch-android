
/*******************************************************************************
* File Name     : accelerometr_demo.c
* Description   : api for acceleromter operation
*                  
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Defines string functions used in this file */
#include <string.h>
#include <stdio.h>
#include <machine.h>

#include "platform.h"
#include "I2Csource/r_riic_rx600.h"
#include "I2Csource/r_riic_rx600_master.h"
#include "I2Csource/riic_master_main.h"
#include "accelerometer_demo.h"

/* Defines ADXL345 parameters */
#include "ADXL345.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define ACCELEROMETER_DEBUG

/*******************************************************************************
Local global variables
*******************************************************************************/
static volatile int16_t g_accel_x_zero;
 volatile int16_t g_accel_y_zero;
static volatile int16_t g_accel_z_zero;
static volatile int16_t g_accel_x;
static volatile int16_t g_accel_y;
static volatile int16_t g_accel_z;


/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

static bool   accel_selftest( void );
static riic_ret_t accelerometer_write (uint8_t riic_channel,
                                uint8_t slave_addr,
                                uint8_t register_number, 
                                uint8_t *source_buff, 
                                uint32_t num_bytes);
                                
static riic_ret_t accelerometer_read (uint8_t riic_channel,
                               uint8_t slave_addr,
                               uint8_t register_number, 
                               uint8_t *dest_buff, 
                               uint32_t num_bytes);
                               

/*******************************************************************************
* Function name: accelerometer_write
* Description  : Writes a specified number of bytes starting from the specfied 
*                accelerometer register. If more than 1 byte is requested then
*                the accelerometer will automatically increment to the next 
*                register number with each sequencial write.
* Arguments    : riic_channel - 
*                   Which IIC channel of the MCU to use.
*                slave_addr -
*                   IIC slave address of the accelerometer.
*                register_number - 
*                   Which register of the accelerometer to be written.
*                (uint8_t*)source_buff - 
*                   pointer to the buffer where data will be copied from.              
*                num_bytes -
*                   The number of bytes to be written
*
* Return value : ret : RIIC result_code
*******************************************************************************/
static riic_ret_t accelerometer_write (uint8_t riic_channel,
                                uint8_t slave_addr,
                                uint8_t register_number, 
                                uint8_t *source_buff, 
                                uint32_t num_bytes)
{
    uint8_t     addr_and_register[2]; /* Storage for the slave address and target register. */
    riic_ret_t  ret = RIIC_OK;
 
    /* To write to a specific register in the accelerometer, first transmit the 
       accelerometer I2C slave address together with the register number. */         
    addr_and_register[0] = slave_addr;    /* The 7-bit I2C address of the ADXL345 and the R/W bit. */ 
    addr_and_register[1] = register_number;
               
    ret |= R_RIIC_MasterTransmitHead(CHANNEL_0, addr_and_register, 2);
    /* Now write the data from the source buffer into the target register. */
    ret |= R_RIIC_MasterTransmit(CHANNEL_0, source_buff, num_bytes);
    
    return ret;
} /* End of function accelerometer_write(). */


/******************************************************************************
* Function name: accelerometer_read
* Description  : Reads a specified number of bytes starting from the specfied 
*                accelerometer register. If more than 1 byte is requested then
*                the accelerometer will automatically increment to the next 
*                register number with each sequencial read.
* Arguments    : riic_channel - 
*                   Which IIC channel of the MCU to use.
*                slave_addr -
*                   IIC slave address of the accelerometer.
*                register_number - 
*                   Which register of the accelerometer to be written.
*                (uint8_t*)dest_buff - 
*                   pointer to the buffer into which the read data will be stored.              
*                num_bytes -
*                   The number of bytes to be read from the accelerometer
* 
* Return value : ret : RIIC result_code
******************************************************************************/
static riic_ret_t accelerometer_read (uint8_t riic_channel,
                               uint8_t slave_addr,
                               uint8_t register_number, 
                               uint8_t *dest_buff, 
                               uint32_t num_bytes)
{ 
    uint8_t     addr_and_register[2]; /* Storage for the slave address and target register. */
    riic_ret_t  ret = RIIC_OK;
    
    /* To read from a specific register in the accelerometer, first transmit the 
       accelerometer I2C slave address together with the register number. */         
    addr_and_register[0] = slave_addr; /* The 7-bit I2C address of the ADXL345 and the R/W bit. */ 
    addr_and_register[1] = register_number;
            
    ret |= R_RIIC_MasterTransmitHead(CHANNEL_0, addr_and_register, 2);
    /* Now read the data from the target register into the destination buffer. */    
    ret |= R_RIIC_MasterReceive(CHANNEL_0, slave_addr, dest_buff, num_bytes);
    
    return ret;    
} /* End of function accelerometer_read(). */


/******************************************************************************
* Function name:  accelerometer_init
* Description  : 
* Argument     : none
* Return value : riic_ret_t : RIIC result code
*******************************************************************************/
riic_ret_t accelerometer_init(void)
{
    bool            err = true;    /* Declare error flag */
    uint8_t         target_data; 
    riic_ret_t      ret;

    /* Read the DEVID register to verify the presence of the accelerometer device. */    
    ret |= accelerometer_read(RIIC_CHANNEL, ADXL345_ADDR, ADXL345_ID_REG, &target_data, 1);
                                                               
    if ((target_data != ADXL345_DEVICE_ID) || (RIIC_OK != ret))
    {   /* Add to error return information here if desired. */
        return ret;
    }

    /* Set up the accelerometer data format register. */
    target_data = 3; /* Set data format register range bits to +/- 16g. */
    ret |= accelerometer_write(RIIC_CHANNEL, ADXL345_ADDR, ADXL345_DATA_FORMAT_REG, &target_data, 1);

    /* Put accelerometer FIFO into bypass mode. */
    target_data = 0; /* FIFO bypass mode. */
    ret |= accelerometer_write(RIIC_CHANNEL, ADXL345_ADDR, ADXL345_FIFO_CTL_REG, &target_data, 1);

    /* Set the measure bit in the accelerometer power control register. */                                     
    target_data = 8; /* Measure bit. */
    ret |= accelerometer_write(RIIC_CHANNEL, ADXL345_ADDR, ADXL345_POWER_CTL_REG, &target_data, 1);                                   

    /* Get baseline readings to calibrate accelerometer. */
    g_accel_x_zero = 0;
    g_accel_y_zero = 0;
    g_accel_z_zero = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        g_accel_x_zero += accel_axis_read(0x32);
        g_accel_y_zero += accel_axis_read(0x34);
        g_accel_z_zero += accel_axis_read(0x36);            
    }

    /* Determine the average reading. */
    g_accel_x_zero = g_accel_x_zero / 8;
    g_accel_y_zero = g_accel_y_zero / 8;
    g_accel_z_zero = g_accel_z_zero / 8;

    /* Run self test to see if the accelerometer is working. */
    err &= accel_selftest();

    /* Activate accelerometer X, Y Z to detect activity. */
    target_data = 0x70;    
    ret |= accelerometer_write(RIIC_CHANNEL, ADXL345_ADDR, ADXL345_ACT_INACT_CTL_REG, &target_data, 1);  

    LED4 = LED5 = LED6 = LED7 = LED8 = LED9 = LED10 = LED11 = LED12 = LED13 = LED14 = LED15 = LED_OFF;

    return ret;
} /* End of function accelerometer_init(). */




/*******************************************************************************
* Function name: accel_selftest
* Description  : This function invokes the acceleromter's self-test feature.
*                Done to check if the unit is working by testing its electrical
*                and mechanical systems.
* Argument     : none
* Return value : bool   true - 
*                           pass 
*                       false - 
*                           fail
*******************************************************************************/
static bool  accel_selftest( void )
{
    uint8_t     target_data;
    riic_ret_t  ret;           
    bool        err = true;
 
    /* Set up the accelerometer data format register. */    
    target_data = 0x83;  /* Set the data format range bits to +/- 16g, and selftest mode bit. */
    ret |= accelerometer_write(RIIC_CHANNEL, ADXL345_ADDR, ADXL345_DATA_FORMAT_REG, &target_data, 1);

    /* Wait for self-test forces to act. */
    for (volatile uint16_t i = 0; i < 1000; i++)
    {
        ; /* Spin loop delay. */
    }

    g_accel_x = 0;
    g_accel_y = 0;
    g_accel_z = 0;

    /* Take an average of 8 readings per axis to serve as basis for range check. */
    for (uint16_t i = 0; i < 8; i++)
    {
        g_accel_x += accel_axis_read(ADXL345_DATAX0_REG);
        g_accel_y += accel_axis_read(ADXL345_DATAY0_REG);
        g_accel_z += accel_axis_read(ADXL345_DATAZ0_REG);        
    }
    /* Divide the 8 readings by 8 to obtain the average value. */
    g_accel_x = g_accel_x / 8;
    g_accel_y = g_accel_y / 8;
    g_accel_z = g_accel_z / 8;

    /* Normalize the self test values. */
    g_accel_x = SCALE_X(g_accel_x) - g_accel_x_zero;
    g_accel_y = SCALE_Y(g_accel_y) - g_accel_y_zero;
    g_accel_z = SCALE_Z(g_accel_z) - g_accel_z_zero;

    /* Range check self-test values. */
    err &= ((g_accel_x >   6) && (g_accel_x < 67))  ? true : false;
    err &= ((g_accel_y > -67) && (g_accel_y < -6))  ? true : false;
    err &= ((g_accel_z >  10) && (g_accel_z < 110)) ? true : false;

    /* Turn off self test mode. */                          
    target_data = 0x03;  /* Set the data format range bits to +/- 16g, and clear selftest mode bit. */
    ret |= accelerometer_write(RIIC_CHANNEL, ADXL345_ADDR, ADXL345_DATA_FORMAT_REG, &target_data, 1);
                                 
    if(ret)
    {
        err = false; /* Fail. */   
    }                             
    return err;                                 
} /* End of function accel_selftest(). */


/******************************************************************************
* Function name: accel_axis_read
* Description  : This function reads the accelerometer's X, Y or Z axis. 
* Argument     : uint8_t    axis -
*                   Which axis register to be read.
* Return value : short value of axis
*******************************************************************************/
 int16_t  accel_axis_read (uint8_t axis)
{
    int16_t    axis_val;    /* 16-bit storage for the x, y, or z axis data. */
    uint8_t    accel_reg;   /* Storage for the accelerometer register parameter. */
    uint8_t    accel_data[2];
    riic_ret_t ret;         /* Result code from the RIIC API functions. */
    
    accel_reg = axis;

    /* Uses RIIC to read the accelerometer axis data. */ 
    ret = accelerometer_read(RIIC_CHANNEL, ADXL345_ADDR, accel_reg, accel_data, 2);
    
    while (RIIC_OK != ret)
    {
        nop(); /* Stay here for debug of IIC error. */    
    }
    
    axis_val    = accel_data[1] << 8;
    axis_val   += accel_data[0];

    return axis_val;
} /* End of function accel_axis_read(). */

