/*
 * UART_CFG.c
 *
 * Created: 03/09/2019 12:08:40 ص
 *  Author: AhmedGaber
 */ 
#include "UART.h"

/*void test (void);*/

const UART_Cfg_S UART_Cfg = {
		9600,
		DS_Disabled, RxINT_Dis, TxINT_Dis, EmptyINT_Dis,
		Data_8, parity_odd, Stop_Two
	/*	,test,test,test */
	};