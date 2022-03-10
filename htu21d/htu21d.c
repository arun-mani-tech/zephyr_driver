/* HTU21D temperature & humidity sensor with i2c
 *
 * Copyright (c) 2022 Linumiz
 * Author: Arunmani <arunmani@linumiz.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT te_htu21d
#include <zephyr.h>
#include <stdio.h>
#include "htu21d.h"
#include <logging/log.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/sensor.h> 
#include <drivers/i2c.h>

LOG_MODULE_REGISTER(htu21d, CONFIG_SENSOR_LOG_LEVEL);

struct htu21d_data {
	const struct device *i2c;
	int16_t temp;
	int16_t humidity;
};

/* pn: this is not required, just use CONFIG_HTU21D_NH_MASTER */
#ifdef CONFIG_HTU21D_NH_MASTER
#define NO_HOLD_MASTER_MODE	1
#endif

static int send_device_cmd(struct htu21d_data *drv_data, uint16_t *val, const char cmd)
{
	int ret;
	/* pn: should be int counter = 0 and this applies to whole file */
	int counter=0;
	uint8_t value[3];
	value[0] = cmd;
	/* pn: should have new line here and this applies to whole file */
	/* pn: can be changed to while(counter--) if counter = MAX_READ_RETRY_COUNT */
	while(1){
		if (NO_HOLD_MASTER_MODE) {
			k_sleep(K_MSEC(MEASURE_WAIT_MS));
			counter++;
		}
		/* pn: should have new line here and this applies to whole file */
		ret = i2c_write(drv_data->i2c,&value[0],1,HTU21D_I2CADDR);	/*to write register address*/
		if(ret){	
			LOG_ERR("Error writing to HTU21D! (0x%X)\n", value[0]);
			/* pn: error case should return back to caller */
		}
		/* pn: should have new line here and this applies to whole file */
		if (i2c_read(drv_data->i2c, &value, 3, HTU21D_I2CADDR) != 0){ 	/*read values*/
			if (NO_HOLD_MASTER_MODE) {
				LOG_ERR("Retry! (0x%X)....\n",value[0]);
				if (counter >= MAX_READ_RETRY_COUNT) {
					break;
				}
				continue;
			}
		}
		break;
	}
	/* pn: should have new line here and this applies to whole file */
	*val = (value[0] << 8 | value[1]) & 0xFFFC;
	/* pn: should have new line here and this applies to whole file */
	return 0;
}
/* pn: should have new line here and this applies to whole file */
static int htu21d_sample_fetch(const struct device *dev,
		enum sensor_channel *chan)
{
	struct htu21d_data *drv_data = dev->data;
	uint16_t value,value2;
	drv_data->temp = 0U;
	drv_data->humidity = 0U;

	/* pn: should we not differentiate SENSOR_CHAN_HUMIDITY and SENSOR_CHAN_AMBIENT_TEMP here too? */
	/* Trigger Temperature Measurement*/
	if (send_device_cmd(drv_data, &value, (NO_HOLD_MASTER_MODE
					? HTU21D_READ_TEMP_NH : 
					HTU21D_READ_TEMP_H)) != 0){
		return -EIO;
	}
	/* pn: should have new line here and this applies to whole file */

	drv_data->temp = value; 
	/* Trigger Humidity Measurement*/
	if (send_device_cmd(drv_data, &value, (NO_HOLD_MASTER_MODE
					? HTU21D_READ_HUMID_NH : 
					HTU21D_READ_HUMID_H)) != 0){
		return -EIO;
	}
	/* pn: should have new line here and this applies to whole file */
	drv_data->humidity = value;
	/* pn: should have new line here and this applies to whole file */
	return 0;
}
/* pn: should have new line here and this applies to whole file */
static int htu21d_channel_get(const struct device *dev,
		enum sensor_channel chan,
		struct sensor_value *val)
{
	struct htu21d_data *drv_data = dev->data;
	int32_t temp_val,humidity_val;
	double sensor_tmp_val,sensor_humidity_val;
	double result;
	float float_val;
	temp_val = drv_data->temp;
	/* pn: magic value 65536.0, this should be from macro or documented */
	sensor_tmp_val = temp_val/65536.0;
	humidity_val = drv_data->humidity;
	/* pn: magic value 65536.0, this should be from macro or documented */
	sensor_humidity_val = humidity_val/65536.0;
	/* pn: should have new line here and this applies to whole file */
	if(chan != SENSOR_CHAN_AMBIENT_TEMP && chan != SENSOR_CHAN_HUMIDITY){
		return -ENOTSUP;
	}
	/* pn: should have new line here and this applies to whole file */
	switch (chan){
	case SENSOR_CHAN_AMBIENT_TEMP:
		/*temperature calc*/
		/* pn: magic value */
		result = -46.85  + (175.72 * sensor_tmp_val);
		val->val1 = (int)result;
		/* pn: magic value */
		/* pn: spacing not proper */
		float_val=(result-(int)result)*10000;
		/* pn: magic value */
		/* pn: spacing not proper */
		val->val2 = (int)float_val/100;
		break;
	case SENSOR_CHAN_HUMIDITY:
		/*humidity calc*/
		/* pn: magic value */
		result = -6.0 + (125.0 * sensor_humidity_val);
		val->val1 = (int)result;
		/* pn: magic value */
		/* pn: spacing not proper */
		float_val=(result-(int)result)*10000;
		/* pn: magic value */
		/* pn: spacing not proper */
		val->val2 = (int)float_val/100;
		break;
	}	
	return 0;
}
/* pn: should have new line here and this applies to whole file */
static const struct sensor_driver_api htu21d_driver_api = {
	.sample_fetch = htu21d_sample_fetch,
	.channel_get = htu21d_channel_get,
};
/* pn: should have new line here and this applies to whole file */
static int htu21d_init(const struct device *dev)
{
	struct htu21d_data *drv_data = dev->data;
	/* pn: should have new line here and this applies to whole file */
	drv_data->i2c = device_get_binding(DT_INST_BUS_LABEL(0));
	/* pn: should have new line here and this applies to whole file */
	if (drv_data->i2c == NULL) {
		LOG_DBG("Failed to get pointer to %s device!",
				DT_INST_BUS_LABEL(0));
		return -EINVAL;
	}
	/* pn: should have new line here and this applies to whole file */
	/*HTU21D reset*/
	i2c_write(drv_data->i2c, HTU21D_SOFT_RESET, 1, HTU21D_I2CADDR);
	k_sleep(K_MSEC(STARTUP_WAIT_MS));
	/* pn: should have new line here and this applies to whole file */
	return 0;
}
static struct htu21d_data htu21d_drv_data;
DEVICE_DT_INST_DEFINE(0, htu21d_init, NULL, &htu21d_drv_data, NULL, POST_KERNEL,
		CONFIG_SENSOR_INIT_PRIORITY, &htu21d_driver_api);
