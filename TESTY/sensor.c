#include "sensor.h"
#include "functions.h"
#include <stdio.h>

uint8_t bufferTX[9];
uint8_t bufferRX[9];

extern volatile uint32_t timeout;
extern volatile uint8_t DMABufRX[100];
extern volatile uint8_t BufRead[100];

//communication via USART 
uint8_t SendReceiveCMD(uint8_t TXByteCount, uint8_t RXByteCount, uint16_t delayMS){

//flooding the buffer with unused values
	for(int i=0; i<9; i++){
		bufferRX[i]=0xC1;
	}
	USART3SendString("tu jestem1");
	uint8_t CheckSum = 0;
	USART2SendChar(CMD_HEAD);
	
	//sending the buffer and calculating checksum
	for(int i = 0; i < TXByteCount; i++){
		USART2SendChar(bufferTX[i]);
		CheckSum ^= bufferTX[i];
		USART3SendString("tu jestem2");
	}

	USART2SendChar(CheckSum);
	USART2SendChar(CMD_TAIL);
	USART3SendString("tu jestem3");
	
	//delay_ms(delayMS);
	timeout = 0;
	while(timeout < delayMS){;}
	
	//reception of data below
	USART2GetBufferRX(RXByteCount, bufferRX);
	
	if(bufferRX[0] != CMD_HEAD) return ACK_FAIL;
	if(bufferTX[RXByteCount-1] != CMD_TAIL) return ACK_FAIL;
	if(bufferRX[1] != bufferTX[0]) return ACK_FAIL;

	for(int j = 1; j < RXByteCount-1; j++){
		CheckSum ^= bufferRX[j];
		USART3SendString("tu jestem 4");
	}
	
	if(CheckSum!=0){ 
		USART3SendString("tu jestem fail");
		return ACK_FAIL;
		
	}
	USART3SendString("tu jestem succ");
	return ACK_SUCCESS;
}

uint8_t CountRXCheckSum(void){
		char buf;
		uint8_t RXCheckSum = 0;
	
		for(int i = 1; i<7; i++){
			RXCheckSum ^= BufRead[i];
			USART3SendString("\r\n kolejna checksuma: ");
			sprintf(&buf, "%d", BufRead[i]);
			USART3SendChar(buf);
		}
	return RXCheckSum;
}	

uint16_t GetUserCount(){
	
		uint8_t temp;
		uint8_t CheckSum=0;
		char buf[2];
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
			
			sprintf(buf, "%d", UserCount);
			USART3SendString("\r\n SUCC ID: ");
			USART3SendString(buf);
			
			return UserCount;
			
		}else{
			USART3SendString("tu jestem");
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
		}
		
}

uint8_t DeleteAllUsers(void){
		
		uint8_t CheckSum=0;
	
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
		
		uint16_t match;
		char buf[10];
	
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
		}
		
		if(bufferRX[4] == ACK_NO_USER){
				USART3SendString("\r\n MATCH NO USER");
				return ACK_NO_USER;
		}else if(bufferRX[4] == ACK_TIMEOUT){
			USART3SendString("\r\n TIMEOUT");
			return ACK_TIMEOUT;
		}else if(bufferTX[1] == BufRead[1] && BufRead[4]<=3){
				match = (BufRead[2] << 8) + BufRead[3];
				sprintf(buf, "%d", match);
				USART3SendString("\r\n Matched ID: ");
				USART3SendString(buf);
				sprintf(buf, "%d", BufRead[4]);
				USART3SendString("\r\n Matched permissionn: ");
				USART3SendString(buf);
		}

}
	
	



