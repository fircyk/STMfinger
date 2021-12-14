#ifndef _FUNCTIONS
#define _FUNCTIONS


#include <stm32f446xx.h>
#include <system_stm32f4xx.h>
#include <stdbool.h>
#include <stm32f4xx.h>

typedef enum{LedOff = 0, LedOn = 1, LedTog = 2}eLed;
typedef enum{red = 0, blue = 1, green = 2}LedCol;
typedef enum{null = 0, click = 1}eButton;

void delay_ms(uint32_t ms);
void delay_ms2(uint32_t ms);
void LedConf(void);
void LedOnOff(LedCol col, eLed state);
void SysTick_Handler(void);

void B1Conf(void);
eButton ButtonRead(void);
bool B1Read(void);


void USART2Conf(void);
void USART2SendChar(char c);
uint8_t USART2GetChar(void);
void USART2SendString(char* str);
void USART2SendUINT(uint8_t* str);
void USART2GetBufferRX(int byteCount, uint8_t* buffer);
   
void USART2ConfDMA(void);
void USART2ReinitDMA(void);
void USART2SendDMA(const char *str, uint16_t length);	
void USART2SendDMAUINT(const uint8_t *str, uint16_t length);

void USART2_IRQHandler(void);
void DMA1_Stream6_IRQHandler(void);
//void DMA1_Stream5_IRQHandler(void);

void USART3Conf(void);
void USART3SendChar(char c);
uint8_t USART3GetChar(void);
void USART3SendString(char* str);
void USART3SendUINT(uint8_t* str);
void USART3GetBufferRX(int byteCount, uint8_t* buffer);

void USART3ConfDMA(void);
void USART3ReinitDMA(void);
void USART3SendDMA(const char *str, uint16_t length);
void USART3SendDMAUINT(const uint8_t *str, uint16_t length);

void USART3_IRQHandler(void);
void DMA1_Stream3_IRQHandler(void);
//void DMA1_Stream1_IRQHandler(void);

void ZeroDMABufRX(void);






#endif
