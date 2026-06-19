#include "arm_nucleof446re.h"
#include "arm_gpio_driver.h"
#include "arm_stm32f446xx.h"
#include "arm_spi_driver.h"


int main(void){

    // turn on the clock for GPIOC 
    GPIO_clk_cfg(GPIOC,1);
    // setup GPIO 
   gpio_pinconfig_t gpio_button_init = 
   {
    .gpio_pinmode = GPIO_MODE_IT_FE ,
    .gpio_pinnumber = PIN_NO_13 ,
    .gpio_pinspeed = GPIO_SPEED_HIGH ,
    .gpio_pinpupdcontrol = GPIO_PIN_PU 

   };

   gpio_handle_t gpio_button_handle = 
   {
    .pGPIOx = GPIOC,
    .gpio_pinconfig = &gpio_button_init
   };

   GPIO_init(&gpio_button_handle);

   GPIO_IRQ_config(40, 1);


    for(;;);
    
};

 void EXTI15_10_IRQHandler(void)
   {
    // delay to debounce button
    for (int i = 0; i < 5000000/2; i++);
    GPIO_IRQ_handle(PIN_NO_13);
    user_LED_toggle();

   };