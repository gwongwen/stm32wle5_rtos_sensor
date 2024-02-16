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
} data[FLASH_BUFFER_SIZE];		

int8_t isr_ind;

int8_t app_flash_init(const struct device *dev)
{
	int8_t ret;
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

	ret  = flash_erase(dev, FLASH_PARTITION_OFFSET, FLASH_PAGE_SIZE);
	if (ret) {
		printk("error erasing flash. error: %d\n", ret);
	} else {
		printk("erased all pages\n");
	}
	isr_ind = 0;
	return 0;
}

int8_t app_flash_write(const struct device *dev, uint16_t data_wrt)
{
	int8_t ret;
	dev = FLASH_PARTITION_DEVICE;

	ret = flash_write(dev, FLASH_PARTITION_OFFSET, data, sizeof(data_wrt));
	if (ret) {
		printk("error writing data. error: %d\n", ret);
	} else {
		printk("wrote %zu bytes to address 0x0003f000\n", sizeof(data_wrt));
	}
	return 0;
}

int8_t app_flash_read(const struct device *dev)
{
	int8_t ret;
	dev = FLASH_PARTITION_DEVICE;

	ret = flash_read(dev, FLASH_PARTITION_OFFSET, data, sizeof(data));
	if (ret) {
		printk("error reading data.. error: %d\n", ret);
	} else {
		printk("read %zu bytes from address 0x0003f000\n", sizeof(data));
	}
	for (int8_t i = 0; i < FLASH_BUFFER_SIZE; i++) {
		printk("vbat: %d", data[i].vbat);
	}
	return 0;		
}

int8_t app_flash_handler(const struct device *dev)
{
	int8_t ret;
	const struct device *bme_dev;
	const struct device *bat_dev;

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

