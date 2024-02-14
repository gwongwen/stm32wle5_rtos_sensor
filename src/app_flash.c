/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_vbat.h"
#include "app_bme280.h"
#include "app_flash.h"

struct flash_data {
	uint16_t vbat;
	uint16_t temp;
	uint16_t press;
	uint16_t hum;
} data[384];			// 4*48*2 = 384 smples of uint8 (bytes) in each page

int8_t app_flash_init(const struct device *dev)
{
	dev = FLASH_PARTITION_DEVICE;

	if (dev == NULL) {
        printk("error: no flash device found\n");
		return 0;
	}

    if (!device_is_ready(dev)) {
		printk("error: flash is not ready\n");
		return 0;
	} else {
        printk("- found device \"%s\", writing/reading data\n", dev->name);
    }
	return 0;
}
