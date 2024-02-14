/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_vbat.h"

int8_t app_stm32_vbat_init(const struct device *dev)
{
    dev = DEVICE_DT_GET_ONE(st_stm32_vbat);

    if (dev == NULL) {
        printk("error: no stm32 vbat device found\n");
		return 0;
	}

    if (!device_is_ready(dev)) {
		printk("error: stm32 vbat is not ready\n");
		return 0;
	} else {
        printk("- found device \"%s\", getting vbat data\n", dev->name);
    }
    return 0;
}

uint16_t app_stm32_get_vbat(const struct device *dev)
{
    struct sensor_value bat_int32;
    uint16_t bat_uint16;
    int8_t ret = 0;

    dev = DEVICE_DT_GET_ONE(st_stm32_vbat);

    ret = sensor_sample_fetch(dev);
    if (ret < 0 && ret != -EBADMSG) {        
	    printk("error: stm32 vbat sample is not up to date\n");
	    return 0;
    }

	ret = sensor_channel_get(dev, SENSOR_CHAN_VOLTAGE, &bat_int32);
    if (ret < 0) {
        printk("error: can't read sensor channels\n");
	    return 0;
    }

    bat_uint16 = (uint16_t)(bat_int32.val1*100 + bat_int32.val2 / 10000);
    printk("stm32 vbat: %d\n", bat_uint16);
    return bat_uint16;
}
