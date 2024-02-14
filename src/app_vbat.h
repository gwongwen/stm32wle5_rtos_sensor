/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_STM32_VBAT_H
#define APP_STM32_VBAT_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define BAT_BUFFER_SIZE     4       // 1 sample per 30min
                                    // 48 samples in uint16 format 

int8_t app_stm32_vbat_init(const struct device *dev);
uint16_t app_stm32_get_vbat(const struct device *dev);

#endif /* APP_STM32_VBAT_H */