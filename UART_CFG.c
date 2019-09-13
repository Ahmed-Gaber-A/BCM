/*
 * UART_CFG.c
 *
 * Created: 03/09/2019 12:08:40 ص
 *  Author: AhmedGaber
 */ 
#include "UART.h"

extern volatile uint8 TxByte_done ; 

extern volatile uint8 RxByte_done ; 

const UART_Cfg_S UART_Cfg = {
		9600,
		DS_Disabled, RxINT_EN, TxINT_EN, EmptyINT_Dis,
		Data_8, parity_odd, Stop_One,
		&RxByte_done , &TxByte_done
	};