/*
 * UART.h
 *
 * Created: 03/09/2019 12:07:39 ص
 *  Author: AhmedGaber
 */ 


#ifndef UART_H_
#define UART_H_
#include "DIO.h"
#include "std_types.h"
/************************************************************************/
/*						Mapped Registers                                */
/************************************************************************/
#define UDR   (*(volatile uint8 *)0x2C)	/* USART I/O Data Register – UDR */
#define UCSRA (*(volatile uint8 *)0x2B)	/* USART Control and Status Register A – UCSRA */
#define UCSRB (*(volatile uint8 *)0x2A)	/* USART Control and Status Register B – UCSRB */
#define UCSRC (*(volatile uint8 *)0x40)	/* USART Control and Status Register C – UCSRC */
#define UBRRH (*(volatile uint8 *)0x40)	/* USART Baud Rate Registers – UBRRH */
#define UBRRL (*(volatile uint8 *)0x29)	/* USART Baud Rate Registers – UBRRL */

/*************************************************************************/
/*						Registers Bits                                  */
/************************************************************************/

/**************************** UCSRA PINS ***************************************/
#define RXC  (7u)		/*Bit 7 – RXC: UART Receive Complete*/
#define TXC  (6u)		/*Bit 6 – TXC: UART Transmit Complete*/
#define UDRE (5u)		/*Bit 5 – UDRE: UART Data Register Empty*/
#define FE   (4u)		/*Bit 4 – FE: Frame Error*/
#define DOR  (3u)		/*Bit 3 – DOR: Data OverRun*/
#define PE   (2u)		/*Bit 2 – PE: Parity Error*/
#define U2X	 (1u)		/*Bit 1 – U2X: Double the USART Transmission Speed*/
#define MPCM (0u)		/*Bit 0 – MPCM: Multi-processor Communication Mode*/

/**************************** UCSRB PINS ***************************************/
#define RXCIE  (7u)		/*Bit 7 – RXCIE: RX Complete Interrupt Enable*/
#define TXCIE  (6u)		/*Bit 6 – TXCIE: TX Complete Interrupt Enable*/
#define UDRIE  (5u)		/*Bit 5 – UDRIE: USART Data Register Empty Interrupt Enable*/
#define RXEN   (4u)		/*Bit 4 – RXEN: Receiver Enable*/
#define TXEN   (3u)		/*Bit 3 – TXEN: Transmitter Enable*/
#define UCSZ2  (2u)		/*Bit 2 – UCSZ2: Character Size*/
#define RXB8   (1u)		/*Bit 1 – RXB8: Receive Data Bit 8*/
#define TXB8   (0u)		/*Bit 0 – TXB8: Transmit Data Bit 8*/

/**************************** UCSRC PINS ***************************************/
#define URSEL  (7u)		/*Bit 7 – URSEL: Register Select*/
#define UMSEL  (6u)		/*Bit 6 – UMSEL: USART Mode Select*/
#define UPM1   (5u)		/*Bit 5 – UPM1: Parity Mode*/
#define UPM0   (4u)		/*Bit 4 – UPM0: Parity Mode*/
#define USBS   (3u)		/*Bit 3 – USBS: Stop Bit Select*/
#define UCSZ1  (2u)		/*Bit 2 – UCSZ1: Character Size*/
#define UCSZ0  (1u)		/*Bit 1 – UCSZ0: Character Size*/
#define UCPOL  (0u)		/*Bit 0 – UCPOL: Clock Polarity*/


/************************************************************************/
/*                  USER Defined types (configuration)                  */
/************************************************************************/
typedef enum {UART_OK =0, UART_NOK}status_t; /* check type to check function execution succes*/
	
typedef enum {DS_Enabled=1, DS_Disabled=0} DoubleSpeed_t;
typedef enum {RxINT_EN=1, RxINT_Dis=0} RxINT_t;
typedef enum {TxINT_EN=1, TxINT_Dis=0} TxINT_t;
typedef enum {EmptyINT_EN=1, EmptyINT_Dis=0} EmptyINT_t;
typedef enum {Data_5=0, Data_6=1, Data_7=2, Data_8=3, Data_9=4} Datasize_t;
typedef enum {parity_disabled=0, parity_odd=1, parity_even=2} parity_t;
typedef enum {Stop_One=0, Stop_Two=1} stopbits_t;

typedef void (*UART_CallBackPtr)(void);

/*A structure type to contain all the required configuration*/
typedef struct {
	uint32 BaudRate;    
	DoubleSpeed_t DoubleSpeed; 
	RxINT_t RxINT; 
	TxINT_t TxINT; 
	EmptyINT_t EmptyINT; 
	Datasize_t Datasize; 
	parity_t parity; 
	stopbits_t StopBits; 
	/*
	UART_CallBackPtr FrameErrorCallBackPtr;
	UART_CallBackPtr ParityErrorCallBackPtr;
	UART_CallBackPtr DataOverRunCallBackPtr;
	*/
	}UART_Cfg_S;
	
extern const UART_Cfg_S UART_Cfg;
/************************************************************************/
/*                            Functions Prototypes                      */
/************************************************************************/
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
extern status_t UART_INIT(void); /* UART_Cfg_S* UART_Cfg */

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
extern status_t UART_SendChar(uint8 a_data);
extern status_t UART_StartTransmit(void);
extern status_t UART_StopTransmit(void);

/*******************************************************************************
 * Function Name:	UART_RecieveChar
 *
 * Description: 	*assign value of the data to be transmitted to the UDR if it was empty
 *					* if it wasn't it return UART_NOK state
 *
 * Inputs:			NULL
 *
 * Outputs:			uint8 a_data ; it represnetns the byte to be trasmitted
 *
 * Return:			Status to check if UDR hold the new data or it wasn't empty
 *******************************************************************************/
extern status_t UART_ReceiveChar(uint8* a_data_ptr);
extern status_t UART_StartReceive(void);
extern status_t UART_StopReceive(void);

extern status_t UART_SendString( uint8* a_data_ptr);
extern status_t UART_ReceiveString(uint8* a_data_ptr);

status_t UART_ErrorMng(void); 

#endif /* UART_H_ */