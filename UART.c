/*
 * UART.c
 *
 * Created: 03/09/2019 12:06:01 ص
 *  Author: AhmedGaber
 */ 
#include "UART.h"
#include "UART_CFG.h"

/*******************************************************************************
 * Function Name:	UART_Init
 *
 * Description: 	*Initialize the UART Driver Registers
 * 					*Set the driver Configurations
 *
 * Inputs:			Void ; the function reads configurations from global constant structure (UART_Cfg)
 *
 * Outputs:			NULL
 *
 * Return:			Status to check function executions if it is done properly or not
 *******************************************************************************/
status_t UART_INIT( void/*UART_Cfg_S* UART_Cfg*/)
{
	status_t checker = UART_OK; /* checker UART init returns NOT_OK if any configuration wasn't correct*/
	uint16 UBRR = 0u; /* variable to hold BaudRate */
	
	/*******  check DoubleSpeed configuration  **********/
	if (UART_Cfg.DoubleSpeed == DS_Enabled)
	{
	    setbit(UCSRA,U2X);
		UBRR = (((F_CPU / (UART_Cfg.BaudRate * 8UL))) - 1);
	}
	else if (UART_Cfg.DoubleSpeed == DS_Disabled)
	{
	    clrbit(UCSRA,U2X);
		UBRR = (((F_CPU / (UART_Cfg.BaudRate * 16UL))) - 1);
	}
	else 
	{
		checker = UART_NOK;
	}
	/* Set BaudRate */
	clrbit(UCSRC,URSEL); /* access UBRRH */
	UBRRL = (uint8)UBRR;
	UBRRH = (uint8)(UBRR >> 8);
	setbit(UCSRC,URSEL); /* access UCSRC */
	/************* check RX Interrupt configuration ****************/
	if (UART_Cfg.RxINT == RxINT_EN)
	{
		setbit(UCSRB,RXCIE);
	}
	else if (UART_Cfg.RxINT == RxINT_Dis)
	{
		clrbit(UCSRB,RXCIE);
	}
	else 
	{
		checker = UART_NOK;
	}
	/**************** check TX Interrupt configuration *************/
	if (UART_Cfg.TxINT == TxINT_EN)
	{
		setbit(UCSRB,TXCIE);
	}
	else if (UART_Cfg.TxINT == TxINT_Dis)
	{
		clrbit(UCSRB,TXCIE);
	}
	else 
	{
		checker = UART_NOK;
	}
	/***************** check Empty Interrupt configuration **********/
	if (UART_Cfg.EmptyINT == EmptyINT_EN)
	{
		setbit(UCSRB,UDRIE);
	}
	else if (UART_Cfg.EmptyINT == EmptyINT_Dis)
	{
		clrbit(UCSRB,UDRIE);
	}
	else 
	{
		checker = UART_NOK;
	}
	/***************** check Data size in frame configuration ********/
	if (UART_Cfg.Datasize == Data_5)
	{
		clrbit(UCSRB,UCSZ2);
		clrbit(UCSRC,UCSZ1);
		clrbit(UCSRB,UCSZ0);
	}
	else if (UART_Cfg.Datasize == Data_6)
	{
		clrbit(UCSRB,UCSZ2);
		clrbit(UCSRC,UCSZ1);
		setbit(UCSRB,UCSZ0);
	}
	else if (UART_Cfg.Datasize == Data_7)
	{
		clrbit(UCSRB,UCSZ2);
		setbit(UCSRC,UCSZ1);
		clrbit(UCSRB,UCSZ0);
	}
	else if (UART_Cfg.Datasize == Data_8)
	{
		clrbit(UCSRB,UCSZ2);
		setbit(UCSRC,UCSZ1);
		setbit(UCSRB,UCSZ0);
	}
	else if (UART_Cfg.Datasize == Data_9)
	{
		setbit(UCSRB,UCSZ2);
		setbit(UCSRC,UCSZ1);
		setbit(UCSRB,UCSZ0);
	}
	else
	{
		checker = UART_NOK;
	}
	/***************** Check parity bit configuration **********/
	if(UART_Cfg.parity == parity_odd)
	{
		setbit(UCSRC,UPM0);
		setbit(UCSRC,UPM1);
	}
	else if (UART_Cfg.parity == parity_even)
	{
		clrbit(UCSRC,UPM0);
		setbit(UCSRC,UPM1);
	}
	else if (UART_Cfg.parity == parity_disabled)
	{
		clrbit(UCSRC,UPM0);
		clrbit(UCSRC,UPM1);
	}
	else 
	{
		checker = UART_NOK;
	}
	
	/***************** Check stop bits number configuration ******/
	if(UART_Cfg.StopBits == Stop_One)
	{
		clrbit(UCSRC,USBS);
	}
	else if (UART_Cfg.StopBits == Stop_Two)
	{
		setbit(UCSRC,USBS);
	}
	else
	{
		checker = UART_NOK;
	}
	 
	/* return UART_init function status*/
return checker;
}

/*******************************************************************************
 * Function Name:	UART_SendChar
 *
 * Description: 	*assign value of the data to be transmitted to the UDR if it was empty
 *					* if it wasn't it return UART_NOK state
 *
 * Inputs:			uint8 a_data ; it represnetns the byte to be trasmitted
 *
 * Outputs:			NULL
 *
 * Return:			Status to check if UDR hold the new data or it wasn't empty
 *******************************************************************************/

status_t UART_SendChar(uint8 a_data)
{
	status_t checker = UART_OK;
	if (getbit(UCSRA,UDRE)==1) /* UDR empty and ready to transmit */
	 {
		UDR = a_data;
	 }
    else if (getbit(UCSRA,UDRE)==0) 
	{
		checker = UART_NOK;
	}	
return checker;	
}


status_t UART_StartTransmit(void)
{ 
	status_t checker;
	setbit(UCSRB,TXEN); /* Enable UART as TX to start transmitting*/
	if  (getbit(UCSRA,TXC)==0) /*wait until transmitting complete*/
	{
		checker=UART_NOK;
	}
	else{
	setbit(UCSRA,TXC); /* clear TXC flag */
	checker = UART_OK;
	}	
return checker ;
}

status_t UART_StopTransmit(void)
{
	status_t checker = UART_OK;
	clrbit(UCSRB,TXEN); /* Disable UART as TX to stop Transmitting*/
	return checker ;
}

status_t UART_ReceiveChar(uint8* a_data_ptr)
{
	status_t checker = UART_OK;
	if (getbit(UCSRA,RXC)==1)
	{
		*a_data_ptr = UDR;
	}
	else if (getbit(UCSRA,RXC)==0)
	{
		checker = UART_NOK;
	}
	return checker;
}

status_t UART_StartReceive(void)
{
	status_t checker = UART_OK;
	setbit(UCSRB,RXEN); /* Enable UART as TX to start transmitting*/
	if (getbit(UCSRA,RXC)==0) 
	{
	checker = UART_NOK;
	}
	checker = UART_OK;
	//while (getbit(UCSRA,RXC)==0); /*wait until Reception complete*/
	return checker ;
}

status_t UART_StopReceive(void)
{
	status_t checker = UART_OK;
	clrbit(UCSRB,RXEN); /* Disable UART as RX to stop Reception */
	return checker ;
}


status_t UART_SendString( uint8* a_data_ptr) 
{
	status_t checker = UART_OK;
	uint8 index ;
	for(index=0;a_data_ptr[index]!='#' && checker == UART_OK;index++)
	{
		checker = UART_SendChar(a_data_ptr[index]);
		checker = UART_StartTransmit();
		checker = UART_StopTransmit();
	}
	return checker;
}

status_t UART_ReceiveString( uint8* a_data_ptr)
{
	status_t checker = UART_OK;
	uint8 index ;
	for(index=0;a_data_ptr[index]!='#' && checker == UART_OK;index++)
	{
		checker = UART_ReceiveChar(&a_data_ptr[index]);
		checker = UART_StartReceive();
		checker = UART_StopReceive();
	}
	return checker;
}
/*
status_t UART_ErrorMng(void)
{
	if (getbit(UCSRA,FE)==1)
	{
		FrameErrorCallBackPtr(void);	
	}
	if (getbit(UCSRA,DOR)==1)
	{
		DataOverRunCallBackPtr(void);	
	}
	if (getbit(UCSRA,PE)==1)
	{
		ParityErrorCallBackPtr();
	}
}
*/