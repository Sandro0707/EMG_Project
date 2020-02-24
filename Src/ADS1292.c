/*------------------------------- DEFINES-----------------------------*/

#include "ADS1292.h"
#include "main.h"


/*------------------------------- VARIABLES -----------------------------*/
    bool intDRDY = false; // Flag to ready data from ADS1292

    int stat_1, stat_2;    	// used to hold the status register for boards 1 and 2

    uint8_t regData[24];		// array with data from all registers
    int32_t channelData [16];	// array used when reading channel data board 1+2
/*------------------------------- FUNCTIIONS -----------------------------*/

void ADS_Init() {
    ADS_RESET();
    HAL_Delay(500);

    ADS_SDATAC();
    HAL_Delay(500);

    ADS_getDeviceID();
    HAL_Delay(1000);

    //Work settings
    ADS_WREG(CONFIG1, 0x06);
    HAL_Delay(100);
    ADS_WREG(CONFIG2, 0x10);
    HAL_Delay(100);

    ADS_WREG(LOFF, 0x03);
    HAL_Delay(10);

    ADS_WREG(CH1SET, 0x50);
    HAL_Delay(10);
    ADS_WREG(CH2SET, 0x50);
    HAL_Delay(10);

    ADS_WREG(LOFF_SENS, 0xFF);
    HAL_Delay(10);


    ADS_WREG(LOFF_STAT, 0xF1);
    HAL_Delay(10);


    ADS_WREG(GPIO, 0x00);
    HAL_Delay(10);


    ADS_WREG(0x18, 0x0A);
    HAL_Delay(10);

    ADS_WREG(0x19, 0xE3);
    HAL_Delay(10);


    ADS_START();
    HAL_Delay(100);
}

void ADS_SDATAC(){
    HAL_GPIO_WritePin( ADS_CS_GPIO_Port , ADS_CS_Pin, GPIO_PIN_RESET);
    transferSPI(SDATAC);
    HAL_GPIO_WritePin(ADS_CS_GPIO_Port, ADS_CS_Pin, GPIO_PIN_SET);
}
void ADS_START() {
    HAL_GPIO_WritePin(ADS_CS_GPIO_Port, ADS_CS_Pin, GPIO_PIN_RESET);
    transferSPI(START);
    HAL_GPIO_WritePin(ADS_CS_GPIO_Port, ADS_CS_Pin, GPIO_PIN_SET);
}

//stop data conversion
void ADS_STOP() {
    HAL_GPIO_WritePin(ADS_CS_GPIO_Port,ADS_CS_Pin, GPIO_PIN_RESET);
    transferSPI(STOP);
    HAL_GPIO_WritePin(ADS_CS_GPIO_Port,ADS_CS_Pin, GPIO_PIN_SET);
}
void ADS_RESET(){
    HAL_GPIO_WritePin(ADS_CS_GPIO_Port, ADS_CS_Pin, GPIO_PIN_RESET);
    transferSPI(RESET);
    HAL_GPIO_WritePin(ADS_CS_GPIO_Port, ADS_CS_Pin, GPIO_PIN_SET);
}

void ADS_RDATAC() {
    HAL_GPIO_WritePin(ADS_CS_GPIO_Port,ADS_CS_Pin, GPIO_PIN_RESET);
    transferSPI(RDATAC);
    HAL_GPIO_WritePin(ADS_CS_GPIO_Port,ADS_CS_Pin, GPIO_PIN_SET);
}

void ADS_WAKEUP() {
    HAL_GPIO_WritePin(ADS_CS_GPIO_Port,ADS_CS_Pin, GPIO_PIN_RESET);
    transferSPI(WAKEUP);
    HAL_GPIO_WritePin(ADS_CS_GPIO_Port,ADS_CS_Pin, GPIO_PIN_SET);
}
uint8_t ADS_RREG(uint8_t _address){		//  reads ONE register at _address
    uint8_t opcode1 = _address + 0x20; 		//  RREG expects 001rrrrr where rrrrr = _address

    HAL_GPIO_WritePin(ADS_CS_GPIO_Port, ADS_CS_Pin, GPIO_PIN_RESET);			//  open SPI

    transferSPI(opcode1); 								//  opcode1
    transferSPI( 0); 											//  opcode2

    regData[_address] = transferSPI( 0);		//  update mirror location with returned byte

    HAL_GPIO_WritePin(ADS_CS_GPIO_Port, ADS_CS_Pin, GPIO_PIN_SET);				//  close SPI

    //HAL_UART_Transmit(&huart1, &regData[0], sizeof(uint8_t),0x1000);

    return regData[_address];			// return requested register value
}
void ADS_WREG(uint8_t _address, uint8_t _value) {	//  Write ONE register at _address
    uint8_t opcode1 = _address + 0x40; 				//  WREG expects 010rrrrr where rrrrr = _address

    HAL_GPIO_WritePin(ADS_CS_GPIO_Port,ADS_CS_Pin, GPIO_PIN_RESET); 					//  open SPI

    transferSPI(opcode1);											//  Send WREG command & address
    transferSPI(0x00);												//	Send number of registers to read -1
    transferSPI(_value);											//  Write the value to the register

    HAL_GPIO_WritePin(ADS_CS_GPIO_Port,ADS_CS_Pin, GPIO_PIN_SET); 						//  close SPI

    regData[_address] = _value;			//  update the mirror array

}

void ADS_RDATA() {				//  use in Stop Read Continuous mode when DRDY goes low
    uint8_t inByte,inByte1,inByte2,inByte3;
    int i;
    int nchan = 2;	//assume 2 channel.

    stat_1 = 0;							//  clear the status registers
    stat_2 = 0;

    for(i = 0; i < nchan; i++){
        channelData[i] = 0;
    }

    HAL_GPIO_WritePin(ADS_CS_GPIO_Port,ADS_CS_Pin,GPIO_PIN_RESET); 					//  open SPI
    transferSPI(RDATA);
}
void ADS_SendUARTData(void){
    UART_Send("DT0");
    UART_Send4Byte((uint8_t*)&channelData[0]);
    UART_Send("DT1");
    UART_Send4Byte((uint8_t*)&channelData[1]);
    UART_Send("DT2");
    UART_Send4Byte((uint8_t*)&channelData[2]);
    UART_Send("DT3");
    UART_Send4Byte((uint8_t*)&channelData[3]);
    UART_Send("DT4");
    UART_Send4Byte((uint8_t*)&channelData[4]);
    UART_Send("DT5");
    UART_Send4Byte((uint8_t*)&channelData[5]);
    UART_Send("DT6");
    UART_Send4Byte((uint8_t*)&channelData[6]);
    UART_Send("DT7");
    UART_Send4Byte((uint8_t*)&channelData[7]);
    }

    void ADS_SendData(){
            ADS_SendUARTData();
    }

    int32_t* getChannelData(){
        return channelData;
    }
