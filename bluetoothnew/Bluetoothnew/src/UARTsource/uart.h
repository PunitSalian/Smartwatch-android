
/*******************************************************************************
* File Name		: uart.h
* Description 	: header file for UART driver using RX6xx SCI peripheral.
*******************************************************************************/

#ifndef UART_H // multiple inclusion guard
#define UART_H

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define NEWLINE_CHAR    '\n'
#define RETURN_CHAR     '\r'

/*******************************************************************************
Exported global functions (to be accessed by other files)
*******************************************************************************/
void     sci_uart_init(void);
void     sci_put_char(uint8_t write_data);
uint8_t  sci_get_char(void);
void     sci_put_string(uint8_t *source_string);
uint32_t sci_read_count_get(void);
void     sci_tx_int_enable(void);
void     sci_rx_int_enable(void);
void     sci_tx_int_disable(void);
void     sci_rx_int_disable(void);

#endif