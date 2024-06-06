/*
 * dht11.c
 *
 * Created: 3/6/2024 14:12:15
 *  Author: Barcala
 */ 
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "lcd.h"//asdasdasdsa
#include "dht11.h"




void inicializar_sensor(){
	DDRC |= (1 << PINC0);
	PORTC &= ~(1 << PINC0);
	_delay_ms(20);
	PORTC |= (1 << PINC0);
	_delay_us(30);
	DDRC &= ~(1 << PINC0);
	_delay_us(160);
}

uint8_t leer(uint8_t* res){
	uint8_t i, checksum= 0, aux = 0, posAct = 0, numAct = 0;
	DDRC |= (1 << PINC1); 
	inicializar_sensor();
	_delay_us(50);
	PORTC |= (1 << PINC1);
	_delay_us(1);
	PORTC &= ~(1 << PINC1);
	for(i = 0; i<32; i++){
		_delay_us(50);
		aux |= ((PINC & (1 << PINC0)))<<(posAct);
		PORTC |= (1 << PINC1);
		_delay_us(1);
		PORTC &= ~(1 << PINC1);
		if(++posAct == 8){
			res[numAct++] = aux;
			aux = 0;
			posAct = 0;
		}
		if(aux & (1 <<(i%8))){
			_delay_us(69);
		}else{
			_delay_us(19);
		}
	}
	for(i = 0; i < 8; i++){
		_delay_us(50);
		checksum |= ((PINC & (1 << PINC0)))<<(i%8);
		if(checksum & (1 <<(i%8))){
			_delay_us(70);
		}else{
			_delay_us(30);
		}
	}
	aux = (res[0] + res[1] + res[2] + res[3]);
	LCDclr();
	LCDhome();
	LCDescribeDato(res[2],3);
	LCDescribeDato(checksum,3);
	return (checksum == aux);
}
