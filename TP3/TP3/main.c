/*
 * TP3.c
 *
 * Created: 3/6/2024 14:11:10
 * Author : Barcala
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "dht11.h"
#include "lcd.h"


int main(void)
{
    /* Replace with your application code */
	
	LCDinit();
	_delay_ms(100);
	LCDhome();
    
	uint8_t h_t[4]={0,0,0,0};
	uint8_t flagCheck=0;
	while (1) 
    {
		flagCheck=leer(h_t);
		/*
		if(flagCheck){
			LCDclr();
			LCDGotoXY(0,0);
			LCDescribeDato(h_t[0],2);
			LCDGotoXY(0,1);
			LCDescribeDato(h_t[2],2);
		}*/

		_delay_ms(2000);
    }
}

