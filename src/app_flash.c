/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_vbat.h"
#include "app_bme280.h"
#include "app_flash.h"

//  ======== globals ============================================
struct flash_data {
	uint16_t vbat;
	uint16_t temp;
	uint16_t press;
	uint16_t hum;
};	

int8_t isr_ind;		// declaration of isr index

//  ======== app_flash_init =====================================
int8_t app_flash_init(const struct device *dev)
{
	int8_t ret;

	// getting storage partition in flash memory
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

	// erasing 2 pages at @0003 F000 (4kbytes at the end of flash memory)
	ret  = flash_erase(dev, FLASH_PARTITION_OFFSET, 2*FLASH_PAGE_SIZE);
	if (ret) {
		printk("error erasing flash. error: %d\n", ret);
	} else {
		printk("erased all pages\n");
	}
	isr_ind = 0;	// initialisation of isr index
	return 0;
}

//  ======== app_flash_write ===================================
int8_t app_flash_write(const struct device *dev, uint16_t data)
{
	int8_t ret;
	dev = FLASH_PARTITION_DEVICE;

	// writing data in the first page of 2kbytes
	ret = flash_write(dev, FLASH_PARTITION_OFFSET, data, sizeof(data));
	if (ret) {
		printk("error writing data. error: %d\n", ret);
	} else {
		printk("wrote %zu bytes to address 0x0003f000\n", sizeof(data));
	}
	return 0;
}

//  ======== app_flash_read ===================================
int8_t app_flash_read(const struct device *dev)
{
	int8_t ret;
	struct flash_data data[FLASH_BUFFER_SIZE];
	dev = FLASH_PARTITION_DEVICE;

	// reading the first page
	ret = flash_read(dev, FLASH_PARTITION_OFFSET, data, sizeof(data));
	if (ret) {
		printk("error reading data.. error: %d\n", ret);
	} else {
		printk("read %zu bytes from address 0x0003f000\n", sizeof(data));
	}
	// printing data
	for (int8_t i = 0; i < FLASH_BUFFER_SIZE; i++) {
		printk("vbat: %d, temp: %d, press: %d, hum: %d\n", data[i].vbat, data[i].temp, data[i].press, data[i].hum);
	}
	return 0;		
}

//  ======== app_flash_handler ================================
int8_t app_flash_handler(const struct device *dev)
{
	int8_t ret;
	struct flash_data data[FLASH_BUFFER_SIZE];
	const struct device *bme_dev;
	const struct device *bat_dev;

	// getting all devices
	dev = FLASH_PARTITION_DEVICE;
	bat_dev = DEVICE_DT_GET_ONE(st_stm32_vbat);
	bme_dev = dev = DEVICE_DT_GET_ANY(bosch_bme280);

	uint16_t vbat, temp, press, hum;

	if (isr_ind < FLASH_BUFFER_SIZE) {
		data->vbat = app_stm32_get_vbat(bat_dev);
		data->temp = app_bme280_get_temp(bme_dev);
		data->press = app_bme280_get_press(bme_dev);
		data->hum = app_bme280_get_hum(bme_dev);
		isr_ind++;
	} else {
		app_flash_write(dev, data);
		k_sleep(K_MSEC(2000));
		app_flash_read(dev);
		isr_ind = 0;
	}
	return 0;
}

