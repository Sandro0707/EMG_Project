//
// Created by ASUS on 18.02.2020.
//

#ifndef EMG_ADS1292_H
#define EMG_ADS1292_H

#include "stm32f4xx_hal.h"
#include "stdbool.h"

// System Commands
#define WAKEUP		0x02		//Wake-up from standby mode
#define STANDBY	    0x04		//Enter standby mode
#define RESET		0x06		//Reset the device
#define START		0x08		//Start/restart (synchronize) conversions
#define STOP		0x0A		//Stop conversion
#define OFFSETCAL   0x1A        //Channel offset calibration

// Data Read Commands
#define RDATAC		0x10		//Enable Read Data Continuous mode.
//This mode is the default mode at power-up.
#define SDATAC		0x11		//Stop Read Data Continuously mode
#define RDATA		0x12		//Read data by command; supports multiple read back.

// Register Read Commands
#define RREG		0x20		//Read n nnnn registers starting at address r rrrr
                                //first byte 001r rrrr (2xh)(2) - second byte 000n nnnn(2)
#define WREG		0x40		//Write n nnnn registers starting at address r rrrr
                                //first byte 010r rrrr (2xh)(2) - second byte 000n nnnn(2)

//Register Addresses
#define ID         0x00
#define CONFIG1    0x01
#define CONFIG2    0x02
#define LOFF       0x03
#define CH1SET     0x04
#define CH2SET     0x05
#define RLD_SENS   0x06
#define LOFF_SENS  0x07
#define LOFF_STAT  0x08
#define RESP1      0x09
#define RESP2      0x0A
#define GPIO       0x0B

extern bool intDRDY;


extern int32_t channelData [16];

void ADS_Init(void);
void ADS_RDATAC(void);
void ADS_SDATAC(void);
void ADS_RESET(void);
void ADS_START(void);
void ADS_STOP(void);
void ADS_WAKEUP(void);

uint8_t ADS_getDeviceID(void);
uint8_t ADS_RREG(uint8_t _address);


void ADS_WREG(uint8_t _address, uint8_t _value);
void ADS_updateChannelData(void);
void ADS_RDATA(void);


extern uint8_t transferSPI(uint8_t send);



#endif //EMG_ADS1292_H
