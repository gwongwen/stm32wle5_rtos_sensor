/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_vbat.h"
#include "app_nvs.h"

int8_t bat_ind;

int8_t app_vbat_init(const struct device *dev)
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
        printk("- found device \"%s\", getting vbat data\n", dev->name);
    }
    bat_ind =0;
    (void)nvs_delete(&fs, NVS_STM32_VBAT_ID);
    return 0;
}

int8_t app_vbat_handler(const struct device *dev)
{
    static struct nvs_fs fs;
    struct sensor_value bat;
    int8_t ret = 0;
    uint16_t val_wrt[BAT_BUFFER_SIZE];
    uint16_t val_rd[BAT_BUFFER_SIZE];

    dev = DEVICE_DT_GET_ONE(st_stm32_vbat);

    ret = sensor_sample_fetch(dev);
    if (ret < 0 && ret != -EBADMSG) {        
	    printk("error: stm32 vbat sample is not up to date\n");
	    return 0;
    }

	ret = sensor_channel_get(dev, SENSOR_CHAN_VOLTAGE, &bat);
    if (ret < 0) {
        printk("error: can't read sensor channels\n");
	    return 0;
    }
 //   printk("stm32 vbat write: %d.%03d\n", bat.val1, bat.val2);

    if (bat_ind < BAT_BUFFER_SIZE) {
        val_wrt[bat_ind] = (sensor_value_to_milli(&bat)/10);
        printk("stm32 vbat: %d\n", val_wrt[bat_ind]);
        bat_ind ++;
    } else {
        (void)nvs_write(&fs, NVS_STM32_VBAT_ID, &val_wrt, sizeof(val_wrt));
        for (int8_t m; m < BAT_BUFFER_SIZE; m++) {
            printk("stm32 vbat write: %d\n", val_wrt[m]);
        }
        printk("\n\n");
        (void)nvs_read(&fs, NVS_STM32_VBAT_ID, &val_rd, sizeof(val_rd));
        for (int8_t n; n < BAT_BUFFER_SIZE; n++) {
            printk("stm32 vbat read: %d\n", val_rd[n]);
        }
        bat_ind =0;
    }
    return 0;
}
