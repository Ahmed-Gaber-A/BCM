/*
 * BCM.h
 *
 * Created: 08/09/2019 05:16:57 م
 *  Author: AhmedGaber
 */ 


#ifndef BCM_H_
#define BCM_H_
/*return status*/
#define status_t uint8
#define BCM_OK 0
#define BCM_BUSY 1
#define BCM_SUCCESS 2
#define BCM_FAIL 3 



/* states*/
#define IDLE 0
#define BUSY 1 //RECEVING
//#define RECEIVING 2
//#define DONE 3

/*  structure holds message information  */
typedef struct {
	uint16 size;
	uint16 index;
	uint8 CheckSum;
	}MsgInfo_t; 


extern status_t BCM_Init(void);
extern status_t BCM_Send (uint8* arr , uint16 size);
extern status_t BCM_Receieve (uint8* arr) ;
extern status_t BCM_TxDispatcher(void);
extern status_t BCM_RxDispatcher(void);

#endif /* BCM_H_ */