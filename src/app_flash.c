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
int8_t ind;		// declaration of isr index

struct nvs_data {
	uint16_t vbat;
	uint16_t temp;
	uint16_t press;
	uint16_t hum;
};

//  ======== app_flash_init =====================================
int8_t app_flash_init(struct nvs_fs *fs)
{
	struct flash_pages_info info;
	int8_t ret;

	// configuration of storage partition in flash memory
	fs->flash_device = NVS_PARTITION_DEVICE;
	if (!device_is_ready(fs->flash_device)) {
		printk("flash device %s is not ready\n", fs->flash_device->name);
		return 0;
	} else {
		printk("- found device: \"%s\", getting nvs memory\n", fs->flash_device->name);
	}

	fs->offset = NVS_PARTITION_OFFSET;
	ret = flash_get_page_info_by_offs(fs->flash_device, fs->offset, &info);
	if (ret) {
		printk("unable to get page info. error: %d\n", ret);
		return 0;
	}

	fs->sector_size = info.size;
	if (!fs->sector_size || fs->sector_size % info.size) {
		printk("invalid sector size\n");
		return 0;
	}

	fs->sector_count = 2U;
	ret = nvs_mount(fs);
	if (ret) {
		printk("flash init failed. error: %d\n", ret);
		return 0;
	}

//	(void)nvs_delete(&fs, SENSOR_ID);
	ind = 0;	// initialisation of isr index
	return 0;
}

//  ======== app_flash_write ====================================
int8_t app_flash_write(struct nvs_fs *fs, void *data)
{
	int8_t ret;
	
	// writing data in the first page of 2kbytes
	(void) nvs_write(fs, SENSOR_ID, data, sizeof(data));
	printk("wrote %zu bytes to address 0x0003f000\n", sizeof(data));
	return 0;
}

//  ======== app_flash_read =====================================
int8_t app_flash_read(struct nvs_fs *fs)
{
	int8_t ret;
	struct nvs_data data_rd[NVS_BUFFER_SIZE];

	// reading the first page
	ret = nvs_read(fs, SENSOR_ID, &data_rd, sizeof(data_rd));
	if (ret) {
		printk("error reading data. error: %d\n", ret);
	} else {
		printk("read %zu bytes from address 0x0003f000\n", sizeof(data_rd));
	}
	// printing data
	for (int8_t i = 0; i < NVS_BUFFER_SIZE; i++) {
		printk("vbat: %d, temp: %d, press: %d, hum: %d\n", data_rd[i].vbat, data_rd[i].temp, data_rd[i].press, data_rd[i].hum);
	}
	return 0;		
}

//  ======== app_flash_handler ==================================
int8_t app_flash_handler(struct nvs_fs *fs)
{
	int8_t ret;
	const struct device *bme_dev;
	const struct device *bat_dev;
	struct nvs_data data_wrt[NVS_BUFFER_SIZE];

	// getting all sensor devices
	bat_dev = DEVICE_DT_GET_ONE(st_stm32_vbat);
	bme_dev = DEVICE_DT_GET_ANY(bosch_bme280);

	uint16_t vbat, temp, press, hum;

	// putting 4 structures in fisrt page for this test
	if (ind < NVS_BUFFER_SIZE) {
		data_wrt[ind].vbat = app_stm32_get_vbat(bat_dev);
		data_wrt[ind].temp = app_bme280_get_temp(bme_dev);
		data_wrt[ind].press = app_bme280_get_press(bme_dev);
		data_wrt[ind].hum = app_bme280_get_hum(bme_dev);
		ind++;
	} else {
		// writing data in the first page of 2kbytes
		app_flash_write(fs, data_wrt);
		k_sleep(K_MSEC(2000));
		// reading data in the first page of 2kbytes
		app_flash_read(fs);
		ind = 0;
	}
	return 0;
}

