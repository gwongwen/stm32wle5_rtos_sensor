/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_bme280.h"
#include "app_nvs.h"

int8_t tph_ind;

int8_t app_bme280_init(const struct device *dev)
{
    static struct nvs_fs fs;

    dev = DEVICE_DT_GET_ONE(st_stm32_vbat);
    if (dev == NULL) {
        printk("error: no stm32 vbat device found\n");
		return 0;
	}

    if (!device_is_ready(dev)) {
		printk("error: stm32 vbat is not ready\n");
		return 0;
	} else {
        printk("- found device \"%s\", getting bme280 data\n", dev->name);
    }
    tph_ind = 0;
    (void)nvs_delete(&fs, NVS_BME280_ID);
    return 0;
}

int8_t app_bme280_handler(const struct device *dev)
{
    static struct nvs_fs fs;
    struct sensor_value temp;
    struct sensor_value hum;
    int8_t ret = 0;
    uint16_t val_mil[TPH_BUFFER_SIZE];

    dev = DEVICE_DT_GET_ANY(bosch_bme280);

	ret = sensor_sample_fetch(dev);
    if (ret < 0 && ret != -EBADMSG) { 
	    printk("bme280 device sample is not up to date. error: %d\n", ret);
	    return 0;
    }

	ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
    if (ret < 0) {
        printk("can't read sensor channels. error: %d\n", ret);
	    return 0;
    }

	ret = sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum);
    if (ret < 0) {
        printk("can't read sensor channels. error: %d\n");
	    return 0;
    }
    printk("temp: %d.%03d; press: %d.%03d\n",temp.val1, temp.val2, hum.val1, hum.val2);

    if (tph_ind < TPH_BUFFER_SIZE) {
        val_mil[tph_ind] = (sensor_value_to_milli(&temp)/10);
        val_mil[tph_ind + 1] = (sensor_value_to_milli(&hum)/10);
        tph_ind++;
    } else {
        (void)nvs_write(&fs, NVS_BME280_ID, &val_mil, sizeof(val_mil));
        tph_ind = 0;
    }
    return 0;
}