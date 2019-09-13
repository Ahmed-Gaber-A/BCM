/*
 * BCM.c
 *
 * Created: 08/09/2019 05:16:38 م
 *  Author: AhmedGaber
 */ 

static uint8 TxState = IDLE;
static uint8 RxState = IDLE;
static uint8 * Tx_data_ptr;
static uint8 * Rx_data_ptr;

static MsgInfo_t TX_MsgInfo = {0u};
static MsgInfo_t RX_MsgInfo = {0u};
	
/*
void BCM_TxManage_INT(void)
{
	TX_MsgInfo.index++ ;
	TX_MsgInfo.CheckSum += arr[TX_MsgInfo.index];
	UART_SendChar(Tx_data_ptr[TX_MsgInfo.index]);
}

void BCM_RxManage_INT(void)
{
	RxState = BUSY;
	UART_ReceiveChar(&data_ptr[RX_MsgInfo.index]);
	RX_MsgInfo.CheckSum += arr[RX_MsgInfo.index];
	RX_MsgInfo.index++;
}
*/

status_t BCM_Send (uint8* arr , uint16 size)
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
		checker = BCM_NOK;
	}	
	
return checker;
}

status_t BCM_Receive (uint8* arr)
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



status_t BCM_Init(void) 
{
	UART_Init();
}

status_t BCM_TxDispatcher(void) 
{
	status_t checker ;
	static uint8 Header_Sent = 0u ;
	static uint8 Tx_Done = 0u ; 
	
	switch (TxState)
	{
		case 'IDLE' :
			checker = BCM_OK;
		    break;
			
		case 'BUSY' : /* Dispatch TX Data*/
     		if (Header_Sent == 0u && Tx_Done == 0u)  /* construct the header */
			 {
			  /*Disable interrupt*/
			  clrbit(SREG,7);
			  UART_SendChar((uint8)(TX_MsgInfo.size % 8u)); /* Send the least significant byte first */
			  while (getbit(U,TXC));  /* polling on TX complete flag for 1 sending cycle */
			  setbit(U,TXC);
			  UART_SendChar((uint8)(TX_MsgInfo.size / 8u)); /* Send the most significant byte second */
			  while (getbit(U,TXC));  /* polling on TX complete flag for 1 sending cycle */
			  setbit(U,TXC);
			  Header_Sent = 1 ;
			  setbit(SREG,7);/*Enable Interrupt*/
			 }
	        else if (Header_Sent == 1u && Tx_Done == 1u )  /*Send Check Sum byte*/
		     {
			  /*Disable interrupt*/
			  //clrbit(SREG,7);
			  UART_SendChar((uint8)(TX_MsgInfo.CheckSum)); /* Send the check sum byte */						//////DONE!
			  while (!(UART_StartTransmit(void)));/* polling on TX complete flag for 1 sending cycle */
			  UART_StopTransmit(void);
			  setbit(SREG,7);/*Enable Interrupt*/
			  Header_Sent =0;
			  Tx_Done = 0;
			  TxState = IDLE; /* now I am done*/
		     }
		    else if (Header_Sent == 1 && Tx_Done == 0 )
		     {				 
			  if (TX_MsgInfo.index == TX_MsgInfo.size)
			   {
				   Tx_Done =1;
			   }
			  else
			   {
				   if (TxByte_done) 
				   {
				   TX_MsgInfo.CheckSum += arr[TX_MsgInfo.index];
				   TX_MsgInfo.index++ ;
				   TxByte_done =0;
				   UART_SendChar(Tx_data_ptr[TX_MsgInfo.index - 1u]);
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

status_t BCM_RxDispatcher(void)  /* idle -> busy  by ISR*/
{
	status_t checker ;
	static uint8 Header_Recieved = 0u ;
	static uint8 Rx_Done = 0u ;
	switch (RxState)
	{
		case 'IDLE' :
		if (RxByte_done)
		{
			RxState = BUSY;
		}
			checker = BCM_OK;
		break;
		
		case 'BUSY' :
		if (RxByte_done)
		{	
			
			if (Header_Recieved == 0u && Rx_Done == 0u)  /* construct the header */
			{
				/*Disable interrupt*/
				clrbit(SREG,7u);
				UART_ReceiveChar(&((uint8 *)(RX_MsgInfo.size))); /* Save the least significant byte first */
				
				while (getbit(U,RXC));  /* polling on RX complete flag for 1 sending cycle */
				setbit(U,RXC);
				UART_ReceiveChar(&((uint8 *)(RX_MsgInfo.size) + 1u)); /* Save the most significant byte second */
				while (getbit(U,RXC));  /* polling on RX complete flag for 1 sending cycle */
				setbit(U,TXC);
				Header_Recieved = 1u ;
				setbit(SREG,7);/*Enable Interrupt*/
			}
			else if (Header_Recieved == 1u && Rx_Done == 1u )  /*Send Check Sum byte*/
			{
				uint8 CheckSum_Tx ;
				UART_ReceiveChar(&CheckSum_Tx);
				if(RX_MsgInfo.CheckSum == CheckSum_Tx)
				{
					checker = BCM_OK ; /*Success*/
				}
				
				Header_Recieved =0;
				Rx_Done = 0;
				RxState = IDLE; /* now I am done*/
			}
		else if (Header_Recieved == 1 && Rx_Done == 0 )
			{
				if (RX_MsgInfo.index == RX_MsgInfo.size)
				{
					Rx_Done =1;
				}
				else
				{
						UART_ReceiveChar(&Rx_data_ptr[RX_MsgInfo.index]);
						RX_MsgInfo.CheckSum += arr[RX_MsgInfo.index];
						RX_MsgInfo.index++ ;
						RxByte_done =0;
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

}
return checker;
}