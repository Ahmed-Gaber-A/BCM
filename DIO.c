/*
 * DIO.c
 *
 * Created: 17/08/2019 12:13:42 م
 *  Author: AhmedGaber
 */ 

#include "DIO.h"

void DIO_WritePin (uint8 PinNum , uint8 PinValue)
{
	if((PinNum>=0)&&(PinNum<=7))
	{
		if(PinValue==0)
		{
			clrbit(PORTA,PinNum);
		}
		else
		{
			setbit(PORTA,PinNum);
		}
	}
	else if((PinNum>=8)&&(PinNum<=15))
	{
		if(PinValue==0)
		{
			clrbit(PORTB,(PinNum-8));
		}
		else
		{
			setbit(PORTB,(PinNum-8));
		}
	}
	else if((PinNum>=16)&&(PinNum<=23))
	{
		if(PinValue==0)
		{
			clrbit(PORTC,(PinNum-16));
		}
		else
		{
			setbit(PORTC,(PinNum-16));
		}
	}
	else if((PinNum>=24)&&((PinNum<=31)))
	{
		if(PinValue==0)
		{
			clrbit(PORTD,(PinNum-24));
		}
		else
		{
			setbit(PORTD,(PinNum-24));
		}
	}
}
uint8 DIO_ReadPin(uint8 PinNum)
{
	uint8 PinValue;


	if((PinNum >= 0) && (PinNum <= 7))
	{
		PinValue = getbit(PINA,PinNum);
	}
	else if((PinNum >= 8) && (PinNum <= 15))
	{
		PinValue = getbit(PINB,(PinNum-8));
	}
	else if((PinNum >= 16) && (PinNum <= 23))
	{
		PinValue = getbit(PINC,(PinNum-16));
	}
	else if((PinNum >= 24) && (PinNum <= 31))
	{
		PinValue = getbit(PIND,(PinNum-24));
	}

	return PinValue;
}
void DIO_SetPinDirection(uint8 PinNum,uint8 PinDirection)
{
	if((PinNum>=0)&&(PinNum<=7))
	{
		if(PinDirection==0)
		{
			clrbit(DDRA,PinNum);
		}
		else
		{
			setbit(DDRA,PinNum);
		}
	}
	else if((PinNum>=8)&&(PinNum<=15))
	{
		if(PinDirection==0)
		{
			clrbit(DDRB,(PinNum-8));
		}
		else
		{
			setbit(DDRB,(PinNum-8));
		}
	}
	else if((PinNum>=16)&&(PinNum<=23))
	{
		if(PinDirection==0)
		{
			clrbit(DDRC,(PinNum-16));
		}
		else
		{
			setbit(DDRC,(PinNum-16));
		}
	}
	else if((PinNum>=24)&&((PinNum<=31)))
	{
		if(PinDirection==0)
		{
			clrbit(DDRD,(PinNum-24));
		}
		else
		{
			setbit(DDRD,(PinNum-24));
		}
	}
}

void cpu_sleep(void) 
{
	setbit(MCUCR,SE);
	clrbit(MCUCR,SM0);
	clrbit(MCUCR,SM1);
	clrbit(MCUCR,SM2);
}