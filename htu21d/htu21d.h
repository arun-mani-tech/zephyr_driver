/* HTU21D temperature & humidity sensor with i2c
 *
 * Copyright (c) 2022 Linumiz
 * Author: Arunmani <arunmani@linumiz.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_DRIVERS_SENSOR_HTU21D_H_
#define ZEPHYR_DRIVERS_SENSOR_HTU21D_H_

/*
   pn: I don't understand this register. Is this I2C slave address?
   if so, this should come from devicetree
*/
#define HTU21D_I2CADDR		0x40

#define HTU21D_READ_TEMP_H	0xE3
#define HTU21D_READ_HUMID_H	0xE5
#define HTU21D_READ_TEMP_NH	0xF3
#define HTU21D_READ_HUMID_NH	0xF5
#define HTU21D_SOFT_RESET	0xFE

#define STARTUP_WAIT_MS		15
#define MEASURE_WAIT_MS		50

/* pn: I think this can go to respective function? */
#define MAX_READ_RETRY_COUNT	5
/* pn: this is not required, just use CONFIG_HTU21D_NH_MASTER */
#define NO_HOLD_MASTER_MODE	0

#endif
