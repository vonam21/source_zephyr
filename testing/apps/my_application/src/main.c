#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define K_MS_SLEEP 200
#define K_MS_SLEEP_1 1000

#define LEDPC13 DT_NODELABEL(led_1)
const struct gpio_dt_spec ledpc13 = GPIO_DT_SPEC_GET(LEDPC13,gpios);

int main(){
    int ret;
    if((gpio_is_ready_dt(&ledpc13))==0)
            return 0;
    ret = gpio_pin_configure_dt(&ledpc13,GPIO_OUTPUT_ACTIVE);
    if(ret <0)
            return 0;
    k_msleep(5000);
    while(1){
        gpio_pin_set_dt(&ledpc13,1);
        k_msleep(K_MS_SLEEP);
        gpio_pin_set_dt(&ledpc13,0);
        k_msleep(K_MS_SLEEP_1);
    }
}