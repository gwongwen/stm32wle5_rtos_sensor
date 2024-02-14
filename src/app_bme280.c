/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_bme280.h"

int8_t app_bme280_init(const struct device *dev)
{
    dev = DEVICE_DT_GET_ANY(bosch_bme280);
    if (dev == NULL) {
        printk("error: no bme280 device found\n");
		return 0;
	}

    if (!device_is_ready(dev)) {
		printk("error: bme280 is not ready\n");
		return 0;
	} else {
        printk("- found device \"%s\", getting bme280 data\n", dev->name);
    }
    return 0;
}

uint16_t app_bme280_get_temp(const struct device *dev)
{
    struct sensor_value temp_int32;
    uint16_t temp_uint16;
    int8_t ret = 0;

    dev = DEVICE_DT_GET_ANY(bosch_bme280);

	ret = sensor_sample_fetch(dev);
    if (ret < 0 && ret != -EBADMSG) { 
	    printk("bme280 device sample is not up to date. error: %d\n", ret);
	    return 0;
    }

	ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp_int32);
    if (ret < 0) {
        printk("can't read sensor channels. error: %d\n", ret);
	    return 0;
    }

	temp_uint16 = (uint16_t)(temp_int32.val1*100 + temp_int32.val2 / 10000);
    printk("bme280 temp: %d\n", temp_uint16);
    return temp_uint16;
}

uint16_t app_bme280_get_press(const struct device *dev)
{
    struct sensor_value press_int32;
    uint16_t press_uint16;
    int8_t ret = 0;

    dev = DEVICE_DT_GET_ANY(bosch_bme280);

	ret = sensor_sample_fetch(dev);
    if (ret < 0 && ret != -EBADMSG) { 
	    printk("bme280 device sample is not up to date. error: %d\n", ret);
	    return 0;
    }

	ret = sensor_channel_get(dev, SENSOR_CHAN_PRESS, &press_int32);
    if (ret < 0) {
        printk("can't read sensor channels. error: %d\n", ret);
	    return 0;
    }

	press_uint16 = ((uint16_t)(press_int32.val1*1000 + press_int32.val2 / 10000)) - 5000;
    printk("bme280 pressure: %d\n", press_uint16);
    return press_uint16;
}

uint16_t app_bme280_get_hum(const struct device *dev)
{
    struct sensor_value hum_int32;
    uint16_t hum_uint16;
    int8_t ret = 0;

    dev = DEVICE_DT_GET_ANY(bosch_bme280);

	ret = sensor_sample_fetch(dev);
    if (ret < 0 && ret != -EBADMSG) { 
	    printk("bme280 device sample is not up to date. error: %d\n", ret);
	    return 0;
    }

	ret = sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum_int32);
    if (ret < 0) {
        printk("can't read sensor channels. error: %d\n", ret);
	    return 0;
    }

	hum_uint16 = (uint16_t)(hum_int32.val1*100 + hum_int32.val2 / 10000);
    printk("bme280 humidity: %d\n", hum_uint16);
    return hum_uint16;
}