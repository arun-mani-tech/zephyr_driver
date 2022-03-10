/* HTU21D temperature & humidity sensor with i2c
 *
 * Copyright (c) 2022 Linumiz
 * Author: Arunmani <arunmani@linumiz.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_DRIVERS_SENSOR_HTU21D_H_
#define ZEPHYR_DRIVERS_SENSOR_HTU21D_H_


#define HTU21D_I2CADDR 0x40

#define HTU21D_SOFT_RESET 0xFE
#define HTU21D_READ_TEMP_NH 0xF3
#define HTU21D_READ_HUMID_NH 0xF5
#define HTU21D_READ_TEMP_H 0xE3
#define HTU21D_READ_HUMID_H 0xE5

#define STARTUP_WAIT_MS 15
#define MEASURE_WAIT_MS 50

#define MAX_READ_RETRY_COUNT 5
#define NO_HOLD_MASTER_MODE 0
#endif
