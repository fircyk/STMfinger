#ifndef _SENSOR
#define _SENSOR

#include "functions.h"
#include <stm32f446xx.h>
#include <system_stm32f4xx.h>
#include <stdbool.h>
#include <stm32f4xx.h>

#define TRUE  1
#define FALSE 0

// Basic response message definition
#define ACK_SUCCESS       	0x00
#define ACK_FAIL         		0x01
#define ACK_FULL	  				0x04
#define ACK_NO_USER	  			0x05
#define ACK_USER_OCCUPIED   0x06
#define ACK_FINGER_OCCUPIED 0x07
#define ACK_TIMEOUT       	0x08
#define ACK_GO_OUT	  			0x0F		// The center of the fingerprint is out of alignment with sensor

//User information definition
#define ACK_ALL_USER       	0x00
#define ACK_GUEST_USER 	  	0x01
#define ACK_NORMAL_USER 	  0x02
#define ACK_MASTER_USER    	0x03

#define USER_MAX_CNT	   	  2000	// Maximum fingerprint number


// Command definition
#define CMD_HEAD	   				0xF5
#define CMD_TAIL	 	 				0xF5
#define CMD_ADD_1    				0x01
#define CMD_ADD_2    				0x02
#define CMD_ADD_3	   				0x03
#define CMD_MATCH_1ON1			0x0B
#define CMD_MATCH	   				0x0C
#define CMD_DEL		   				0x04
#define CMD_DEL_ALL	 				0x05
#define CMD_MODIFY_SN				0x08
#define CMD_USER_CNT 				0x09
#define CMD_COM_LEV	 				0x28
#define CMD_GET_SN					0x2A
#define CMD_LP_MODE  				0x2C
#define CMD_TIMEOUT  				0x2E

#define CMD_FINGER_DETECTED 0x14

uint8_t CountRXCheckSum(void);
uint8_t AddUser(void);
uint8_t AddUser1(void);
uint8_t AddUser1ID(uint8_t ID);
uint8_t AddUser2ID(uint8_t ID);
uint8_t AddUser3ID(uint8_t ID);
uint16_t GetUserCount(void);
uint8_t SendReceiveCMD(uint8_t TXByteCount, uint8_t RXByteCount, uint16_t delay); // byc moze osobno send i receive
uint8_t DeleteAllUsers(void);
uint8_t DeleteUser(uint16_t ID);
uint16_t MatchFingerprint(void);


#endif

