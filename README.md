# Code for 6sens Omnitilt Project : test of STM32 ADC

## Overview
This first code allow us to convert a voltage and add processing to it :

 - take sample of battery level, temperature and pressure levels

## Building and Running

The following commands clean build folder, build and flash the sample:

**Command to use**

west build -t pristine

west build -p always -b stm32wl_dw1000_iot_board applications/stm32wle5_rtos_adc

west flash --runner stm32cubeprogrammer