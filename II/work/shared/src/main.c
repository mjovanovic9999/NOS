#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(hts221_x_nucleo_iks01a2),
// gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led0), gpios);

int main(void)
{
	if (!device_is_ready(led.port)) {
		return -1;
	}

	int ret;
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret != 0) {
		return ret;
	}

	int i = 0;
	while (i < 10) {

		ret = gpio_pin_toggle_dt(&led);
		if (ret != 0) {
			return ret;
		}

		k_msleep(2000);
		printf("sleep\n");

		int pin_value = gpio_pin_get(led.port, led.pin);

		if (pin_value < 0) {
			printf("Error reading the GPIO pin\n");
		} else {
			printf("GPIO pin value: %d\n", pin_value);
		}


        // Change pin configuration to input to read its value
        // ret = gpio_pin_configure_dt(&led, GPIO_INPUT);
        // if (ret < 0) {
        //     printk("Error configuring the GPIO pin as input\n");
        //     return ret;
        // }

        // int pin_value = gpio_pin_get(led.port, led.pin);
        // if (pin_value < 0) {
        //     printk("Error reading the GPIO pin\n");
        // } else {
        //     printk("GPIO pin value: %d\n", pin_value);
        // }

        // // Reconfigure pin as output for the next toggle
        // ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
        // if (ret < 0) {
        //     printk("Error reconfiguring the GPIO pin as output\n");
        //     return ret;
        // }


		i++;
	}

	printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

	return 0;
}
