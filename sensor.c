#include "sensor.h"
#include "functions.h"
#include <stdio.h>

uint8_t bufferTX[9];
uint8_t bufferRX[9];

extern volatile uint32_t timeout;

extern volatile uint8_t DMABufRX[100];
extern volatile uint8_t BufRead[100];

extern volatile uint8_t DMABufTX[100];
extern volatile uint8_t BufWrite[100];

volatile uint16_t match;
volatile uint16_t UserCount;
volatile bool SleepFlag;

uint8_t CountRXCheckSum(void){
		char buf[2];
		uint8_t RXCheckSum = 0;
	
		for(int i = 1; i<7; i++){
			RXCheckSum ^= BufRead[i];
			USART3SendString("\r\n kolejna checksuma: ");
			sprintf(buf, "%d", BufRead[i]);
			USART3SendString(buf);
		}
	return RXCheckSum;
}	

uint16_t GetUserCount(){
	
		uint8_t temp;
		uint8_t CheckSum=0;
	
		ZeroDMABufRX();
	
		char buff[5];
		bufferTX[0]=CMD_HEAD;
		bufferTX[1]=CMD_USER_CNT;
		bufferTX[2]=0;
		bufferTX[3]=0;
		bufferTX[4]=0;
		bufferTX[5]=0;
		for(int i = 1; i<6; i++){
			CheckSum ^= bufferTX[i];
		}
		bufferTX[6]=CheckSum;
		bufferTX[7]=CMD_TAIL;
		
		uint8_t highCountNumber;
		uint8_t lowCountNumber;
		
		uint16_t UserCount;
		
		USART2SendDMAUINT(bufferTX, 8);
		
		delay_ms2(200);
		
		CheckSum = CountRXCheckSum();
		if(CheckSum != 0){
				USART3SendString("\r\n CHECKSUM FAULT");
				return ACK_FAIL;
		}
		if(BufRead[4] == ACK_SUCCESS && bufferTX[1] == BufRead[1]){
			highCountNumber = BufRead[2];
			lowCountNumber = BufRead[3];
			UserCount = lowCountNumber + (highCountNumber << 8);
			
			sprintf(buff, "%d", UserCount);
			USART3SendString("\r\n SUCC ID: ");
			USART3SendString(buff);
			
			return UserCount;
			
		}else{
			USART3SendString("tu jestem\r\n");
			sprintf(buff, "%d", BufRead[4]);
			USART3SendString(buff);
			USART3SendString("\r\n");
			sprintf(buff, "%d", BufRead[1]);
			USART3SendString(buff);
			USART3SendString("\r\n");
			return 0xFF;
			
		}
		
	}
	
//to be redesigned
uint8_t AddUser1(void){

		uint16_t temp;
		uint8_t CheckSum=0;
	
		temp = GetUserCount();
		temp = temp+1;
		uint8_t tempHigh = temp >> 8;
		uint8_t tempLow = temp-(256*tempHigh);
	
		bufferTX[0]=CMD_HEAD;
		bufferTX[1]=CMD_ADD_1;
		bufferTX[2]=tempHigh;
		bufferTX[3]=tempLow;
		bufferTX[4]=1;
		bufferTX[5]=0;
		for(int i = 1; i<6; i++){
			CheckSum ^= bufferTX[i];
		}
		bufferTX[6]=CheckSum;
		bufferTX[7]=CMD_TAIL;
		
		USART2SendDMAUINT(bufferTX, 8);
	
		delay_ms2(2000);
		
		CheckSum = CountRXCheckSum();
		if(CheckSum != 0){
				USART3SendString("\r\n CHECKSUM FAULT");
				return ACK_FAIL;
		}
		
		if(BufRead[4]==ACK_SUCCESS && bufferTX[1] == BufRead[1]){
				USART3SendString("\r\n ADD1 succesful");
				return ACK_SUCCESS;
		}else if(BufRead[4]==ACK_FAIL){
				USART3SendString("\r\n ADD1 FAIL");
				return ACK_FAIL;
		}else if(BufRead[4]==ACK_FULL){
				USART3SendString("\r\n ADD1 FULL");
				return ACK_FULL;
		}else if(BufRead[4]==ACK_USER_OCCUPIED){
				USART3SendString("\r\n ADD1 user occupied");
				return ACK_USER_OCCUPIED;
		}else if(BufRead[4]==ACK_FINGER_OCCUPIED){
				USART3SendString("\r\n ADD1 finger occupied");
				return ACK_FINGER_OCCUPIED;
		}else if(BufRead[4]==ACK_TIMEOUT){
				USART3SendString("\r\n ADD1 timeout");
				return ACK_TIMEOUT;
		}
		
	
	
}

uint8_t AddUser1ID(uint8_t ID){

		//uint16_t temp;
		uint8_t CheckSum=0;
	
		ZeroDMABufRX();
		//temp = GetUserCount();
		//temp = temp+1;
		uint8_t tempHigh = ID >> 8;
		uint8_t tempLow = ID-(256*tempHigh);
	
		bufferTX[0]=CMD_HEAD;
		bufferTX[1]=CMD_ADD_1;
		bufferTX[2]=tempHigh;
		bufferTX[3]=tempLow;
		bufferTX[4]=1;
		bufferTX[5]=0;
		for(int i = 1; i<6; i++){
			CheckSum ^= bufferTX[i];
		}
		bufferTX[6]=CheckSum;
		bufferTX[7]=CMD_TAIL;
		
		USART2SendDMAUINT(bufferTX, 8);
	
		delay_ms2(4000);
		
		CheckSum = CountRXCheckSum();
		if(CheckSum != 0){
				USART3SendString("\r\n CHECKSUM FAULT");
				return ACK_FAIL;
		}
		
		if(BufRead[4]==ACK_SUCCESS && bufferTX[1] == BufRead[1]){
				USART3SendString("\r\n ADD1 succesful");
				return ACK_SUCCESS;
		}else if(BufRead[4]==ACK_FAIL){
				USART3SendString("\r\n ADD1 FAIL");
				return ACK_FAIL;
		}else if(BufRead[4]==ACK_FULL){
				USART3SendString("\r\n ADD1 FULL");
				return ACK_FULL;
		}else if(BufRead[4]==ACK_USER_OCCUPIED){
				USART3SendString("\r\n ADD1 user occupied");
				return ACK_USER_OCCUPIED;
		}else if(BufRead[4]==ACK_FINGER_OCCUPIED){
				USART3SendString("\r\n ADD1 finger occupied");
				return ACK_FINGER_OCCUPIED;
		}else if(BufRead[4]==ACK_TIMEOUT){
				USART3SendString("\r\n ADD1 timeout");
				return ACK_TIMEOUT;
		}else if(BufRead[4]==ACK_GO_OUT){
				USART3SendString("\r\n ADD1 go out");
				return ACK_GO_OUT;
		}else{
			USART3SendString("\r\n ADD1 unknown error");
			return ACK_FAIL;
		}
		
}


uint8_t AddUser3ID(uint8_t ID){

		//uint16_t temp;
		uint8_t CheckSum=0;
	
		
		//temp = GetUserCount();
		//temp = temp+1;
		uint8_t tempHigh = ID >> 8;
		uint8_t tempLow = ID-(256*tempHigh);
	
		bufferTX[0]=CMD_HEAD;
		bufferTX[1]=CMD_ADD_3;
		bufferTX[2]=tempHigh;
		bufferTX[3]=tempLow;
		bufferTX[4]=1;
		bufferTX[5]=0;
		for(int i = 1; i<6; i++){
			CheckSum ^= bufferTX[i];
		}
		bufferTX[6]=CheckSum;
		bufferTX[7]=CMD_TAIL;
		
		USART2SendDMAUINT(bufferTX, 8);
	
		delay_ms2(4000);
		
		CheckSum = CountRXCheckSum();
		if(CheckSum != 0){
				USART3SendString("\r\n CHECKSUM FAULT");
				return ACK_FAIL;
		}
		
		if(BufRead[4]==ACK_SUCCESS && bufferTX[1] == BufRead[1]){
				USART3SendString("\r\n ADD3 succesful");
				return ACK_SUCCESS;
		}else if(BufRead[4]==ACK_FAIL){
				USART3SendString("\r\n ADD3 FAIL");
				return ACK_FAIL;
		}else if(BufRead[4]==ACK_FULL){
				USART3SendString("\r\n ADD3 FULL");
				return ACK_FULL;
		}else if(BufRead[4]==ACK_USER_OCCUPIED){
				USART3SendString("\r\n ADD3 user occupied");
				return ACK_USER_OCCUPIED;
		}else if(BufRead[4]==ACK_FINGER_OCCUPIED){
				USART3SendString("\r\n ADD3 finger occupied");
				return ACK_FINGER_OCCUPIED;
		}else if(BufRead[4]==ACK_TIMEOUT){
				USART3SendString("\r\n ADD3 timeout");
				return ACK_TIMEOUT;
		}else{
			USART3SendString("\r\n ADD3 unknown error");
			return ACK_FAIL;
		}
		
}
uint8_t AddUser2ID(uint8_t ID){

		//uint16_t temp;
		uint8_t CheckSum=0;
	
		//temp = GetUserCount();
		//temp = temp+1;
		uint8_t tempHigh = ID >> 8;
		uint8_t tempLow = ID-(256*tempHigh);
	
		bufferTX[0]=CMD_HEAD;
		bufferTX[1]=CMD_ADD_2;
		bufferTX[2]=tempHigh;
		bufferTX[3]=tempLow;
		bufferTX[4]=1;
		bufferTX[5]=0;
		for(int i = 1; i<6; i++){
			CheckSum ^= bufferTX[i];
		}
		bufferTX[6]=CheckSum;
		bufferTX[7]=CMD_TAIL;
		
		USART2SendDMAUINT(bufferTX, 8);
	
		delay_ms2(4000);
		
		CheckSum = CountRXCheckSum();
		if(CheckSum != 0){
				USART3SendString("\r\n CHECKSUM FAULT");
				return ACK_FAIL;
		}
		
		if(BufRead[4]==ACK_SUCCESS && bufferTX[1] == BufRead[1]){
				USART3SendString("\r\n ADD2 succesful");
				return ACK_SUCCESS;
		}else if(BufRead[4]==ACK_FAIL){
				USART3SendString("\r\n ADD2 FAIL");
				return ACK_FAIL;
		}else if(BufRead[4]==ACK_FULL){
				USART3SendString("\r\n ADD2 FULL");
				return ACK_FULL;
		}else if(BufRead[4]==ACK_USER_OCCUPIED){
				USART3SendString("\r\n ADD2 user occupied");
				return ACK_USER_OCCUPIED;
		}else if(BufRead[4]==ACK_FINGER_OCCUPIED){
				USART3SendString("\r\n ADD2 finger occupied");
				return ACK_FINGER_OCCUPIED;
		}else if(BufRead[4]==ACK_TIMEOUT){
				USART3SendString("\r\n ADD2 timeout");
				return ACK_TIMEOUT;
		}else{
			USART3SendString("\r\n ADD2 unknown error");
			return ACK_FAIL;
		}
		
}


uint8_t AddUserID(uint8_t ID){
	
	uint8_t temp;
	
	ZeroDMABufRX();
	
	temp = AddUser1ID(ID);
	if(temp == ACK_SUCCESS){
			USART3SendString("\r\n ADD1 succ");
			temp = AddUser2ID(ID);
			if(temp == ACK_SUCCESS){
					USART3SendString("\r\n ADD2 succ");
					temp = AddUser3ID(ID);
					if(temp == ACK_SUCCESS){
							USART3SendString("\r\n ADD3 succ");
					}else{
							char errbuf[10];
							sprintf(errbuf, " \t %d", BufRead[4]);
							USART3SendString("\r\n ADD3 error code");
							USART3SendString(errbuf);
							return BufRead[4];
					}
			}else{
					char errbuf[10];
					sprintf(errbuf, " \t %d", BufRead[4]);
					USART3SendString("\r\n ADD2 error code");
					USART3SendString(errbuf);
					return BufRead[4];
			}
	}else{
			char errbuf[10];
			sprintf(errbuf, " \t %d", BufRead[4]);
			USART3SendString("\r\n ADD1 error code");
			USART3SendString(errbuf);
			return BufRead[4];
	}

	
}


uint8_t DeleteAllUsers(void){
		
		uint8_t CheckSum=0;
	
		ZeroDMABufRX();
	
		bufferTX[0]=CMD_HEAD;
		bufferTX[1]=CMD_DEL_ALL;
		bufferTX[2]=0;
		bufferTX[3]=0;
		bufferTX[4]=0;
		bufferTX[5]=0;
		for(int i = 1; i<6; i++){
			CheckSum ^= bufferTX[i];
		}
		bufferTX[6]=CheckSum;
		bufferTX[7]=CMD_TAIL;
		
		USART2SendDMAUINT(bufferTX, 8);
		delay_ms2(500);
		
		CheckSum = CountRXCheckSum();
		if(CheckSum != 0){
				USART3SendString("\r\n CHECKSUM FAULT");
				return ACK_FAIL;
		}
		
		if(BufRead[4] == ACK_SUCCESS && bufferTX[1] == BufRead[1]){
				USART3SendString("\r\n DELETE ALL SUCC");
				return ACK_SUCCESS;
		}			
		else{
				USART3SendString("\r\n DELETE ALL FAIL");
				return ACK_FAIL;
		}
}
	
uint8_t DeleteUser(uint16_t ID){
		
	
		uint8_t CheckSum=0;
		
		ZeroDMABufRX();
	
		uint8_t highID = ID >> 8;
		uint8_t lowID = (ID & 0xFF);
		
		bufferTX[0]=CMD_HEAD;
		bufferTX[1]=CMD_DEL;
		bufferTX[2]=highID;
		bufferTX[3]=lowID;
		bufferTX[4]=0;
		bufferTX[5]=0;
		for(int i = 1; i<6; i++){
			CheckSum ^= bufferTX[i];
		}
		bufferTX[6]=CheckSum;
		bufferTX[7]=CMD_TAIL;
		
		USART2SendDMAUINT(bufferTX, 8);
	
		delay_ms2(400);
		
	if(BufRead[4] == ACK_SUCCESS && bufferTX[1] == BufRead[1]){
				USART3SendString("\r\n DELETE ID SUCC");
				return ACK_SUCCESS;
		}			
		else{
				USART3SendString("\r\n DELETE ID FAIL");
				return ACK_FAIL;
		}
		
}

uint16_t MatchFingerprint(void){

		uint8_t CheckSum=0;
		
		ZeroDMABufRX();
	
		//uint16_t match;
		char bufff[10];
	
		bufferTX[0]=CMD_HEAD;
		bufferTX[1]=CMD_MATCH;
		bufferTX[2]=0;
		bufferTX[3]=0;
		bufferTX[4]=0;
		bufferTX[5]=0;
		for(int i = 1; i<6; i++){
			CheckSum ^= bufferTX[i];
		}
		bufferTX[6]=CheckSum;
		bufferTX[7]=CMD_TAIL;
		
		USART2SendDMAUINT(bufferTX, 8);
		delay_ms2(1000);
		
		CheckSum = CountRXCheckSum();
		if(CheckSum != 0){
				USART3SendString("\r\n CHECKSUM FAULT");
				return ACK_FAIL;
		}else if(BufRead[4] == ACK_NO_USER){
				USART3SendString("\r\n MATCH NO USER");
				return ACK_NO_USER;
		}else if(BufRead[4] == ACK_TIMEOUT){
			USART3SendString("\r\n TIMEOUT");
			return ACK_TIMEOUT;
		}else if(BufRead[4] == ACK_GO_OUT){
			USART3SendString("\r\n FINGER OUT");
			return ACK_GO_OUT;
		}else if(bufferTX[1] == BufRead[1] && BufRead[4]<=3){
				match = (BufRead[2] << 8) + BufRead[3];
				sprintf(bufff, "%d", match);
				USART3SendString("\r\n Matched ID: ");
				USART3SendString(bufff);
				sprintf(bufff, "%d", BufRead[4]);
				USART3SendString("\r\n Matched permissionn: ");
				USART3SendString(bufff);
			return ACK_SUCCESS;
		}else{
			USART3SendString("\r\n MATCH unknown error");
			return ACK_FAIL;
		}

}

void ConfAutoMode(void){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	
	GPIOG -> MODER &= ~GPIO_MODER_MODER0;
	GPIOG -> MODER |= GPIO_MODER_MODER0_0;
	GPIOG -> OTYPER &= ~GPIO_OTYPER_OT0;
	GPIOG -> OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR0;
	
	
	GPIOG -> MODER &= ~GPIO_MODER_MODER1;
	GPIOG -> PUPDR &= ~GPIO_PUPDR_PUPD1;
	GPIOG -> OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR1;
	GPIOG -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1_1;
	
}

void AutoMode(void){
	
	ZeroDMABufRX();
	
	if(SleepFlag){
		GPIOG -> ODR &= ~GPIO_ODR_OD0;
		LedOnOff(red, LedOn);
		delay_ms(300);
	}
	
	while(SleepFlag){
		if((GPIOG -> IDR & GPIO_IDR_ID1) != RESET){
			GPIOG -> ODR |= GPIO_ODR_OD0;
			LedOnOff(blue, LedOn);
			delay_ms(500);
			MatchFingerprint();
			LedOnOff(blue, LedOff);
			delay_ms(300);
			GPIOG -> ODR &= ~GPIO_ODR_OD0;
			delay_ms(300);
			if(DMABufRX[0]==7){
				SleepFlag=false;
				GPIOG -> ODR |= GPIO_ODR_OD0;
				LedOnOff(red, LedOff);
				delay_ms(300);
			}
		}
	}
}

void PCCommandAnalysis(void){
	switch(DMABufRX[0]){
		case 0:
			break;
		case 1:
			UserCount = GetUserCount();
		break;
		case 2:
			UserCount = GetUserCount();
			AddUserID(UserCount+1);
		break;
		case 4:
			DeleteAllUsers();
		break;
		case 5:
			delay_ms(200);
			uint16_t ID = DMABufRX[0];
			DeleteUser(ID);
		break;
		case 6:
			MatchFingerprint();
		break;
		case 7:
			SleepFlag = true;
			AutoMode();
		break;
		default:
			USART3SendString("No such command \r\n");
		break;
	}

}



