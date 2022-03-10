/*
 * Copyright (c) 2016 Firmwave
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <sys/printk.h>
#include <sys/__assert.h>

static void do_main(const struct device *dev)
{
	int ret;
	struct sensor_value temp_value;
	struct sensor_value hum_value;
	while (1) {
		ret = sensor_sample_fetch(dev);
		if (ret) {
			printk("sensor_sample_fetch failed ret %d\n", ret);
			return;
		}
		ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp_value);
		if (ret) {
			printk("sensor_channel_get failed ret %d\n", ret);
			return;
		}
		printk("temp is %d (%d micro)\n", temp_value.val1,
		       temp_value.val2);
		k_sleep(K_MSEC(1000));
		ret = sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum_value);
                if (ret) {
                        printk("sensor_channel_get failed ret %d\n", ret);
                        return;
                }
                printk("humidity is %d (%d micro)\n", hum_value.val1,
                       hum_value.val2);
                k_sleep(K_MSEC(1000));
	}
}

void main(void)
{
	const struct device *dev = device_get_binding("HTU21D");
	if(dev==NULL){
		printk("Failed to get the device\n");
		return;
	}
	do_main(dev);
}
