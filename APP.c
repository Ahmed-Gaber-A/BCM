/*
 * APP.c
 *
 * Created: 08/09/2019 05:18:04 م
 *  Author: AhmedGaber
 */ 

#include "DIO.h"
#include "UART.h"
#include "BCM.h"

/*The code for CPU_sleep() function.
* is in the DIO module.
*/

int main(void)
{
	uint16 data_size = BCM_MaxLength;
	uint8 data[data_size] ;
	
	while(1)
	{
		BCM_Send(data,data_size);
		BCM_TxDispatcher();
		BCM_RxDispatcher();
		cpu_sleep();
		
		
	}
}