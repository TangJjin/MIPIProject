#ifndef BSP_TOUCH_I2C_H
#define BSP_TOUCH_I2C_H

#include "bsp_system.h"

HAL_StatusTypeDef Touch_I2C_Read8(uint16_t device_address,
                                  uint8_t register_address,
                                  uint8_t *data,
                                  uint16_t length);

HAL_StatusTypeDef Touch_I2C_Write8(uint16_t device_address,
                                   uint8_t register_address,
                                   const uint8_t *data,
                                   uint16_t length);

HAL_StatusTypeDef Touch_I2C_Read16(uint16_t device_address,
                                   uint16_t register_address,
                                   uint8_t *data,
                                   uint16_t length);

HAL_StatusTypeDef Touch_I2C_Write16(uint16_t device_address,
                                    uint16_t register_address,
                                    const uint8_t *data,
                                    uint16_t length);

#endif
