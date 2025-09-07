//
// Created by dextercai on 25-6-29.
//

#ifndef AS5048A_H
#define AS5048A_H

#define AS5048_CMD_ANGLE          0x3FFF
#define AS5048_CMD_READ_MAG       0x3FFE
#define AS5048_CMD_READ_DIAG      0x3FFD
#define AS5048_CMD_NOP            0x0000
#define AS5048_CMD_CLEAR_ERROR    0x0001
#define AS5048_CMD_ProgramControl 0x0003
#define AS5048_CMD_OTPHigh        0x0016
#define AS5048_CMD_OTPLow         0x0017

#include "stm32f0xx_hal.h"

typedef struct {
    SPI_HandleTypeDef* hspi;
    GPIO_TypeDef* csPort;
    uint16_t csPin;
    uint8_t error_flag;
    //uint16_t zero_pos;
} AS5048A;


void AS5048A_Init(AS5048A* dev, SPI_HandleTypeDef* hspi, GPIO_TypeDef* csPort, uint16_t csPin);

HAL_StatusTypeDef WriteAs5048aReg(AS5048A* dev, uint16_t cmd, uint16_t value, uint16_t *res);

uint8_t ReadAs5048aReg(AS5048A* dev, uint16_t cmd, uint16_t *res);

#endif //AS5048A_H
