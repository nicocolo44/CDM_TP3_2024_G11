/*
 * TP3.c
 *
 * Created: 3/6/2024 14:11:10
 * Author : Barcala
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "dht11.h"
#define BR9600 (0x67)	// 0x67=103 configura BAUDRATE=9600 a 16MHz

volatile static uint8_t contador=0;
volatile uint8_t RX_Buffer=0;		// Variables que se modificara cuando se atienda la interrupcion

uint8_t* convNumaCar(uint8_t);

int main(void)
{	
	
    SerialPort_Init(BR9600); 		// Inicializo formato 8N1 y BAUDRATE = 9600bps
    SerialPort_TX_Enable();			// Activo el Transmisor del Puerto Serie (1<<TXEN0) en UCSR0B
    SerialPort_RX_Enable();			// Activo el Receptor del Puerto Serie	(1<<RXEN0) en UCSR0B
	SerialPort_RX_Interrupt_Enable();	// Activo Interrupción de recepcion.
	SerialPort_Send_String("Medicion de temperatura y humedad. Presionando 3 veces s o S se finaliza el programa de medicion.\n");   // Envío el mensaje de Bienvenida

	sei();
	
	uint8_t h_t[4]={0,0,0,0};
	uint8_t cadenaT[3],cadenaH[3];
	uint8_t flagCheck=0,flagFin=0;
	uint8_t msg[100];
	while (1) 
    {
		flagCheck=leer(h_t);
	
		if(flagCheck && !flagFin){
			sprintf(cadenaT, "%d", h_t[2]);			//transformo los enteros a cadena de caracteres
			sprintf(cadenaH, "%d", h_t[0]);			
			strcpy(msg, "TEMP: ");					// uso strcpy para iniciar el mensaje
			strcat(msg, cadenaT);
			strcat(msg, "°C HUM: ");
			strcat(msg, cadenaH);
			strcat(msg, "% FECHA: - HORA: -\r\n");	//despues se va a tener que poner la fecha y hora con el otro periferico q usa i2c
			SerialPort_Send_String(msg);			//mando el mensaje a la terminal
		}
		else if(!flagFin)
			SerialPort_Send_String("Error en la transmision. (Fallo el Checksum)\r\n");
		
		
		if(contador > 3 ){
			if(!flagFin){
				SerialPort_Send_String("Se finalizó la medicion. \r\n");
				flagFin=1;
				contador=0;
				}
			else{
				SerialPort_Send_String("Se reanudó la medicion. \r\n");
				flagFin=0;
				contador=0;	
			}
		}

		_delay_ms(2000);
	}
}
ISR(USART_RX_vect){
	RX_Buffer = UDR0; //la lectura del UDR borra flag RXC
	if((RX_Buffer == 's') || (RX_Buffer == 'S')){
		contador++;
	}
		
}

