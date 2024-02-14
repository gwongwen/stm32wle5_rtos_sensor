/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_BME280_H
#define APP_BME280_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define TPH_BUFFER_SIZE     3*48        // 1 sample per 30mins per sensor
                                        // 3*48 samples (tph) in uint16 format

int8_t app_bme280_init(const struct device *dev);
uint16_t app_bme280_get_temp(const struct device *dev);
uint16_t app_bme280_get_press(const struct device *dev);
uint16_t app_bme280_get_hum(const struct device *dev);


#endif /* APP_BME280_H */