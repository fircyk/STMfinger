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


extern volatile bool SleepFlag;

int main(void){
	
	SysTick_Config(16000000/1000);
	
	B1Conf();
	LedConf();
	
	USART2ConfDMA();
	USART3ConfDMA();
	
	
	SleepFlag = false;
	
	
	while(1){
		
		PCCommandAnalysis();
		
	}
	
	return 0;
}




