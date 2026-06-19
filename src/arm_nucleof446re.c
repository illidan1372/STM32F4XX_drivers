#include <stdint.h>
#include "arm_gpio_driver.h"
void usr_LED_set(uint8_t state)
{
     GPIO_clk_cfg(GPIOA,1);
     gpio_pinconfig_t gpio_conf = {PIN_NO_5 , GPIO_MODE_OUT};
     gpio_handle_t  gpiob = {GPIOA , &gpio_conf};
     GPIO_init(&gpiob);
     GPIO_pin_wt(GPIOA,  PIN_NO_5 , state);

}

void user_LED_toggle(void)
{
     GPIO_clk_cfg(GPIOA,1);
     gpio_pinconfig_t gpio_conf = {PIN_NO_5 , GPIO_MODE_OUT};
     gpio_handle_t  led_config = {GPIOA , &gpio_conf};
     GPIO_init(&led_config);
     GPIO_pin_toggle(GPIOA , PIN_NO_5);

};

