/*
 * Copyright (c) 2018 STMicroelectronics
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>
#include <zephyr/sys/util.h>

#include <zephyr/drivers/emul.h>
#include <zephyr/drivers/emul_sensor.h>
#include <zephyr/drivers/sensor_data_types.h>


// #ifdef CONFIG_LSM6DSL_TRIGGER
// static int lsm6dsl_trig_cnt;

// static void lsm6dsl_trigger_handler(const struct device *dev,
// 				    const struct sensor_trigger *trig)
// {
// 	sensor_sample_fetch_chan(dev, SENSOR_CHAN_ALL);
// 	lsm6dsl_trig_cnt++;
// }
// #endif

// int main(void)
// {
// 	struct sensor_value temp1, temp2, hum, press;
// 	struct sensor_value accel1[3], accel2[3];
// 	struct sensor_value gyro[3];
// 	struct sensor_value magn[3];
// 	const struct device *const hts221 = DEVICE_DT_GET_ONE(st_hts221);
// 	const struct device *const lps22hb = DEVICE_DT_GET_ONE(st_lps22hb_press);
// 	const struct device *const lsm6dsl = DEVICE_DT_GET_ONE(st_lsm6dsl);
// 	const struct device *const lsm303agr_a = DEVICE_DT_GET_ONE(st_lis2dh);
// 	const struct device *const lsm303agr_m = DEVICE_DT_GET_ONE(st_lis2mdl);
// #ifdef CONFIG_LSM6DSL_TRIGGER
// 	int cnt = 1;
// #endif

// 	if (!device_is_ready(hts221)) {
// 		printk("%s: device not ready.\n", hts221->name);
// 		return 0;
// 	}
// 	if (!device_is_ready(lps22hb)) {
// 		printk("%s: device not ready.\n", lps22hb->name);
// 		return 0;
// 	}
// 	if (!device_is_ready(lsm6dsl)) {
// 		printk("%s: device not ready.\n", lsm6dsl->name);
// 		return 0;
// 	}
// 	if (!device_is_ready(lsm303agr_a)) {
// 		printk("%s: device not ready.\n", lsm303agr_a->name);
// 		return 0;
// 	}
// 	if (!device_is_ready(lsm303agr_m)) {
// 		printk("%s: device not ready.\n", lsm303agr_m->name);
// 		return 0;
// 	}

// 	/* set LSM6DSL accel/gyro sampling frequency to 104 Hz */
// 	struct sensor_value odr_attr;

// 	odr_attr.val1 = 208;
// 	odr_attr.val2 = 0;

// 	if (sensor_attr_set(lsm6dsl, SENSOR_CHAN_ACCEL_XYZ,
// 			    SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr) < 0) {
// 		printk("Cannot set sampling frequency for accelerometer.\n");
// 		return 0;
// 	}

// 	if (sensor_attr_set(lsm6dsl, SENSOR_CHAN_GYRO_XYZ,
// 			    SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr) < 0) {
// 		printk("Cannot set sampling frequency for gyro.\n");
// 		return 0;
// 	}

// #ifdef CONFIG_LSM6DSL_TRIGGER
// 	struct sensor_trigger trig;

// 	trig.type = SENSOR_TRIG_DATA_READY;
// 	trig.chan = SENSOR_CHAN_ACCEL_XYZ;
// 	sensor_trigger_set(lsm6dsl, &trig, lsm6dsl_trigger_handler);
// #endif

// 	while (1) {
// 		int ret;

// 		/* Get sensor samples */

// 		if (sensor_sample_fetch(hts221) < 0) {
// 			printf("HTS221 Sensor sample update error\n");
// 			return 0;
// 		}
// 		if (sensor_sample_fetch(lps22hb) < 0) {
// 			printf("LPS22HB Sensor sample update error\n");
// 			return 0;
// 		}
// #ifndef CONFIG_LSM6DSL_TRIGGER
// 		if (sensor_sample_fetch(lsm6dsl) < 0) {
// 			printf("LSM6DSL Sensor sample update error\n");
// 			return 0;
// 		}
// #endif
// 		ret = sensor_sample_fetch(lsm303agr_a);
// 		if (ret < 0 && ret != -EBADMSG) {
// 			printf("LSM303AGR Accel Sensor sample update error\n");
// 			return 0;
// 		}
// 		if (sensor_sample_fetch(lsm303agr_m) < 0) {
// 			printf("LSM303AGR Magn Sensor sample update error\n");
// 			return 0;
// 		}

// 		/* Get sensor data */

// 		sensor_channel_get(hts221, SENSOR_CHAN_AMBIENT_TEMP, &temp1);
// 		sensor_channel_get(hts221, SENSOR_CHAN_HUMIDITY, &hum);
// 		sensor_channel_get(lps22hb, SENSOR_CHAN_PRESS, &press);
// 		sensor_channel_get(lps22hb, SENSOR_CHAN_AMBIENT_TEMP, &temp2);
// 		sensor_channel_get(lsm6dsl, SENSOR_CHAN_ACCEL_XYZ, accel1);
// 		sensor_channel_get(lsm6dsl, SENSOR_CHAN_GYRO_XYZ, gyro);
// 		sensor_channel_get(lsm303agr_a, SENSOR_CHAN_ACCEL_XYZ, accel2);
// 		sensor_channel_get(lsm303agr_m, SENSOR_CHAN_MAGN_XYZ, magn);

// 		/* Display sensor data */

// 		/* Erase previous */
// 		printf("\0033\014");

// 		printf("X-NUCLEO-IKS01A2 sensor dashboard\n\n");

// 		/* temperature */
// 		printf("HTS221: Temperature: %.1f C\n",
// 		       sensor_value_to_double(&temp1));

// 		/* humidity */
// 		printf("HTS221: Relative Humidity: %.1f%%\n",
// 		       sensor_value_to_double(&hum));

// 		/* pressure */
// 		printf("LPS22HB: Pressure:%.3f kpa\n",
// 		       sensor_value_to_double(&press));

// 		/* lps22hb temperature */
// 		printf("LPS22HB: Temperature: %.1f C\n",
// 		       sensor_value_to_double(&temp2));

// 		/* lsm6dsl accel */
// 		printf("LSM6DSL: Accel (m.s-2): x: %.1f, y: %.1f, z: %.1f\n",
// 		       sensor_value_to_double(&accel1[0]),
// 		       sensor_value_to_double(&accel1[1]),
// 		       sensor_value_to_double(&accel1[2]));

// 		/* lsm6dsl gyro */
// 		printf("LSM6DSL: Gyro (dps): x: %.3f, y: %.3f, z: %.3f\n",
// 		       sensor_value_to_double(&gyro[0]),
// 		       sensor_value_to_double(&gyro[1]),
// 		       sensor_value_to_double(&gyro[2]));

// #if defined(CONFIG_LSM6DSL_TRIGGER)
// 		printf("%d:: lsm6dsl trig %d\n", cnt++, lsm6dsl_trig_cnt);
// #endif

// 		/* lsm303agr accel */
// 		printf("LSM303AGR: Accel (m.s-2): x: %.1f, y: %.1f, z: %.1f\n",
// 		       sensor_value_to_double(&accel2[0]),
// 		       sensor_value_to_double(&accel2[1]),
// 		       sensor_value_to_double(&accel2[2]));

// 		/* lsm303agr magn */
// 		printf("LSM303AGR: Magn (gauss): x: %.3f, y: %.3f, z: %.3f\n",
// 		       sensor_value_to_double(&magn[0]),
// 		       sensor_value_to_double(&magn[1]),
// 		       sensor_value_to_double(&magn[2]));

// 		k_sleep(K_MSEC(2000));
// 	}
// }


// // static bool emul_sensor_backend_is_supported(const struct emul *target);
// // static int emul_sensor_backend_set_channel(const struct emul *target, struct sensor_chan_spec ch, const q31_t *value, int8_t shift);
// // static int emul_sensor_backend_get_sample_range(const struct emul *target, struct sensor_chan_spec ch, q31_t *lower, q31_t *upper, q31_t *epsilon, int8_t *shift);
// // static int emul_sensor_backend_set_attribute(const struct emul *target, struct sensor_chan_spec ch, enum sensor_attribute attribute, const void *value);
// // static int emul_sensor_backend_get_attribute_metadata(const struct emul *target, struct sensor_chan_spec ch, enum sensor_attribute attribute, q31_t *min, q31_t *max, q31_t *increment, int8_t *shift);


#define SENSOR_Q31_SCALE ((q31_t)(1 << 30))

void main(void)
{

	DEVICE_DT_DEFINE(DT_NODELABEL(st_hts221))

	// EMUL_DT_DEFINE(DT_NODELABEL(st_hts221),)



    const struct device *dev =  DEVICE_DT_GET_ONE(st_hts221);//device_get_binding("st_hts221");

    // if (!dev) {
    //     printk("No device found; did initialization fail?\n");
    //     return;
    // }

    // if (!device_is_ready(dev)) {
    //     printk("Device is not ready\n");
    //     return;
    // }

    const struct emul *emul = emul_get_binding(dev->name);
    if (!emul) {
        printk("No emulator found for HTS221\n");
        return;
    }

    if (!emul_sensor_backend_is_supported(emul)) {
        printk("Sensor emulator backend not supported\n");
        return;
    }

    /* Set expected temperature and humidity values */
    q31_t temp_value = (q31_t)(25 * SENSOR_Q31_SCALE);
    q31_t hum_value = (q31_t)(50 * SENSOR_Q31_SCALE);

    struct sensor_chan_spec temp_chan = {
        .chan_type = SENSOR_CHAN_AMBIENT_TEMP,
        .chan_idx = 0,
    };

    struct sensor_chan_spec hum_chan = {
        .chan_type = SENSOR_CHAN_HUMIDITY,
        .chan_idx = 0,
    };

    emul_sensor_backend_set_channel(emul, temp_chan, &temp_value, 0);
    emul_sensor_backend_set_channel(emul, hum_chan, &hum_value, 0);

    while (1) {
        // sensor_sample_fetch(dev);
        struct sensor_value temp_val, hum_val;
        // sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp_val);
        // sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum_val);

        printk("Temperature: %d.%06d C\n", temp_val.val1, temp_val.val2);
        printk("Humidity: %d.%06d %%\n", hum_val.val1, hum_val.val2);
        k_sleep(K_SECONDS(1));
    }
}