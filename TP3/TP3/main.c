#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "dht11.h"
#include "rtc.h"
#define BR9600 (0x67)	// 0x67=103 configura BAUDRATE=9600 a 16MHz

volatile uint8_t RX_Buffer=0;		// Variables que se modificaran cuando se atienda la interrupcion
rtc_t hora;
volatile uint8_t flagFin=0;

//prototipos de funciones
uint8_t obtenerMes(const uint8_t*);
void obtenerFechaHoraCompilacion(rtc_t*);


int main(void)
{	
	
	SerialPort_Init(BR9600); 		// Inicializo formato 8N1 y BAUDRATE = 9600bps
	SerialPort_TX_Enable();			// Activo el Transmisor del Puerto Serie (1<<TXEN0) en UCSR0B
	SerialPort_RX_Enable();			// Activo el Receptor del Puerto Serie	(1<<RXEN0) en UCSR0B
	SerialPort_RX_Interrupt_Enable();	// Activo Interrupci�n de recepcion.
	SerialPort_Send_String("Medicion de temperatura y humedad. Presionando 's' o 'S' se finaliza el programa de medicion.\r\n");   // Env�o el mensaje de Bienvenida

	obtenerFechaHoraCompilacion(&hora);
	
	RTC_Init();
	RTC_SetDateTime(&hora);
	
	sei();
	
	uint8_t h_t[4]={0,0,0,0};
	uint8_t cadenaT[3],cadenaH[3],fechayhora[100];
	uint8_t flagCheck=0;
	uint8_t msg[100];
	while (1) 
    {
		flagCheck=leer(h_t);
	
		if(flagCheck && !flagFin){
			RTC_GetDateTime(&hora);
			sprintf(fechayhora, " FECHA: %02d/%02d/%02d HORA: %02d:%02d:%02d\r\n", hora.year,hora.month,hora.date,hora.hour,hora.min,hora.sec);	
			sprintf(cadenaT, "%d", h_t[2]);			//transformo los enteros a cadena de caracteres
			sprintf(cadenaH, "%d", h_t[0]);			
			strcpy(msg, "TEMP: ");					// uso strcpy para iniciar el mensaje
			strcat(msg, cadenaT);
			strcat(msg, "�C HUM: ");
			strcat(msg, cadenaH);
			strcat(msg, fechayhora);
			SerialPort_Send_String(msg);			//mando el mensaje a la terminal
		}
		else if(!flagFin)
			SerialPort_Send_String("Error en la transmision. (Fallo el Checksum)\r\n");

		_delay_ms(2000);
	}
}
ISR(USART_RX_vect){
	RX_Buffer = UDR0; //leo el dato
	if((RX_Buffer == 's') || (RX_Buffer == 'S')){		//si se ingres� s o S
		if(!flagFin){
			SerialPort_Send_String("Se finaliz� la medicion. \r\n");
			flagFin=1;
		}
		else{
			SerialPort_Send_String("Se reanud� la medicion. \r\n");
			flagFin=0;
		}
	}
	RX_Buffer=0;  // reseteo el buffer que guarda el dato leido
}

uint8_t obtenerMes(const char *mes) {
	if (strcmp(mes, "Jan") == 0) return 1;
	if (strcmp(mes, "Feb") == 0) return 2;
	if (strcmp(mes, "Mar") == 0) return 3;
	if (strcmp(mes, "Apr") == 0) return 4;
	if (strcmp(mes, "May") == 0) return 5;
	if (strcmp(mes, "Jun") == 0) return 6;
	if (strcmp(mes, "Jul") == 0) return 7;
	if (strcmp(mes, "Aug") == 0) return 8;
	if (strcmp(mes, "Sep") == 0) return 9;
	if (strcmp(mes, "Oct") == 0) return 10;
	if (strcmp(mes, "Nov") == 0) return 11;
	if (strcmp(mes, "Dec") == 0) return 12;
	return 0; // En caso de error
}

void obtenerFechaHoraCompilacion(rtc_t *hora){
	uint16_t ano;
	// _DATE_ y _TIME_ son macros predefinidos por el compilador
	char date[] = __DATE__; // Formato: "Jun 10 2024"
	char time[] = __TIME__; // Formato: "14:30:59"

	// Descomponer la fecha
	char mes_str[4];
	sscanf(date, "%s %d %lu ", mes_str, &(hora->date), &ano);
	(hora->year)= ano%100;
	hora->month = obtenerMes(mes_str);
	
	// Descomponer la hora
	sscanf(time, "%hhu:%hhu:%hhu", &(hora->hour), &(hora->min), &(hora->sec));
}
