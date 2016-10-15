
/*******************************************************************************
* File Name		: r_riic_rx600_config.h
* Description   : Developer configuration file for RX600 family RIIC driver APIs
*******************************************************************************/

#ifndef RIIC_CONFIG_H
#define RIIC_CONFIG_H

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* Default queue size definitions. This space will be allocated X number of 
   RIIC channels. Minimize these if application will use its own queue buffers */
#define TX_BUF_LEN 16  // Interrupt-mode transmit queue buffer size 
#define RX_BUF_LEN 16  // Interrupt-mode receive queue buffer size

                

/* Possible values for PCLK_DIVISOR:
    0: PCLK/1 
    1: PCLK/2 
    2: PCLK/4 
    3: PCLK/8 
    4: PCLK/16 
    5: PCLK/32 
    6: PCLK/64 
    7: PCLK/128 */
/* Nominal settings for 100kbps */    
#define PCLK_DIVISOR            3   /* PCLK/8 */   
#define BITRATE_HIGH_PERIOD     23
#define BITRATE_LOW_PERIOD      28

/* Defines the maximium rise time of the SCL line, in nanoseconds */
#define SCL_RISE_TIME               300
/* Defines the maximum fall time of the SCL line, in nanoseconds */
#define SCL_FALL_TIME               300


/* For operating at clock rates greater than 400kbps the fm+ (fast mode+) 
   spec must be used. There is a control bit FMPE (Fast-mode Plus Enable) 
   in the RIIC register ICFER (I2C Bus Function Enable Register) that 
   must be set to 1. Use this define to enable the FMPE bit. */
// #define USE_FASTMODE_PLUS


/* Define this to enable internal port pin pull-up resistors on RIIC bus lines. */
/* Otherwise, leave it undefined to use only external pull-up resistors.  */
// #define USE_PULLUP 


/* Slave Wait function.
   Waiting will hold the SCL line low while the receive data register (ICDRR)
   is full after each byte received. When ICDRR is read, the SCL line is released. 
   This allows the slave to perform flow control if its receive queue becomes full
   and it can't process the received data soon enough before master wants to 
   send more data.
   
   If wait function is disabled, then the RIIC will continue to receive another 
   byte in its receive shift register (ICDRS) for double-buffering continuous 
   read operation. If both the ICDRR and ICDRS bcome full then SCL will be held low 
   before ACK is sent. */

#define WAIT_OPTION_ON  // Define this to enable receive operation in byte units.


/* Define an interrupt priority. Adjust as needed */                           
#define RIIC_INT_PRIO   7

/*******************************************************************************
Exported Functions Prototypes (accessible by other modules)
*******************************************************************************/
      
#endif /* RIIC_CONFIG_H */
