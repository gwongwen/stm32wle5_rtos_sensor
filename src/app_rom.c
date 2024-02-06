/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

 #include "app_rom.h"

 int8_t app_rom_init(const struct device *dev)
 {
    dev = DEVICE_DT_GET(DT_ALIAS(eeprom0));

    if (dev = NULL) {
		printk("no eeprom device found. error: %d\n", dev);
		return 0;
	}
    if (!device_is_ready(dev)) {
		printk("eeprom is not ready. error: %d\n", dev);
		return 0;
	}
	return 0;
}

int8_t app_rom_write(const struct device *dev, uint16_t data)
{
	int8_t ret;

	dev = DEVICE_DT_GET(DT_ALIAS(eeprom0));

	ret = eeprom_write(dev, EEPROM_SAMPLE_OFFSET, &data, sizeof(data));
	if (ret  < 0){
		printk("couldn't write eeprom. error: %d\n", ret);
	}
	return 0;
}

uint16_t app_rom_read(const struct device *dev)
{
	int8_t ret;
	uint16_t data;

	dev = DEVICE_DT_GET(DT_ALIAS(eeprom0));

	ret = eeprom_read(dev, EEPROM_SAMPLE_OFFSET, &data, sizeof(data));
	if (ret  < 0){
		printk("couldn't read eeprom. error: %d\n", ret);
	}
	return data;
}