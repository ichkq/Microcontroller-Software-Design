/**
 * @file spi_bmp388.h
 * @brief Header file for defining the SPI read and write functions for the sensor.
 *
 * This header declares the 8-bit and 16-bit read functions and the 8-bit write function
 * necessary for accessing the sensor's registers over the SPI interface. These functions
 * facilitate communication with the sensor to retrieve or modify register values.
 * It also defines the calibration data structure and the pressure compensation function.
 * (c) LAZARYEV ALEKSANDR 12.10.2024
 */

#include "main.h"

#ifndef __SPI_BMP388_H_
#define __SPI_BMP388_H_
//****************************************
//FUNTION DEFINITIONS
//***************************************
//READ 8 BIT REGISTER FUNCTION
HAL_StatusTypeDef read8SPI(SPI_HandleTypeDef hspi1, uint8_t adr, uint16_t *result);
//READ 16 BIT REGISTER FUNCTION
HAL_StatusTypeDef read16SPI(SPI_HandleTypeDef hspi1, uint8_t adr, uint16_t *result);
//WRITE 8 BIT REGISTER FUNCTION
HAL_StatusTypeDef write8SPI(SPI_HandleTypeDef hspi1, uint8_t adr, uint8_t *data);

#endif
