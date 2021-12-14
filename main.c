#include <stm32f446xx.h>
#include <system_stm32f4xx.h>
#include <stdbool.h>
#include <stm32f4xx.h>
#include "functions.h"
#include "sensor.h"
#include <stdio.h>

extern volatile uint8_t DMABufRX[100];
extern volatile uint8_t BufRead[100];

extern volatile uint8_t DMABufTX[100];
extern volatile uint8_t BufWrite[100];

extern volatile bool NewData;
extern volatile bool SleepFlag;

int main(void){
	
	SysTick_Config(16000000/1000);
	
	B1Conf();
	LedConf();
	USART2ConfDMA();
	//USART2Conf();
	USART3Conf();
	USART3ConfDMA();
	uint16_t usercount;
	
	SleepFlag = false;
	
	uint8_t asdf1[8]={245,42,0,0,0,0,42,245};
	uint8_t asdf2[8] = {245,9,0,0,0,0,9,245};
	uint8_t asdf3[8] = {245,44,0,0,0,0,44,245};
	uint8_t testbuf[10];
	
	usercount = GetUserCount();
	delay_ms(500);
	/*
	
	
	AddUser1ID((uint8_t)usercount + 1);
	delay_ms(500);
	AddUser2ID((uint8_t)usercount + 1);
	delay_ms(500);
	AddUser3ID((uint8_t)usercount + 1);
	delay_ms(500);
	
	//GetUserCount();
	*/
	NewData=false;

	
	//usercount=GetUserCount();
	
	bool state = false;
	
	while(1){
		
		PCCommandAnalysis();
		
	
	}
	
	return 0;
}




