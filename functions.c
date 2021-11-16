#include "functions.h"

//test

volatile uint32_t tick = 0;
volatile uint32_t timeout = 0;
volatile uint8_t DMABufTX[100];
volatile uint8_t DMABufRX[100];

volatile uint8_t BufWrite[100];
volatile uint8_t BufRead[100];


void LedConf(void){
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER &= ~GPIO_MODER_MODER0;
	GPIOB->MODER |= GPIO_MODER_MODER0_0;
	GPIOB->MODER &= ~GPIO_MODER_MODER7;
	GPIOB->MODER |= GPIO_MODER_MODER7_0;
	GPIOB->MODER &= ~GPIO_MODER_MODER14;
	GPIOB->MODER |= GPIO_MODER_MODER14_0;
	
}


void LedOnOff(LedCol col, eLed state){
	
	switch(col){
		case red:
			if(state == LedOff) 			GPIOB->ODR &= ~GPIO_ODR_ODR_14;
			else if(state == LedOn) 	GPIOB->ODR |= GPIO_ODR_ODR_14;
			else if(state == LedTog) 	GPIOB->ODR ^= GPIO_ODR_ODR_14;
		break;
		case blue:
			if(state == LedOff) 			GPIOB->ODR &= ~GPIO_ODR_ODR_7;
			else if(state == LedOn) 	GPIOB->ODR |= GPIO_ODR_ODR_7;
			else if(state == LedTog) 	GPIOB->ODR ^= GPIO_ODR_ODR_7;
		break;
		case green:
			if(state == LedOff) 			GPIOB->ODR &= ~GPIO_ODR_ODR_0;
			else if(state == LedOn) 	GPIOB->ODR |= GPIO_ODR_ODR_0;
			else if(state == LedTog) 	GPIOB->ODR ^= GPIO_ODR_ODR_0;
		break;
	}
	
}




void SysTick_Handler(void){
	tick++;
	timeout++;
}


void delay_ms(uint32_t ms){
	tick = 0;
	while(tick<ms);
}

void delay_ms2(uint32_t ms){
	timeout = 0;
	while(timeout<ms);
}

void B1Conf(void){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	
	GPIOC->MODER &= ~GPIO_MODER_MODE13;
	

}	


eButton ButtonRead(void){
	
	eButton state = null;
	
	if((GPIOC->IDR & GPIO_IDR_ID13) != 0) state = click;
	return state;
}

bool B1Read(void){
	if((GPIOC->IDR & GPIO_IDR_ID13) != 0) return true;
	else return false;
}


void USART2Conf(void){
	
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //taktowanie
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;	//piny
	
	//piny alternate function
	GPIOD->MODER &= ~GPIO_MODER_MODER6 & ~GPIO_MODER_MODER5;
	GPIOD->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1;

	
	USART2->BRR = 16000000/19200;
	GPIOD->AFR[0] |= 0x07700000; //usart tx i rx na piny, alternate function
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE; //transmit and receive
	
	
}


uint8_t USART2GetChar(void){
	uint8_t temp;
	while((USART2->SR & USART_SR_RXNE)==RESET){;}
	temp = USART2->DR;
	return temp;
}


void USART2SendChar(char c)
{
    while(!(USART2->SR & USART_SR_TXE)){;}
        USART2->DR = c;
}

void USART2SendUINT(uint8_t* str){
		while(*str)
        USART2SendChar(*str++);
}

void USART2GetBufferRX(int byteCount, uint8_t* buffer){
	
	for(int i = 0; i<byteCount; i++){
		buffer[i]=USART2GetChar();
	}
	
}

void USART2ConfDMA(void){
	
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //taktowanie
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;	//piny
	RCC -> AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	
	//piny alternate function
	GPIOD->MODER &= ~GPIO_MODER_MODER6 & ~GPIO_MODER_MODER5;
	GPIOD->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1;
	
	
	USART2->BRR = 16000000/19200;
	USART2->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
	GPIOD->AFR[0] |= 0x07700000; //usart tx i rx na piny, alternate function
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE; //transmit and receive
	//USART2->CR1 |= USART_CR1_IDLEIE | USART_CR1_TCIE;
	USART2->CR1 |= USART_CR1_IDLEIE;
	
	NVIC_EnableIRQ(USART2_IRQn);
	
	DMA1_Stream6 -> PAR = (uint32_t)&USART2->DR;
	DMA1_Stream6 -> M0AR = (uint32_t)BufWrite;
	DMA1_Stream6 -> NDTR = (uint16_t)100;
	DMA1_Stream6 -> CR |= DMA_SxCR_CHSEL_2;
	DMA1_Stream6 -> CR |= DMA_SxCR_MINC | DMA_SxCR_DIR_0 | DMA_SxCR_TCIE;
	
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
	
	DMA1_Stream5 -> PAR = (uint32_t)&USART2->DR;
	DMA1_Stream5 -> M0AR = (uint32_t)BufRead;
	DMA1_Stream5 -> NDTR = (uint16_t)100;
	DMA1_Stream5 -> CR |= DMA_SxCR_CHSEL_2;
	//DMA1_Stream5 -> CR |= DMA_SxCR_MINC | DMA_SxCR_EN | DMA_SxCR_TCIE;
	DMA1_Stream5 -> CR |= DMA_SxCR_MINC | DMA_SxCR_EN;
	
	//NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	
}

void USART2ReinitDMA(void){
	
		DMA1_Stream5 -> CR &= ~DMA_SxCR_EN;
		DMA1_Stream5 -> NDTR = (uint16_t)100;
		DMA1_Stream5 -> CR |= DMA_SxCR_EN;
	
}


void USART2SendDMA(const char *str, uint16_t length){
	
		for(uint16_t i=0; i<length; i++){
				BufWrite[i]=str[i];
		}
		
		DMA1_Stream6 -> CR &= ~DMA_SxCR_EN;
		DMA1_Stream6 -> NDTR = (uint16_t)length;
		DMA1_Stream6 -> CR |= DMA_SxCR_EN;
		
}

void USART2SendDMAUINT(const uint8_t *str, uint16_t length){
	
	for(uint16_t i=0; i<length; i++){
				BufWrite[i]=str[i];
		}
		
		DMA1_Stream6 -> CR &= ~DMA_SxCR_EN;
		DMA1_Stream6 -> NDTR = (uint16_t)length;
		DMA1_Stream6 -> CR |= DMA_SxCR_EN;

}

void USART2_IRQHandler(void){
		
	if((USART2 -> SR & USART_SR_IDLE) != RESET){
				uint8_t temp = USART2 -> DR;
				USART2ReinitDMA();
		}
		/*
		if((USART2 -> SR & USART_SR_TC) != RESET){
			USART2 -> SR &= ~USART_SR_TC;
			//USART3ReinitDMA();
		}
	*/
}

void DMA1_Stream6_IRQHandler(void){

		if(DMA1 -> HISR & (DMA_HISR_TCIF6) ){
				DMA1 -> HIFCR |= DMA_HIFCR_CTCIF6;
				//USART2ReinitDMA();;
		//	while(!(USART3->SR & USART_SR_TC));
		}
		
		if((DMA1 -> HISR & DMA_HISR_HTIF6) != RESET){
				DMA1 -> HIFCR |= DMA_HIFCR_CHTIF6;
		}
		if((DMA1 -> HISR & DMA_HISR_FEIF6) != RESET){
				DMA1 -> HIFCR |= DMA_HIFCR_CFEIF6;
		}
	
}

void DMA1_Stream5_IRQHandler(void){

		if(DMA1 -> HISR & (DMA_HISR_TCIF5) ){
				DMA1 -> HIFCR |= DMA_HIFCR_CTCIF5;
				USART2ReinitDMA();
			//while(!(USART3->SR & USART_SR_TC));
		}
		/*
		if((DMA1 -> LISR & DMA_LISR_HTIF3) != RESET){
				DMA1 -> LIFCR |= DMA_LIFCR_CHTIF3;
		}
		if((DMA1 -> LISR & DMA_LISR_FEIF3) != RESET){
				DMA1 -> LIFCR |= DMA_LIFCR_CFEIF3;
		}
	*/
}


void USART3Conf(void)
{
		//usart enable
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN; 
		//pins
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
		//proper pins AF mode
	
	
    GPIOD->MODER &= ~GPIO_MODER_MODER8 & ~GPIO_MODER_MODER9;
    GPIOD->MODER |= GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1;
		//AF selection
    GPIOD->AFR[1] |= 0x00000077;
		//BR 57.6kbps
    USART3->BRR = 16000000/57600;
		//usart (receive, transmit) enable
    USART3->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
}




void USART3SendChar(char c)
{
    while(!(USART3->SR & USART_SR_TXE)){;}
        USART3->DR = c;
}

uint8_t USART3GetChar(void)
{
    uint8_t temp;
    while((USART3->SR & USART_SR_RXNE) == RESET){;}
    temp = USART3->DR;
    return temp;
}


void USART3SendString(char* str)
{
    while(*str)
        USART3SendChar(*str++);
}



void USART3GetBufferRX(int byteCount, uint8_t* buffer){
	
	for(int i = 0; i<byteCount; i++){
		buffer[i]=USART3GetChar();
	}
	
}


void USART3ConfDMA(void)
{
		//usart enable
    RCC -> APB1ENR |= RCC_APB1ENR_USART3EN; 
		//pins
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
		//DMA enable
	RCC -> AHB1ENR |= RCC_AHB1ENR_DMA1EN;
		//proper pins AF mode
    GPIOD -> MODER &= ~GPIO_MODER_MODER8 & ~GPIO_MODER_MODER9;
    GPIOD -> MODER |= GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1;
		//AF selection
    GPIOD -> AFR[1] |= 0x00000077;
		//BR 57.6kbps
    USART3 -> BRR = 16000000/57600;
		//DMA transmit and receive enable
			USART3->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
		//usart (receive, transmit) enable
    USART3 -> CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	USART3 -> CR1 |= USART_CR1_IDLEIE | USART_CR1_TCIE;
	NVIC_EnableIRQ(USART3_IRQn);
	//stream 1 channel 4 usart 3TX, stream3 channel4 usart3TX
	DMA1_Stream3 -> PAR = (uint32_t)&USART3->DR;
	DMA1_Stream3 -> M0AR = (uint32_t)DMABufTX;
	DMA1_Stream3 -> NDTR = (uint16_t)100;
	DMA1_Stream3 -> CR |= DMA_SxCR_CHSEL_2;
	DMA1_Stream3 -> CR |= DMA_SxCR_MINC | DMA_SxCR_DIR_0 | DMA_SxCR_TCIE;
	
	NVIC_EnableIRQ(DMA1_Stream3_IRQn);
	

	DMA1_Stream1 -> PAR = (uint32_t)&USART3->DR;
	DMA1_Stream1 -> M0AR = (uint32_t)DMABufRX;
	DMA1_Stream1 -> NDTR = (uint16_t)100;
	DMA1_Stream1 -> CR |= DMA_SxCR_CHSEL_2;
	DMA1_Stream1 -> CR |= DMA_SxCR_MINC | DMA_SxCR_EN | DMA_SxCR_TCIE;
	NVIC_EnableIRQ(DMA1_Stream1_IRQn);
	
	
}


void USART3ReinitDMA(void){
	
		DMA1_Stream1 -> CR &= ~DMA_SxCR_EN;
		DMA1_Stream1 -> NDTR = (uint16_t)100;
		DMA1_Stream1 -> CR |= DMA_SxCR_EN;
	
}
void USART3SendUINT(uint8_t* str){
		while(*str)
        USART3SendChar(*str++);
}

void USART3SendDMA(const char *str, uint16_t length){
		
		for(uint16_t i=0; i<length; i++){
				DMABufTX[i]=str[i];
		}
		
		DMA1_Stream3 -> CR &= ~DMA_SxCR_EN;
		DMA1_Stream3 -> NDTR = (uint16_t)length;
		DMA1_Stream3 -> CR |= DMA_SxCR_EN;
		
}

void USART3SendDMAUINT(const uint8_t *str, uint16_t length){
	
	for(uint16_t i=0; i<length; i++){
				DMABufTX[i]=str[i];
		}
		
		DMA1_Stream3 -> CR &= ~DMA_SxCR_EN;
		DMA1_Stream3 -> NDTR = (uint16_t)length;
		DMA1_Stream3 -> CR |= DMA_SxCR_EN;

}

// ustawia sie tcif3 htif3 feif3 (LISR)
//CLEAR W LIFCR


void USART3_IRQHandler(void){
		if((USART3 -> SR & USART_SR_IDLE) != RESET){
			uint8_t temp = USART3 -> DR;
			USART3ReinitDMA();
				
		}
		
	
		if((USART3 -> SR & USART_SR_TC) != RESET){
			USART3 -> SR &= ~USART_SR_TC;
			//USART3ReinitDMA();
		}
	
}

void DMA1_Stream3_IRQHandler(void){

		if(DMA1 -> LISR & (DMA_LISR_TCIF3) ){
				DMA1 -> LIFCR |= DMA_LIFCR_CTCIF3;
		//	while(!(USART3->SR & USART_SR_TC));
		}
		
		if((DMA1 -> LISR & DMA_LISR_HTIF3) != RESET){
				DMA1 -> LIFCR |= DMA_LIFCR_CHTIF3;
		}
		if((DMA1 -> LISR & DMA_LISR_FEIF3) != RESET){
				DMA1 -> LIFCR |= DMA_LIFCR_CFEIF3;
		}
	
}

void DMA1_Stream1_IRQHandler(void){

		if(DMA1 -> LISR & (DMA_LISR_TCIF1) ){
				DMA1 -> LIFCR |= DMA_LIFCR_CTCIF1;
				USART3ReinitDMA();
			//while(!(USART3->SR & USART_SR_TC));
		}
		/*
		if((DMA1 -> LISR & DMA_LISR_HTIF3) != RESET){
				DMA1 -> LIFCR |= DMA_LIFCR_CHTIF3;
		}
		if((DMA1 -> LISR & DMA_LISR_FEIF3) != RESET){
				DMA1 -> LIFCR |= DMA_LIFCR_CFEIF3;
		}
	*/
}













