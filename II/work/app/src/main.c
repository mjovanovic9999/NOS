#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/gpio.h>

static double process_sample_lps22hb(const struct device *dev)
{
	static unsigned int obs;
	struct sensor_value temp, pressure;
	if (sensor_sample_fetch(dev) < 0) {
		printf("Sensor lps22hb sample update error\n");
		return -1.0;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp) < 0) {
		printf("Cannot read lps22hb temperature channel\n");
		return -1.0;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_PRESS, &pressure) < 0) {
		printf("Cannot read lps22hb pressure channel\n");
		return -1.0;
	}

	++obs;
	printf("\nObservation:%u\n", obs);
	printf("LPS22HB pressure:%.1f kPa\n", sensor_value_to_double(&pressure));
	printf("LPS22HB temperature:%.1f C\n", sensor_value_to_double(&temp));

	return sensor_value_to_double(&temp);
}

static double process_sample_hts221(const struct device *dev)
{
	struct sensor_value temp, hum;
	if (sensor_sample_fetch(dev) < 0) {
		printf("Sensor hts221 sample update error\n");
		return -1.0;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp) < 0) {
		printf("Cannot read HTS221 temperature channel\n");
		return -1.0;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum) < 0) {
		printf("Cannot read HTS221 humidity channel\n");
		return -1.0;
	}

	printf("HTS221 temperature:%.1f C\n", sensor_value_to_double(&temp));
	printf("HTS221 relative humidity:%.1f%%\n", sensor_value_to_double(&hum));

	return sensor_value_to_double(&temp);
}

int main(void)
{
	int ret;

	const struct device *const hts221 = DEVICE_DT_GET_ONE(st_hts221);
	const struct device *const lps22hb = DEVICE_DT_GET_ONE(st_lps22hb_press);

	bool led_state = false;
	static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

	if (!device_is_ready(hts221)) {
		printk("hts221: device not ready.\n");
		return 0;
	}

	if (!device_is_ready(lps22hb)) {
		printk("lps22hb: device not ready.\n");
		return 0;
	}

	if (!gpio_is_ready_dt(&led)) {
		printk("led: device not ready.\n");
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (!gpio_is_ready_dt(&led)) {
		printk("led: error config.\n");
		return 0;
	}

	double hts_temp, lps_temp, res;
	while (1) {
		lps_temp = process_sample_lps22hb(lps22hb);
		k_sleep(K_MSEC(2000));
		hts_temp = process_sample_hts221(hts221);

		if (hts_temp > lps_temp) {
			res = hts_temp / lps_temp;
		} else {
			res = lps_temp / hts_temp;
		};
		res = (res - 1) * 100;

		printf("Difference: %.1f%%\n", res);

		if (res > 1) {
			gpio_pin_set_dt(&led, 1);
			printf("Bigger than 1%%, led on!\n");
			led_state = true;

		} else {
			gpio_pin_set_dt(&led, 0);
			printf("Smaller than 1%%, led off!\n");
		}
		if (ret < 0) {
			printf("Error gpio set\n");
			led_state = false;
			return 0;
		}
		k_sleep(K_MSEC(4000));
	}

	return 0;
}