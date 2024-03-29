/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_flash.h"
#include "app_bme280.h"
#include "app_vbat.h"

//  ======== interrupt sub-routine ===============================
void sens_work_handler(struct k_work *work_rtc)
{
	struct nvs_fs flash;

	printk("sensor handler called\n");
	app_flash_handler(&flash);
}
K_WORK_DEFINE(sens_work, sens_work_handler);

void sens_timer_handler(struct k_timer *rtc_dum)
{
	k_work_submit(&sens_work);
}
K_TIMER_DEFINE(sens_timer, sens_timer_handler, NULL);

//  ======== main ===============================================
int main(void)
{
	const struct device *bme_dev = NULL;
	const struct device *bat_dev = NULL;
	struct nvs_fs flash;

	// setup all devices
	app_bme280_init(bme_dev);
	app_stm32_vbat_init(bat_dev);
	app_flash_init(&flash);
	
	printk("Sensor BME280 and Battery Example\nBoard: %s\n", CONFIG_BOARD);

	// beginning of interrupt subroutine
	k_timer_start(&sens_timer, K_MSEC(2000), K_MSEC(2000));		// for test
//	k_timer_start(&sens_timer, K_MINUES(30), K_MINUTES(30));	// final code

	return 0;
}