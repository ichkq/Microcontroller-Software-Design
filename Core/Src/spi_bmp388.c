/**
 * @file spi_bmp388.c
 * @brief This file contains the 8-bit and 16-bit read and 8-bit write functions
 *        necessary for accessing the registers of the SPI sensor.
 *
 * These functions are used to read data from and write data to the sensor's
 * registers over the SPI interface. The 8-bit read function retrieves a single byte
 * from the specified register, while the 16-bit read function retrieves two bytes.
 * The 8-bit write function allows writing a byte of data to a specific register.
 * (c) LAZARYEV ALEKSANDR 12.10.2024
 */


#include "spi_bmp388.h" //INCLUDE THE PRIVAATE HEADER FILE
#include "main.h"

HAL_StatusTypeDef read8SPI(SPI_HandleTypeDef hspi1, uint8_t adr, uint16_t *result)
{
    HAL_StatusTypeDef ret; //RETURNS HAL_OK IF OK.

    uint8_t spiTx1[1] = {0}; //TRANSMIT ADRESS TO SPI SENSOR BUFFER
    uint8_t spiRx1[2] = {0}; //RECIEVE VALUE FROM SPI, 2 BITS BECAUSE OF DUMMY BITE

    spiTx1[0] = adr | (1 << 7); // SETTING THE 7TH BIT TO 1 FOR "READ MODE"


    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0); //ENABLE CHIP SELECT CS


    HAL_SPI_Transmit(&hspi1, spiTx1, 1, HAL_MAX_DELAY); //TRANSMIT THE BITSHIFTED READ ADRESS TO SPI SENSOR
    ret = HAL_SPI_Receive(&hspi1, spiRx1, 2, HAL_MAX_DELAY);// RECIEVE DUMMY BITE AND 8 BIT VALUE


    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1); //DISABLE CHIP SELECT CS

    *result = (spiRx1[0] << 8) | spiRx1[1]; //CLEAR THE DUMMY BITE AND BITSHIFT THE VALUE TO THE LEFT

    return ret;
}


HAL_StatusTypeDef read16SPI(SPI_HandleTypeDef hspi1, uint8_t adr, uint16_t *result)
{
    HAL_StatusTypeDef ret; //SET RETURN

    uint8_t spiTx2[1] = {0}; //TRANSMIT ADRESS TO SPI SENSOR BUFFER
    uint8_t spiRx2[3] = {0}; //RECIEVE VALUE FROM SPI, 3 BITS BECAUSE OF DUMMY BITE AND 16 BIT VALUE

    spiTx2[0] = adr | (1 << 7); // SETTING THE 7TH BIT TO 1 FOR "READ MODE"

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0); //ENABLE CHIP SELECT CS

    HAL_SPI_Transmit(&hspi1, spiTx2, 1, HAL_MAX_DELAY); //TRANSMIT THE BITSHIFTED READ ADRESS TO SPI SENSOR
    ret = HAL_SPI_Receive(&hspi1, spiRx2, 3, HAL_MAX_DELAY); // RECIEVE DUMMY BITE AND 16 BIT VALUE

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1); //DISABLE CHIP SELECT CS

    *result = ((spiRx2[1] << 8) | spiRx2[2]); //CLEAR THE DUMMY BITE AND BITSHIFT THE 16 BIT VALUE TO THE LEFT

    return ret;
}

HAL_StatusTypeDef write8SPI(SPI_HandleTypeDef hspi1, uint8_t adr, uint8_t *data)
{
    HAL_StatusTypeDef ret; //SET RETURN

    uint8_t spiTx[2]; //TRANSMIT WRTIE BUFFER

    spiTx[0] = adr & 0x7F; //SET 7TH BIT TO 0 FOR WRITE MODE
    spiTx[1] = *data; //TRANSMIT THE VALUE

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0); //ENABLE CHIP SELECT CS

    ret = HAL_SPI_Transmit(&hspi1, spiTx, 2, HAL_MAX_DELAY); //TRANSMIT THE BITSHIFTED WRITE ADRESS AND VALUE TO SPI SENSOR

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1); //DISABLE CHIP SELECT CS

    return ret;
}
// (c) LAZARYEV ALEKSANDR 12.10.2024
