#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "dht11.h"




void inicializar_sensor(){
	DDRC |= (1 << PINC0);		//Se setea el PINC0 como salida
	PORTC &= ~(1 << PINC0);		//Se envia un nivel logico bajo por el pin
	_delay_ms(20);
	PORTC |= (1 << PINC0);		//Se envia un nivel logico alto por ele pin
	_delay_us(30);
	DDRC &= ~(1 << PINC0);		//Se setea el PINC0 como entrada
	_delay_us(160);
}

uint8_t leer(uint8_t* res){
	uint8_t i, checksum= 0, aux = 0, posAct = 0, numAct = 0, salidaAct = 0, salidaAnt = 0;
	DDRC |= (1 << PINC3);
	inicializar_sensor();		//Se inicializa el sensor
	while(salidaAct != 1){		//Se espera un flanco de subida (No se necesita chequear que el valor anterior sea 0 porque ya sabemos que lo és en el primer caso)
		salidaAct = (PINC & (1 << PINC0));	//Pongo la salida actual en 1
	}
	salidaAnt = 1;
	for(i = 0; i<32; i++){			//Repito por los primeros 4 numeros
		_delay_us(50);
		aux |= ((PINC & (1 << PINC0)))<<(7-posAct);		//Me quedo con el valor del PINC0 y voy asignandolo a la posición del numero que corresponde
		if(++posAct == 8){								//Si ya leí 8 bits del mismo numero es que termine
			res[numAct++] = aux;						//Seteo a mi numero auxiliar al vector a devolver
			aux = 0;									//Vuelvo el auxiliar a 0
			posAct = 0;
		}
		while(!(salidaAct == 1 && salidaAnt == 0)){		//Repito mientras no hay flanco de subida
			salidaAnt = salidaAct;
			salidaAct = (PINC & (1 << PINC0));
		}
		PORTC |= (1 << PINC3);
		_delay_us(1);
		PORTC &= ~(1 << PINC3);
		salidaAnt = 1;
	}
	for(i = 0; i < 8; i++){							//Repito procedimiento de arriba para el checksum
		_delay_us(50);
		checksum |= ((PINC & (1 << PINC0)))<<(7-i);
		while(!(salidaAct == 1 && salidaAnt == 0)){
			salidaAnt = salidaAct;
			salidaAct = (PINC & (1 << PINC0));
		}
		salidaAnt = 1;
	}
	aux = (res[0] + res[1] + res[2] + res[3]);		//sumo todos para hacer el checksum
	return (checksum == aux);						//Devuelvo si el checksum es igual a la suma de todos los numeros
}
