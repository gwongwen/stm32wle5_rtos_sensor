/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */
 

#include <zephyr/device.h>
#include <zephyr/kernel.h>

#include "app_bme280.h"
#include "app_vbat.h"

void sens_work_handler(struct k_work *work_rtc)
{
	const struct device *flash_dev = NULL;

	printk("sensor handler called\n");
	app_flash_handler(flash_dev);
}
K_WORK_DEFINE(sens_work, sens_work_handler);

void sens_timer_handler(struct k_timer *rtc_dum)
{
	k_work_submit(&sens_work);
}
K_TIMER_DEFINE(sens_timer, sens_timer_handler, NULL);

int main(void)
{
	const struct device *bme_dev = NULL;
	const struct device *bat_dev = NULL;
	const struct device *flash_dev = NULL;

	app_bme280_init(bme_dev);
	app_stm32_vbat_init(bat_dev);
	app_flash_init(flash_dev);
	
	printk("sensor BME280 and Battery Example\nBoard: %s\n", CONFIG_BOARD);

	k_timer_start(&sens_timer, K_MSEC(5000), K_MSEC(5000));

	return 0;
}