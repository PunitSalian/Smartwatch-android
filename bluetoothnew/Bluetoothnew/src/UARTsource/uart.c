
/*******************************************************************************
* File Name		: uart.c

* Description 	:Hardware abstraction layer for Uart.
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <machine.h>

#include "platform.h"
#include "uart.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define TX_BUF_LEN	128
#define RX_BUF_LEN	128
#define BAUDRATE	9600 /* example other choices: 9600, 19200, 38400 */

#define SCI12_INT_LVL 6

/******************************************************************************
Private global variables and functions
******************************************************************************/
volatile uint8_t g_tx_buffer[TX_BUF_LEN];
volatile uint8_t g_rx_buffer[RX_BUF_LEN];

volatile uint32_t g_tx_count = 0;
volatile uint32_t g_tx_in_index = 0;
volatile uint32_t g_tx_out_index = 0;

/* Keeps track of un-processed data count remaining in read buffer. */
volatile uint32_t g_rx_count = 0;
/* Read buffer position at which incoming data from UART will be stored. */
volatile uint32_t g_rx_in_index = 0;
/* Read buffer position from which data has been processed. */
volatile uint32_t g_rx_out_index = 0;
/* A flag that indicates TX buffer status. */
volatile bool g_uart_tx_ready = false;

volatile bool notif = false;
/******************************************************************************
* Function Name	: sci_uart_init
* Description	: Initialize SCI12 as asynchronous UART
* Arguments     : none
* Return value	: none
*******************************************************************************/
void sci_uart_init(void)
{
	uint16_t i; /* for delay counter */


#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
#endif

							   /* clear ACSE Bit (All-Module Clock Stop Mode Enable) */
	SYSTEM.MSTPCRA.BIT.ACSE = 0;

	/* Cancel stop state of SCI12 Peripheral to enable writing to it*/
	MSTP(SCI12) = 0;

#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
#endif     

							   /* Clear bits TIE, RIE, RE, and TEIE in SCR to 0. Set CKE to internal. */
	SCI12.SCR.BYTE = 0x00;

	/* Set up the UART I/O port and pins. */
	MPC.PE2PFS.BYTE = 0x4C; /* P50 is TxD2 */
	MPC.PE1PFS.BYTE = 0x4C; /* P52 is RxD2 */

	PORTE.PDR.BIT.B1 = 1;    /* TxD2 is output. */
	PORTE.PDR.BIT.B2 = 0;    /* RxD2 is input. */

	PORTE.PCR.BIT.B2 = 1;	//PULL UP

	PORTE.PMR.BIT.B1 = 1;    /* TxD2 is peripheral. */
	PORTE.PMR.BIT.B2 = 1;    /* RxD2 is peripheral. */

							 /* Set data transfer format in Serial Mode Register (SMR)*/
							 /*  -Asynchronous Mode`
							 -8 bits
							 -no parity
							 -1 stop bit
							 -PCLK clock (n = 0) */
	SCI12.SMR.BYTE = 0x00;

	SCI12.SCMR.BIT.SMIF = 0; /* Set to 0 for serial communications interface mode. */

							 /* Set bit rate register.
							 Example: set to 115200 baud
							 N = (PCLK Frequency) / (64 * 2^(2*n - 1) * Bit Rate) - 1
							 N = (48,000,000)     / (64 * 2^(2*0 - 1) * 115200)   - 1
							 N = 12 */
	SCI12.BRR = 48000000 / ((64 / 2) * BAUDRATE) - 1; /* SCI12.BRR = 12; 12 is for 115200 */

													  /* Wait at least one bit interval */
													  /* TODO: check this for sufficient delay */
	for (i = 0; i < 200; i++) /* assume minimum of 2 instructions at 98MHz? */
	{
	}

	/* Enable RXI and TXI interrupts in SCI peripheral */
	SCI12.SCR.BIT.RIE = 1;   /* Set Receive Interrupt (RX buffer full) enable. */
	SCI12.SCR.BIT.TIE = 1;   /* Set Transmit Interrupt (TX data register empty) enable. */
	SCI12.SCR.BIT.TEIE = 1;   /* Set Transmit End interrupt enable */

							  /* Clear IR bits for TIE and RIE */
	IR(SCI12, RXI12) = 0;
	IR(SCI12, TXI12) = 0;



	/* Set interrupt prio for SCI. */
	IPR(SCI12, TXI12) = SCI12_INT_LVL;
	IPR(SCI12, RXI12) = SCI12_INT_LVL;

	/* Enable Tx/Rx */
	/* Note: A 1 can only be written to these bits when TE = 0 and RE = 0. */
	/* After setting TE or RE to 1, only 0 can be written in TE and RE.    */
	/* So to enable both, write them simultaneously.                       */
	SCI12.SCR.BYTE |= 0x30;

	/* Initialize buffered I/O variables.  */
	g_tx_out_index = 0;
	g_tx_in_index = 0;
	g_tx_count = 0;
	g_rx_out_index = 0;
	g_rx_in_index = 0;
	g_rx_count = 0;

} /* End of function sci_uart_init() */


  /******************************************************************************
  * Function Name	: sci_put_char
  * Description	: Writes a byte to the serial port. Uses buffered output for
  *               : low latency. If there is room in the buffer the data gets
  *               : copied in and the function returns immediately. The accumulated
  *               : data in the buffer will be written out by the TX interrupt
  *               : handler. If the buffer is full then this function blocks until
  *               : space is available. Adjust baud rate and/or buffer size as
  *               : required by the application to minimize waiting time.
  *
  * Arguments     : write_data -
  *                     The byte to be written
  * Return value	: None
  *******************************************************************************/
void sci_put_char(uint8_t write_data)
{
	/* Enable transmit interrupts in ICU */
	IEN(SCI12, TXI12) = 1;    /* Expands to: ICU.IER1A.BIT.IEN7 = 1; */

							  /* Wait for room in the write buffer. Normally the buffer would be large enough */
							  /* that this does not block. Adjust buffer size or baudrate if the buffer gets  */
							  /* full too frequently.                                                         */
	while (g_tx_count >= TX_BUF_LEN) /* TX ISR will make room as it sends out data */
	{
	}

	/* Disable transmit interrupts to avoid access conflict of globals */
	IEN(SCI12, TXI12) = 0;

	/* No data being sent right now so put this character right into the UART */
	/* and enable TX interrupts                                               */
	if (g_uart_tx_ready)
	{
		/* TODO: Peripheral interrupt enables already enabled in init function. */
		/*       Do interrupts need to be re-enabled with every write? */
		SCI12.SCR.BIT.TEIE = 1;          /* Enable transmit complete IRQ in peripheral. */
		SCI12.SCR.BIT.TIE = 1;          /* Enable transmit IRQ in peripheral. */

		SCI12.TDR = write_data;
		g_uart_tx_ready = false;      /* Clear ready flag to indicate busy status. */
	}
	else      /* Characters already being sent, queue this one in buffer. */
	{
		g_tx_buffer[g_tx_in_index] = write_data; /* Put character in outbound buffer */
		g_tx_in_index++;

		/* Treat transmit buffer as circular buffer */
		if (g_tx_in_index >= TX_BUF_LEN)
		{
			g_tx_in_index = 0; /* Wrap back around to start of buffer */
		}
		g_tx_count++;
	}

	IEN(SCI12, TXI12) = 1; /* Re-enable TX buffer empty interrupt. */
	IEN(SCI12, TEI12) = 1; /* Transmit complete interrupt. */

} /* End of function sci_put_char() */


  /******************************************************************************
  * Function Name	: sci_get_char
  * Description	: Transfers a byte from receive buffer that has been filled by the
  *               : serial port receive interrupt handler.
  * Arguments     : None
  * Return value	: read_data -
  *                     the byte read
  *******************************************************************************/
uint8_t sci_get_char(void)
{
	uint8_t read_data = 0;

	if (g_rx_count > 0)
	{
		/* Disable RX interrupts to avoid access conflict of globals. */
		while (IEN(SCI12, RXI12) != 0)    /* Loop if necessary to handle latency */
		{
			IEN(SCI12, RXI12) = 0;
		}

		read_data = g_rx_buffer[g_rx_out_index];
		g_rx_out_index++;

		/* Treat read buffer as circular */
		if (g_rx_out_index >= RX_BUF_LEN)
		{
			g_rx_out_index = 0;     /* Reached end of buffer. Wrap back to start */
		}

		g_rx_count--;
		IEN(SCI12, RXI12) = 1;        /* Re-enable RX interrupts */
	}

	return (read_data);

} /* End of function sci_get_char() */


  /******************************************************************************
  * Function Name	: sci_put_string
  * Description	: Outputs a null terminated string from the serial port
  * Arguments     : source_string -
  *                     Pointer to the null terminated string to be sent
  * Return value	: None
  *******************************************************************************/
void sci_put_string(uint8_t *source_string)
{
	do
	{
		sci_put_char(*source_string);
		source_string++;
	} while (0 != *source_string);

	sci_put_char(*source_string);         /* write the null terminator */
} /* End of function sci_put_string() */


  /******************************************************************************
  * Function Name	: sci_read_count_get
  * Description	: Gets the current count of unprocessed data in the read buffer.
  * Arguments     : None
  * Return value	: The byte count.
  *******************************************************************************/
uint32_t sci_read_count_get(void)
{
	uint32_t count;

	/* Disable RX interrupts to avoid access conflict to globals. */
	IEN(SCI12, RXI12) = 0;

	if (IEN(SCI12, RXI12))
	{
		/* Dummy read to handle I/O write latency. */
	}

	count = g_rx_count;
	IEN(SCI12, RXI12) = 1;         /* Re-enable RX interrupts */
	return count;
} /* End of function sci_read_count_get() */


  /******************************************************************************
  * Function Name	: sci_tx_int_enable
  * Description	: Enables handling of TXI interrupts from SCI channel 2.
  * Arguments     : None
  * Return value	: None
  *******************************************************************************/
void sci_tx_int_enable(void)
{
	IEN(SCI12, TXI12) = 1;
} /* End of function sci_tx_int_enable() */


  /******************************************************************************
  * Function Name	: sci_rx_int_enable
  * Description	: Enables handling of RXI interrupts from SCI channel 2.
  * Arguments     : None
  * Return value	: None
  *******************************************************************************/
void sci_rx_int_enable(void)
{
	IEN(SCI12, RXI12) = 1;
} /* End of function sci_rx_int_enable() */


  /******************************************************************************
  * Function Name	: sci_tx_int_disable
  * Description	: Disables handling of TXI interrupts from SCI channel 2.
  * Arguments     : None
  * Return value	: None
  *******************************************************************************/
void sci_tx_int_disable(void)
{
	IEN(SCI12, TXI12) = 0;

	if (IEN(SCI12, TXI12)) /* Wait for pipelining. See manual about writing I/O registers. */
	{
	}
} /* End of function sci_tx_int_disable() */


  /******************************************************************************
  * Function Name	: sci_rx_int_disable
  * Description	: Disables handling of RXI interrupts from SCI channel 2.
  * Arguments     : None
  * Return value	: None
  *******************************************************************************/
void sci_rx_int_disable(void)
{
	IEN(SCI12, RXI12) = 0;

	if (IEN(SCI12, RXI12)) /* Wait for pipelining. See manual about writing I/O registers. */
	{
	}
} /* End of function sci_rx_int_disable() */

  /******************************************************************************
  * Function Name	: SCI12_TXI12_isr
  * Description	: SCI12 transmit data buffer empty interrupt service routine
  * Arguments     : none
  * Return value	: none
  *******************************************************************************/
#pragma interrupt SCI12_TXI12_isr(vect = VECT_SCI12_TXI12, enable)
static void SCI12_TXI12_isr(void)
{
	if (g_tx_count)
	{
		g_uart_tx_ready = false;       /* Clear ready flag to indicate busy status. */
		SCI12.TDR = g_tx_buffer[g_tx_out_index];
		g_tx_out_index++;

		/* Treat transmit buffer as circular buffer */
		if (g_tx_out_index >= TX_BUF_LEN)
		{
			g_tx_out_index = 0;
		}

		g_tx_count--;   /* Decrement the count of number of bytes to be written. */
	}
	else    /* Nothing to transmit. */
	{
		SCI12.SCR.BIT.TIE = 0;    /* TODO: is this necessary? */
		IEN(SCI12, TXI12) = 0;    /* Disable transmit interrupts in ICU. */
		g_uart_tx_ready = true; /* Flag that the TX buffer is available. */
	}

} /* End of SCI12_TXI12_isr */


  /******************************************************************************
  * Function Name	: SCI12_RXI12_isr
  * Description	: SCI12 receive data buffer full interrupt service routine
  * Arguments     : none
  * Return value	: none
  *******************************************************************************/
#pragma interrupt SCI12_RXI12_isr(vect = VECT_SCI12_RXI12, enable)
static void SCI12_RXI12_isr(void)
{
	notif = true;
	uint8_t read_byte;

	read_byte = SCI12.RDR;      /* Read a byte from the UART data register. */

	while (SCI12.SSR.BIT.ORER)   /* Check for overrun error. */
	{
		SCI12.SSR.BIT.ORER = 0; /* Clear overrun error. Repeats until condition cleared. */
								/* TODO user: Error could be reported here if desired. */
	}

	if (g_rx_in_index < RX_BUF_LEN)
	{
		g_rx_buffer[g_rx_in_index] = read_byte;
	}
	else     /* Treat read buffer as a circular buffer. Point back to beginning if full. */
	{
		g_rx_in_index = 0;
		g_rx_buffer[g_rx_in_index] = read_byte;
	}

	g_rx_in_index++;
	g_rx_count++;    /* Increment unprocessed data count. Gets decremented when processed. */

} /* End of SCI12_RXI12_isr */


  /******************************************************************************
  * Function Name	: SCI12_TEI12_isr
  * Description	: SCI12 Transmit Completed interrupt service routine
  * Arguments     : none
  * Return value	: none
  *******************************************************************************/
#pragma interrupt SCI12_TEI12_isr(vect = VECT(SCI12, TEI12))
static void SCI12_TEI12_isr(void)
{
	SCI12.SCR.BIT.TEIE = 0;          /* TODO: does this need to be done here? */
	IEN(SCI12, TEI12) = 0;          /* Disable transmit ended interrupts in ICU. */
	g_uart_tx_ready = true;       /* Flag that the TX buffer is available */

} /* End of SCI12_TEI12_isr */


  /* end of file uart.c */

