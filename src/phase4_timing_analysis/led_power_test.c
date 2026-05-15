#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED0_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
    if (!gpio_is_ready_dt(&led)) {
        return 0;
    }

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    while (1) {
        gpio_pin_set_dt(&led, 1);
        k_sleep(K_SECONDS(2));

        gpio_pin_set_dt(&led, 0);
        k_sleep(K_SECONDS(2));
    }

    return 0;
}