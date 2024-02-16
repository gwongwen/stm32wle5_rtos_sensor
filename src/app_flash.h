/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_FLASH_H
#define APP_FLASH_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>

#define FLASH_PARTITION			storage_partition
#define FLASH_PARTITION_DEVICE	FIXED_PARTITION_DEVICE(FLASH_PARTITION)
#define FLASH_PARTITION_OFFSET	FIXED_PARTITION_OFFSET(FLASH_PARTITION)
#define FLASH_PAGE_SIZE         2048
#define FLASH_BUFFER_SIZE       4*4 // just for test: 4 samples of vbat and tph

int8_t app_flash_init(const struct device *dev);
int8_t app_flash_write(const struct device *dev, uint16_t data_wrt);
int8_t app_flash_read(const struct device *dev);
int8_t app_flash_handler(const struct device *dev);

#endif /* APP_FLASH_H */