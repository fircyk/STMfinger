#include <stm32f446xx.h>
#include <system_stm32f4xx.h>
#include <stdbool.h>
#include <stm32f4xx.h>
#include "functions.h"
#include "sensor.h"
#include <stdio.h>

extern volatile uint8_t DMABufRX[100];
extern volatile uint8_t DMABufTX[100];

extern volatile uint8_t BufWrite[100];
extern volatile uint8_t BufRead[100];
int main(void){
	
	SysTick_Config(16000000/1000);
	
	
	LedConf();
	USART2ConfDMA();
	//USART2Conf();
	USART3Conf();
	USART3ConfDMA();
	uint16_t usercount;
	char str[100];
	uint16_t result;
	uint8_t asdf1[8]={245,42,0,0,0,0,42,245};
	uint8_t asdf2[8] = {245,9,0,0,0,0,9,245};
	
	MatchFingerprint();
	
//	AddUser1ID((uint8_t)usercount + 1);
	//AddUser2ID((uint8_t)usercount + 1);
	//AddUser3ID((uint8_t)usercount + 1);
	
	while(1){
		//
		
		
		delay_ms(1000);
		LedOnOff(red, LedTog);
				
		
	}
	
	
	return 0;
}




