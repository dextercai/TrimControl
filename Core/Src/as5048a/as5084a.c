//
// Created by dextercai on 25-6-29.
//

#include "as5048a.h"


uint8_t parity_even(uint16_t v)
{
    if(v == 0) return 0;
    v ^= v >> 8;
    v ^= v >> 4;
    v ^= v >> 2;
    v ^= v >> 1;
    return v & 1;
}

typedef union {
    uint16_t value;
    uint8_t  bytes[2];
} Converter;

void AS5048A_Init(AS5048A* dev, SPI_HandleTypeDef* hspi, GPIO_TypeDef* csPort, uint16_t csPin) {
    dev->csPin = csPin;
    dev->hspi = hspi;
    dev->csPort = csPort;
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET);
}

uint8_t ReadAs5048aReg(AS5048A* dev, uint16_t cmd, uint16_t *res)
{
    uint16_t data = 0;
    uint16_t command = 0x4000;// PAR=0 R/W=R
    command = command | cmd;
    command |= ((uint16_t)parity_even(command)<<15);

    uint8_t left_byte = command & 0xFF;
    uint8_t right_byte = ( command >> 8 ) & 0xFF;
    uint8_t command_bytes[2] = { // MSB发送uint16
        right_byte, left_byte
    };
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(dev->hspi, command_bytes, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET);
    if (status != HAL_OK) {
        return -1;
    }
    command = 0x4000;// PAR=0 R/W=R
    command = command | AS5048_CMD_NOP;
    command |= ((uint16_t)parity_even(command)<<15);
    left_byte = command & 0xFF;
    right_byte = ( command >> 8 ) & 0xFF;
    command_bytes[1] = left_byte;
    command_bytes[0] = right_byte;
    uint8_t rx[2] = {0};
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_RESET);
    status = HAL_SPI_TransmitReceive(dev->hspi, command_bytes, rx, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET);
    uint16_t rxData = 0;
    rxData = (( ( rx[0] & 0xFF ) << 8 ) | ( rx[1] & 0xFF )) & ~0xC000;
    if (status != HAL_OK) {
        return -1;
    }
    dev->error_flag = 1;
    if ((rxData & (1 << 14)) == 0){
        data = (rxData & 0x3FFF);
        dev->error_flag = (parity_even(data) ^ (rxData >> 15));
        *res = data;
    }else
    {
        command = 0x4000;
        command = command | AS5048_CMD_CLEAR_ERROR;
        command |= ((uint16_t)parity_even(command)<<15);
        left_byte = command & 0xFF;
        right_byte = ( command >> 8 ) & 0xFF;
        command_bytes[1] = left_byte;
        command_bytes[0] = right_byte;
        HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_RESET);
        status = HAL_SPI_Transmit(dev->hspi, command_bytes, 2, HAL_MAX_DELAY);
        HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET);
        if (status != HAL_OK) {
            return -1;
        }
    }
    return 0;
}


HAL_StatusTypeDef WriteAs5048aReg(AS5048A* dev, uint16_t cmd, uint16_t value, uint16_t *res)
{
    uint16_t data = 0;
    uint16_t command = 0x0000; // PAR=0 R/W=W

    command = command | cmd;
    command |= ((uint16_t)parity_even(command)<<15);//Add a parity bit on the the MSB

    Converter conv;
    conv.value = command;

    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(dev->hspi, (uint8_t*)conv.bytes, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET);

    if (status != HAL_OK) {
        return status;
    }

    command = 0x0000; // PAR=0 R/W=W
    command = command | value;
    command |= ((uint16_t)parity_even(command)<<15);//Add a parity bit on the the MSB
    conv.value = command;

    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_RESET);
    status = HAL_SPI_Transmit(dev->hspi, (uint8_t*)conv.bytes, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET);

    if (status != HAL_OK) {
        return status;
    }

    command = 0x4000;// PAR=0 R/W=R
    command = command | AS5048_CMD_NOP;
    command |= ((uint16_t)parity_even(command)<<15);//Add a parity bit on the the MSB
    conv.value = command;


    uint8_t rx[2] = {0};
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_RESET);
    status = HAL_SPI_TransmitReceive(dev->hspi, (uint8_t*)conv.bytes, rx, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET);

    if (status != HAL_OK) {
        return status;
    }
    dev->error_flag = 1;

    conv.bytes[0] = rx[0];
    conv.bytes[1] = rx[1];

    if ((conv.value & (1 << 14)) == 0){
        data = (conv.value & 0x3FFF);
        dev->error_flag = (parity_even(data) ^ (conv.value >> 15));
        *res = data;
    }else
    {
        command = 0x4000;
        command = command | AS5048_CMD_CLEAR_ERROR;
        command |= ((uint16_t)parity_even(command)<<15);

        conv.value = command;
        HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_RESET);
        status = HAL_SPI_Transmit(dev->hspi, (uint8_t*)conv.bytes, 2, HAL_MAX_DELAY);
        HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET);

        if (status != HAL_OK) {
            return status;
        }
    }
    return HAL_OK;
}


// uint16_t ReadAs5048aQhValue(AS5048A* dev, uint16_t cmd)
// {
//     uint16_t val;
//     ReadAs5048aReg(dev, cmd, &val);
//     if(dev->error_flag)
//     {
//         val = 0;
//     }
//     return val;
// }

