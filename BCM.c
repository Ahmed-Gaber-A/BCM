/*
 * BCM.c
 *
 * Created: 08/09/2019 05:16:38 م
 *  Author: AhmedGaber
 */ 

#include "BCM.h"
#include "UART.h"

/************************************************************************/
/* Global variables                                                                     */
/************************************************************************/
volatile uint8 TxByte_done ;
volatile uint8 RxByte_done ;
/************************************************************************/
/*                    Static Global Variables                                                  */
/************************************************************************/

static uint8 TxState = IDLE;
static uint8 RxState = IDLE;

static uint8 * Tx_data_ptr; /*pointer to TX data buffer*/
static uint8 * Rx_data_ptr; /*pointer to RX data buffer*/

static MsgInfo_t TX_MsgInfo = {0u}; /* TX msg info structure it holds the size , checksum,index*/
static MsgInfo_t RX_MsgInfo = {0u}; /* RX msg info structure the size , checksum,index*/

/************************************************************************/
/*                           Function definitions                       */
/************************************************************************/

/************************************************************************/
/* Function name: BCM_Send
*
* Description: It is called by the user to invoke transmitting process
*
* Inputs: arr  ; is pointer to the first element in the data
*		  size ; is the number of bytes of the data
*
* Outputs: NULL
*
* Return:  status to checker if the BCM is OK to send or busy.                                                               */
/************************************************************************/

BCM_status_t BCM_Send (uint8* arr , uint16 size)
{
	status_t checker;
	if (TxState==IDLE){
		
	TxState=BUSY;
	Tx_data_ptr = arr;
	TX_MsgInfo.size = size;
	checker = BCM_OK;
	}
	else 
	{
		checker = BCM_BUSY;
	}	
	
return checker;
}
/************************************************************************/
/* Function name: BCM_Receive
*
* Description: It is called by the user to invoke Reception process
*
* Inputs: NULL
*
*
* Outputs: arr  ; is pointer to the first element in the data buffer to be filled
*
* Return:  status to checker if the BCM was receiving so that new data available or it was IDle.                                                               */
/************************************************************************/
BCM_status_t BCM_Receive (uint8* arr)
{	
	status_t checker;
	if (RxState==IDLE){
		
		checker = BCM_OK;
	}
	else
	{
		arr = Rx_data_ptr;
		checker = BCM_OK;
	}
	
	return checker;
}

/************************************************************************/
/* Function name: BCM_Init
*
* Description: It is called by the user to initialize the BCM module so that the UART...
*
* Inputs: NULL
*
* Outputs: NULL
*
* Return:  status to checker if the BCM initialized succesfully or not.                                                               */
/************************************************************************/

BCM_status_t BCM_Init(void) 
{
return	UART_INIT();

}

/**********************************************************************************************/
/* Function name: BCM_TxDispatcher
*
* Description: It is called by the user to dispatch transmitting process
*			   It constructs the header and checksum ,sends them and handle data transmitting 
* Inputs: NULL
*
*
* Outputs: NULL
*
* Return:  status to checker if the BCM Transmitting was successful or failed                                                               */
/************************************************************************************************/
BCM_status_t BCM_TxDispatcher(void) 
{
	status_t checker ;
	static uint8 Header_Sent = 0u ;
	static uint8 Tx_Done = 0u ; 
	
	switch (TxState)
	{
		case 'IDLE' :
			UART_StopTransmit();
			checker = BCM_OK;
		    break;
			
		case 'BUSY' : /* Dispatch TX Data*/
     		if (Header_Sent == 0u && Tx_Done == 0u)  /* construct the header and send first byte */ 
			 {
			  UART_SendChar((uint8)(TX_MsgInfo.size % 8u)); /* Send the least significant byte first */
			  UART_StartTransmit();
			 /* while (!(UART_StartTransmit(void))); /*polling on TX complete flag for 1 sending cycle */
			 /*  UART_StopTransmit(void); */
			  Header_Sent = 1u; 
			 }
		else if (Header_Sent == 1u && Tx_Done == 0u) /* Send the most significant byte second */
			 {			  
			 UART_SendChar((uint8)(TX_MsgInfo.size / 8u)); 
			 UART_StartTransmit();
			 /* while (!(UART_StartTransmit(void))); /*polling on TX complete flag for 1 sending cycle */ 
			 /*  UART_StopTransmit(void); */
			 /*other way is dealing with header construction as critical section and disable interrupts while 
			  *executing this part of code */
			  Header_Sent = 2u ;
			 }
	        else if (Header_Sent == 2u && Tx_Done == 1u )  /*Send Check Sum byte*/
		     {
			  UART_SendChar(TX_MsgInfo.CheckSum); /* Send the check sum byte */
			  UART_StartTransmit();						
			  /* while (!(UART_StartTransmit(void)));/* polling on TX complete flag for 1 sending cycle */
			  /*UART_StopTransmit(void);*/
			  
			  Header_Sent =0u;
			  Tx_Done = 0u;
			  TxState = IDLE; /* now I am done*/
		     }
		    else if (Header_Sent == 2u && Tx_Done == 0u )
		     {				 
			  if (TX_MsgInfo.index == TX_MsgInfo.size)
			   {
				   Tx_Done =1u;
			   }
			  else
			   {	/*This part of code is executed while transmitting the data */
				   if (TxByte_done)  /*Flag set by the ISR if the data byte is transmitted*/
				   {
				   TX_MsgInfo.CheckSum += Tx_data_ptr[TX_MsgInfo.index]; 
				   UART_SendChar(Tx_data_ptr[TX_MsgInfo.index]);
				   TX_MsgInfo.index++ ; /*increment index to the next byte*/
				   TxByte_done =0u;
				   UART_StartTransmit();
				   }			       
			   } 
			 }
		    else
			 {
				checker  = BCM_NOK;
			 }
		    checker  =BCM_OK;
		    break;
		
		default : /*Do_nothing*/
		    break;
	}
	
return checker;	
}

/**********************************************************************************************/
/* Function name: BCM_RxDispatcher
*
* Description: It is called by the user to dispatch transmitting process
*			   It de-constructs the header , calculate checksum  and handle data receiving
* Inputs: NULL
*
*
* Outputs: NULL
*
* Return:  status to checker if the BCM receiving was successful or failed                                                               */
/************************************************************************************************/
BCM_status_t BCM_RxDispatcher(void)  /* idle -> busy  by ISR*/
{
	status_t checker ;
	static uint8 Header_Recieved = 0u ;
	static uint8 Rx_Done = 0u ;
	switch (RxState)
	{
		case 'IDLE' :
		if (RxByte_done==0u)
		{
			checker = BCM_OK;
			break;
		}
		else
		{
			RxState = BUSY;
			checker = BCM_OK;
		}
		
		case 'BUSY' :
		if (RxByte_done)
		{	
			
			if (Header_Recieved == 0u && Rx_Done == 0u)  /* construct the header */
			{
				UART_ReceiveChar(((uint8 *)(RX_MsgInfo.size))); /* Save the least significant byte first */
				UART_StartReceive();
				/*while (getbit(U,RXC));  /* polling on RX complete flag for 1 sending cycle */
			Header_Recieved = 1u;	
			}			
		else if (Header_Recieved == 1u && Rx_Done == 0u)
				UART_ReceiveChar(((uint8 *)(RX_MsgInfo.size) + 1u)); /* Save the most significant byte second */
				UART_StartReceive();
				/*while (getbit(U,RXC));  /* polling on RX complete flag for 1 sending cycle */
				Header_Recieved = 2u ;
			}
			else if (Header_Recieved == 2u && Rx_Done == 1u )  /*Send Check Sum byte*/
			{
				uint8 CheckSum_Tx ;
				UART_ReceiveChar(&CheckSum_Tx);
				if(RX_MsgInfo.CheckSum == CheckSum_Tx)
				{
					checker = BCM_SUCCESS ; /*Success*/
				}
				else 
				{
				   checker = BCM_FAIL;
				}
				
				Header_Recieved =0u;
				Rx_Done = 0u;
				RxState = IDLE; /* now I am done*/
				UART_StopReceive();
			}
		else if (Header_Recieved == 1u && Rx_Done == 0u )
			{
				if (RX_MsgInfo.index == RX_MsgInfo.size)
				{
					Rx_Done =1u;
				}
				else
				{
						UART_ReceiveChar(&Rx_data_ptr[RX_MsgInfo.index]);
						RX_MsgInfo.CheckSum += Tx_data_ptr[RX_MsgInfo.index];
						RX_MsgInfo.index++ ;
						RxByte_done =0u;
						UART_StartReceive();
				}
				}
		  else
		    {
			  checker =BCM_NOK;
	     	}
		
		
		break;
		
		default : /*Do_nothing*/
		break;
	}


return checker;
}