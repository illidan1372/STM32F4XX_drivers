
#ifndef INC_ARM_GPIO_DRIVER_H_
#define INC_ARM_GPIO_DRIVER_H_

#include "arm_stm32f446xx.h"
#include <stdint.h>
/* configuration structure for a gpio pin */

typedef struct  
{
    uint8_t gpio_pinnumber; // pin number possible options @GPIO_PIN_NUMBERS
    uint8_t gpio_pinmode; //pin mode possible options @GPIO_PIN_MODES
    uint8_t gpio_pinspeed; // pin speed possible options @GPIO_SPEED
    uint8_t gpio_pinpupdcontrol; // pull up-pull down mode possible options @GPIO_PUPD_CONTROL
    uint8_t gpio_pinoptype; // output type possible options @GPIO_PIN_OUTPUT_TYPES
    uint8_t gpio_pinaltfunmode; // alternative function mode possible options @GPIO_ALTERNATE_FUNCTION_MODES


}gpio_pinconfig_t;


typedef struct 
{
    GPIO_REGDEF_t * pGPIOx;  // this holds the base address of the gpio port that you are setting up
    gpio_pinconfig_t * gpio_pinconfig; // this holds the gpio pin configuration settings

}gpio_handle_t ;


                                                          /* function prototypes*/

                                                         /* GPIO function prototypes */ 

// a function to control the gpio peripheral clock register
// state means enable clock or disable clock
void GPIO_clk_cfg(GPIO_REGDEF_t *GPIO_port , uint8_t state);                                                         
// a function to initialise gpio port 
void GPIO_init(gpio_handle_t *pGPIOx);
// a function to de-initialise gpio port
void GPIO_deinit(GPIO_REGDEF_t *GPIO_port);
// a function to write to output pin
// since gpio pins are either 1 or 0
// state means set = 1 or reset = 0
void GPIO_pin_wt(GPIO_REGDEF_t *GPIO_port , uint8_t pin , uint8_t state);
// a function to write to output port
void GPIO_port_wt(GPIO_REGDEF_t *GPIO_port , uint16_t data);
// a function to read from pin input
uint8_t GPIO_pin_rd(GPIO_REGDEF_t *GPIO_port , uint8_t pin);
// a function to read the entire port 
uint16_t GPIO_port_rd(GPIO_REGDEF_t *GPIO_port);
// function to toggle pin
void GPIO_pin_toggle(GPIO_REGDEF_t *GPIO_port , uint8_t pin);
// a function to setup interrupt ( set priority etc)
// state means either enable or disable
void GPIO_IRQ_config(uint8_t IRQ_number  , uint8_t state);
// a call-back function that does something whenever an interrupt happens 
void GPIO_IRQ_handle(uint8_t pin);

void GPIO_IRQ_priority_config(uint8_t IRQ_number , uint8_t priority);


                                            /* GPIO available modes macros*/

                                            // @GPIO_PIN_MODES

// output mode
#define GPIO_MODE_OUT   1U
// input mode
#define GPIO_MODE_IN    0U
// alternate function mode
#define GPIO_MODE_ALTFN 2U
// analog mode
#define GPIO_MODE_ANLG  3U
// interrupt mode rising edge
#define GPIO_MODE_IT_RE  5U
// interrupt mode falling edge
#define GPIO_MODE_IT_FE  4U
// interrupt mode rising falling edge
#define GPIO_MODE_IT_RFE 6U
                                            // GPIO  output types
                                            //@GPIO_PIN_OUTPUT_TYPES

// output push pull mode                                             
#define GPIO_OT_TYPE_PP 0U
// output open drain mode                                             
#define GPIO_OT_TYPE_OD 1U

                                            // GPIO output speeds
                                            // @GPIO_SPEED
#define GPIO_SPEED_LOW        0U
#define GPIO_SPEED_MEDIUM     1U
#define GPIO_SPEED_HIGH       2U
#define GPIO_SPEED_VERYHIGH   3U
                                            //GPIO pull up and pull down conf macros
                                            //@GPIO_PUPD_CONTROL
                                            
#define GPIO_NO_PUPD          0U // no pull up , pull down mode
#define GPIO_PIN_PU           1U// pull up mode
#define GPIO_PIN_PD           2U // pull down mode

                                            //GPIO pin numbers
                                            //@GPIO_PIN_NUMBERS
#define PIN_NO_0              0U
#define PIN_NO_1              1U
#define PIN_NO_2              2U
#define PIN_NO_3              3U
#define PIN_NO_4              4U
#define PIN_NO_5              5U
#define PIN_NO_6              6U
#define PIN_NO_7              7U
#define PIN_NO_8              8U
#define PIN_NO_9              9U
#define PIN_NO_10             10U
#define PIN_NO_11             11U
#define PIN_NO_12             12U
#define PIN_NO_13             13U
#define PIN_NO_14             14U
#define PIN_NO_15             15U
                                            //GPIO EXTI pin patterns numbers
                                            //@GPIO_EXTI_PATTERNS
#define EXTI_GPIOA_PATTERN     0X0
#define EXTI_GPIOB_PATTERN     0X1
#define EXTI_GPIOC_PATTERN     0X2
#define EXTI_GPIOD_PATTERN     0X3
#define EXTI_GPIOE_PATTERN     0X4
#define EXTI_GPIOF_PATTERN     0X5
#define EXTI_GPIOG_PATTERN     0X6
#define EXTI_GPIOH_PATTERN     0X7

                                    // GPIO pin reset macros
                                    // @GPIO_REG_RESET
#define GPIOA_REG_RESET       do {RCC -> RCC_AHB1_RSTR |= (1 << 0); RCC -> RCC_AHB1_RSTR &= ~(1 << 0); }while(0)
#define GPIOB_REG_RESET       do {RCC -> RCC_AHB1_RSTR |= (1 << 1); RCC -> RCC_AHB1_RSTR &= ~(1 << 1); }while(0)
#define GPIOC_REG_RESET       do {RCC -> RCC_AHB1_RSTR |= (1 << 2); RCC -> RCC_AHB1_RSTR &= ~(1 << 2); }while(0)
#define GPIOD_REG_RESET       do {RCC -> RCC_AHB1_RSTR |= (1 << 3); RCC -> RCC_AHB1_RSTR &= ~(1 << 3); }while(0)
#define GPIOE_REG_RESET       do {RCC -> RCC_AHB1_RSTR |= (1 << 4); RCC -> RCC_AHB1_RSTR &= ~(1 << 4); }while(0)
#define GPIOF_REG_RESET       do {RCC -> RCC_AHB1_RSTR |= (1 << 5); RCC -> RCC_AHB1_RSTR &= ~(1 << 5); }while(0)
#define GPIOG_REG_RESET       do {RCC -> RCC_AHB1_RSTR |= (1 << 6); RCC -> RCC_AHB1_RSTR &= ~(1 << 6); }while(0)
#define GPIOH_REG_RESET       do {RCC -> RCC_AHB1_RSTR |= (1 << 7); RCC -> RCC_AHB1_RSTR &= ~(1 << 7); }while(0)

#endif