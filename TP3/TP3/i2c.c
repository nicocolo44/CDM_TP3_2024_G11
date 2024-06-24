#include "i2c.h"

void i2c_init(void){
	TWSR=0x00;		//prescaler en 0
	TWBR=152;		//valor para que la Fscl sea 50KHz con XTAL=16MHz
	TWCR=0x04;		//Habilito el modo TWI
}

void i2c_start(void){		//Envia start y espera confirmacion
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR & (1<<TWINT))==0);
}

void i2c_write(uint8_t data){
	TWDR= data; //Copia dato a enviar
	TWCR=(1<<TWINT)|(1<<TWEN);	//Pone en 1 para iniciar transmision
	while((TWCR & (1<<TWINT))==0);	//Espera activacion de flag TWINT para ver transmision completada
}

uint8_t i2c_read(uint8_t isLast){
	/*
		TWEN=1 y TWINT =1 para iniciar la recepción
		• Esperar activación del flag TWINT (reception complete) y leer el dato desde el TWDR
	*/
	if(isLast==0)	//Send ACK
		TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);	//Send ACK
	else	
		TWCR=(1<<TWINT)|(1<<TWEN); //Send NACK
	while((TWCR & (1<<TWINT))==0);
	return TWDR;
}

void i2c_stop(){		//No usar TWINT luego de enviar el stop
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}