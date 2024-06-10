/*
 * rtc.h
 *
 * Created: 9/6/2024 20:36:19
 *  Author: perei
 */ 


#ifndef RTC_H_
#define RTC_H_

#define C_Ds3231ReadMode_U8   0xD1  // DS3231 ID
#define C_Ds3231WriteMode_U8  0xD0  // DS3231 ID
#define C_Ds3231SecondRegAddress_U8 0x00	//defines the register address for the seconds value in the DS3231 RTC module
#define C_Ds3231ControlRegAddress_U8 0x0E	//defines the register address for the control register in the DS3231 RTC module
//Everybody say gracias ChatGPT por las ultimas 2 direcciones!!!!

typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t weekDay;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}rtc_t;

void RTC_Init(void);

void RTC_SetDateTime(rtc_t *rtc);

void RTC_GetDateTime(rtc_t *rtc);

#endif /* RTC_H_ */