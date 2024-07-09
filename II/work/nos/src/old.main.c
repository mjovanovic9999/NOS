#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

// nzm za sta ovo
//  static const struct gpio_dt_spec temp_hum =
//  GPIO_DT_SPEC_GET(DT_NODELABEL(hts221_x_nucleo_iks01a2), gpios);

static const struct device * dev = DEVICE_DT_GET_ONE(st_hts221);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led0), gpios);


static void process_sample()
{
	static unsigned int obs;
	struct sensor_value temp, hum;
	if (sensor_sample_fetch(dev) < 0) {
		printf("Sensor sample update error\n");
		return;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp) < 0) {
		printf("Cannot read HTS221 temperature channel\n");
		return;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum) < 0) {
		printf("Cannot read HTS221 humidity channel\n");
		return;
	}

	++obs;
	printf("Observation:%u\n", obs);

	/* display temperature */
	printf("Temperature:%.1f C\n", sensor_value_to_double(&temp));

	/* display humidity */
	printf("Relative Humidity:%.1f%%\n",
	       sensor_value_to_double(&hum));
}

int main(void)
{
	if (!device_is_ready(led.port)) {
		return -1;
	}

	if (!device_is_ready(dev)) {
		printf("sensor: st_hts221 not ready.\n");
		printk("sensor: st_hts221 not ready.\n");
		return 0;
	}
	printf("sensor: st_hts221 is ready.\n");
	printk("sensor: st_hts221 is ready.\n");

	// int ret;
	// ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	// if (ret != 0) {
	// 	return ret;
	// }

	// int i = 0;
	// while (i < 20) {

	// 	ret = gpio_pin_toggle_dt(&led);
	// 	if (ret != 0) {
	// 		return ret;
	// 	}

	// 	k_msleep(2000);
	// 	printf("sleep\n");

	// 	i++;
	// }

	printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

	return 0;
}
