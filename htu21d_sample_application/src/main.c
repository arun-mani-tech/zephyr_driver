/*
 * Copyright (c) 2020 Linumiz
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <sys/printk.h>
/* pn: we shouldn't need this AFAIK */
#include <sys/__assert.h>

static void do_main(const struct device *dev)
{
	int ret;
	struct sensor_value temp_value;
	struct sensor_value hum_value;
	/* pn: should have new line here and this applies to whole file */
	while (1) {
		ret = sensor_sample_fetch(dev);
		if (ret) {
			printk("sensor_sample_fetch failed ret %d\n", ret);
			return;
		}
		/* pn: should have new line here and this applies to whole file */
		ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp_value);
		if (ret) {
			printk("sensor_channel_get failed ret %d\n", ret);
			return;
		}
		/* pn: should have new line here and this applies to whole file */
		printk("temp is %d (%d micro)\n", temp_value.val1,
		       temp_value.val2);
		k_sleep(K_MSEC(1000));
		/* pn: should have new line here and this applies to whole file */
		ret = sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum_value);
                if (ret) {
                        printk("sensor_channel_get failed ret %d\n", ret);
                        return;
                }
		/* pn: should have new line here and this applies to whole file */
                printk("humidity is %d (%d micro)\n", hum_value.val1,
                       hum_value.val2);
                k_sleep(K_MSEC(1000));
	}
}

void main(void)
{
	/* pn: I think we should get label macro here instead of hardcoded value */
	const struct device *dev = device_get_binding("HTU21D");
	/* pn: spacing issue */
	/* pn: should have new line here and this applies to whole file */
	if(dev==NULL){
		printk("Failed to get the device\n");
		return;
	}
	/* pn: should have new line here and this applies to whole file */
	do_main(dev);
}
