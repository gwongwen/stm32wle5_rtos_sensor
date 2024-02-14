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

int8_t app_flash_init(const struct device *dev);

#endif /* APP_FLASH_H */