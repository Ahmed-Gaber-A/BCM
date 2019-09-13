/*
 * std_types.h
 *
 * Created: 17/08/2019 11:55:17 ص
 *  Author: AhmedGaber
 */ 


#ifndef STD_TYPES_H_
#define STD_TYPES_H_

typedef unsigned char uint8;
typedef  char int8;
typedef unsigned short uint16;
typedef  short int16;
typedef unsigned int uint32;
typedef int int32;
typedef float f16;
typedef double f32;

//Macros

#define setbit(port,bit)    (port |=(1<<(bit)))
#define clrbit(port,bit) 	(port &=(~(1<<(bit))))
#define togglebit(port,bit) (port ^=(1<<(bit)))
#define getbit(port ,bit)   ((port >> (bit) )&1)



#endif /* STD_TYPES_H_ */