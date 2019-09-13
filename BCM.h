/*
 * BCM.h
 *
 * Created: 08/09/2019 05:16:57 م
 *  Author: AhmedGaber
 */ 
#include "DIO.h"

#ifndef BCM_H_
#define BCM_H_
/*return status*/
#define BCM_status_t uint8
#define BCM_OK 0

#define BCM_BUSY 1
#define BCM_SUCCESS 2
#define BCM_FAIL 3 
#define BCM_NOK 5


/* states*/
#define IDLE 0
#define BUSY 1 //RECEVING

#define BCM_MaxLength 1000 

/*  structure holds message information  */
typedef struct {
	uint16 size;
	uint16 index;
	uint8 CheckSum;
	}MsgInfo_t; 

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
extern BCM_status_t BCM_Init(void);

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
extern BCM_status_t BCM_Send (uint8* arr , uint16 size);

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
extern BCM_status_t BCM_Receieve (uint8* arr) ;

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
extern BCM_status_t BCM_TxDispatcher(void);

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
extern BCM_status_t BCM_RxDispatcher(void);

#endif /* BCM_H_ */