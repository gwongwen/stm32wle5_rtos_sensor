/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */
 
#ifndef APP_ROM_H_
#define APP_ROM_H_

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/eeprom.h>

#define EEPROM_SAMPLE_OFFSET        0

int8_t app_rom_init(const struct device *dev);
int8_t app_rom_write(const struct device *dev, uint16_t data);
uint16_t app_rom_read(const struct device *dev);

#endif /* APP_ROM_H_ */