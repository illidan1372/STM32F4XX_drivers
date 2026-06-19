#include "arm_gpio_driver.h"
#include "arm_stm32f446xx.h"
#include <stdint.h>

/**
 * @brief  This function takes a GPIO port name as well as state (0 or 1) and sets up the clock for that particular GPIO port 
 *
 * @param  GPIO_port  GPIO port name (GPIOA...GPIOH)
 * @param  state  0 or 1 , to turn the clock on or off
 *
 * @return        none
 */
void GPIO_clk_cfg(GPIO_REGDEF_t *GPIO_port , uint8_t state)
{
    if (state == 1) {
        if (GPIO_port == GPIOA) {
                GPIOA_CLCKEN();
        }else if (GPIO_port == GPIOB) {
                GPIOB_CLCKEN();
        }else if (GPIO_port == GPIOC) {
                GPIOC_CLCKEN();
        }else if (GPIO_port == GPIOD) {
                GPIOD_CLCKEN();
        }else if (GPIO_port == GPIOE) {
                GPIOE_CLCKEN();
        }else if (GPIO_port == GPIOF) {
                GPIOF_CLCKEN();
        }else if (GPIO_port == GPIOG) {
                GPIOG_CLCKEN();
        }else if (GPIO_port == GPIOH) {
                GPIOH_CLCKEN();
        }
    
    }else {
             if (GPIO_port == GPIOA) {
                GPIOA_CLCKDI();
        }else if (GPIO_port == GPIOB) {
                GPIOB_CLCKDI();
        }else if (GPIO_port == GPIOC) {
                GPIOC_CLCKDI();
        }else if (GPIO_port == GPIOD) {
                GPIOD_CLCKDI();
        }else if (GPIO_port == GPIOE) {
                GPIOE_CLCKDI();
        }else if (GPIO_port == GPIOF) {
                GPIOF_CLCKDI();
        }else if (GPIO_port == GPIOG) {
                GPIOG_CLCKDI();
        }else if (GPIO_port == GPIOH) {
                GPIOH_CLCKDI();
        }
    
    }
};                                                         
/**
 * @brief  This function initialises a GPIO port according to the configuration criteria you pass to it 
 *
 * @param  pGPIOx  a pointer to a gpio_handle_t struct. you must declare and initialise it in your code and then pass it's address to this function
 *
 * @return         none
 */ 
void GPIO_init(gpio_handle_t *pGPIOx)
{
     // if pin mode is less than or equal to analog  mode therefore none-interrupt mode
     // refer to @GPIO_PIN_MODES if unclear
     if (pGPIOx -> gpio_pinconfig -> gpio_pinmode <= GPIO_MODE_ANLG) {

    // mode
        uint32_t set_mask = pGPIOx -> gpio_pinconfig -> gpio_pinmode << (2 * pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
        uint32_t clear_mask = 0x3 << (2 * pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
        pGPIOx ->pGPIOx->MODER &= ~(clear_mask);
        pGPIOx ->pGPIOx->MODER |= set_mask;
        set_mask = 0;

    // speed
        set_mask = pGPIOx -> gpio_pinconfig ->gpio_pinspeed << (2 * pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
        clear_mask = 0x3 << (2 * pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
        pGPIOx ->pGPIOx->OSPEEDR &= ~(clear_mask);
        pGPIOx ->pGPIOx->OSPEEDR |= set_mask;
        set_mask = 0;
    //pupd
        set_mask = pGPIOx -> gpio_pinconfig ->gpio_pinpupdcontrol << (2 * pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
        clear_mask = 0x3 << (2 * pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
        pGPIOx ->pGPIOx->PUPDR &= ~(clear_mask);
        pGPIOx ->pGPIOx->PUPDR |= set_mask;
        set_mask = 0;
    //otype
        set_mask = pGPIOx -> gpio_pinconfig ->gpio_pinoptype <<  pGPIOx ->gpio_pinconfig ->gpio_pinnumber;
        clear_mask = 1 << pGPIOx ->gpio_pinconfig ->gpio_pinnumber;
        pGPIOx ->pGPIOx->OTYPER &= ~(clear_mask);
        pGPIOx ->pGPIOx->OTYPER |= set_mask;
        set_mask = 0;
    //altfunc
        if (pGPIOx -> gpio_pinconfig -> gpio_pinmode == GPIO_MODE_ALTFN) {

                // decide whether to update low or high register
                if (pGPIOx ->gpio_pinconfig ->gpio_pinnumber <= 7) {

                        set_mask = pGPIOx -> gpio_pinconfig -> gpio_pinaltfunmode << (4 * pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
                        clear_mask = 0xF << (4 * pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
                        pGPIOx ->pGPIOx->AFRL &= ~(clear_mask);
                        pGPIOx ->pGPIOx->AFRL |= set_mask;
                        set_mask = 0;
                }else {
                        uint8_t pin = pGPIOx ->gpio_pinconfig ->gpio_pinnumber;
                        uint8_t alt_fun = pGPIOx -> gpio_pinconfig -> gpio_pinaltfunmode;
                        uint32_t offset = (pin % 8) * 4;
                        // clear bits to make sure they are 0
                        pGPIOx->pGPIOx->AFRH &= ~(0xF << offset);
                        set_mask = ( alt_fun & 0xF) << offset;
                        pGPIOx ->pGPIOx->AFRH |= set_mask;
                        
                }
        
        }
     }else {

        if (pGPIOx ->gpio_pinconfig->gpio_pinmode == GPIO_MODE_IT_FE  ) {
                // clear RTST bit
                EXTI -> RTSR &= ~(1 << pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
                // configure FTSR
                EXTI ->FTSR |= (1 << pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
        
        }else if (pGPIOx ->gpio_pinconfig->gpio_pinmode == GPIO_MODE_IT_RE) {
                // clear FTSR bit
                EXTI -> FTSR &= ~(1 << pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
                // configure RTSR
                EXTI ->RTSR |= (1 << pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
        }else if (pGPIOx ->gpio_pinconfig->gpio_pinmode == GPIO_MODE_IT_RFE) {
                // configure both  RTSR and FTSR
                EXTI ->FTSR |= (1 << pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
                EXTI ->RTSR |= (1 << pGPIOx ->gpio_pinconfig ->gpio_pinnumber);
        
        }

        // select GPIO port in SYSCFG_EXTICR  
        uint8_t register_number = pGPIOx ->gpio_pinconfig ->gpio_pinnumber / 4;
        uint8_t offset = pGPIOx ->gpio_pinconfig ->gpio_pinnumber % 4;
        uint8_t port_code = GPIO_BASEADDR_TO_CODE(pGPIOx ->pGPIOx);
        SYSCFG_CLCKEN();
        // Clear the 4 bits first (0xF is 1111 in binary)
        SYSCFG->EXTICR[register_number] &= ~(0xF << (4 * offset)); 
        SYSCFG ->EXTICR[register_number] |= port_code << (4 * offset);
       
        // enable the exti interrupt delivery using IMR
        EXTI->IMR |= (1 << pGPIOx ->gpio_pinconfig ->gpio_pinnumber);

     } 
   
}; 
// a function to de-initialise gpio port
void GPIO_deinit(GPIO_REGDEF_t *GPIO_port)
{
        if (GPIO_port == GPIOA) {
                GPIOA_REG_RESET;
        }else if (GPIO_port == GPIOB) {
                GPIOB_REG_RESET;
        }else if (GPIO_port == GPIOC) {
                GPIOC_REG_RESET;
        }else if (GPIO_port == GPIOD) {
                GPIOD_REG_RESET;
        }else if (GPIO_port == GPIOE) {
                GPIOE_REG_RESET;
        }else if (GPIO_port == GPIOF) {
                GPIOF_REG_RESET;
        }else if (GPIO_port == GPIOG) {
                GPIOG_REG_RESET;
        }else if (GPIO_port == GPIOH) {
                GPIOH_REG_RESET;
        }
    
}; 
// a function to write to output pin
// since gpio pins are either 1 or 0
// state means set = 1 or reset = 0
void GPIO_pin_wt(GPIO_REGDEF_t *GPIO_port, uint8_t pin, uint8_t state)
{
    if (pin <= 15) {
        uint16_t mask = 1 << pin;

        if (state == 1) {
            GPIO_port->ODR |= mask;  
        } else {
            GPIO_port->ODR &= ~mask; 
        }
    }
}

// a function to write to output port
void GPIO_port_wt(GPIO_REGDEF_t *GPIO_port , uint16_t data)
{
 GPIO_port -> ODR = data;   
}; 
// a function to read from pin input
uint8_t GPIO_pin_rd(GPIO_REGDEF_t *GPIO_port , uint8_t pin)
{
 uint8_t register_contents =GPIO_port -> IDR;    
 uint8_t data = register_contents & (1 << pin);
 return data;  
}; 
// a function to read the entire port 
uint16_t GPIO_port_rd(GPIO_REGDEF_t *GPIO_port)
{
    uint16_t data = GPIO_port -> IDR;
    return data;
}; 
// function to toggle pin
void GPIO_pin_toggle(GPIO_REGDEF_t *GPIO_port, uint8_t pin)
{
    GPIO_port->ODR ^= (1 << pin);
}
 
/**
 * @brief  This function takes IRQ_number and state(0 or 1. Enable or disable ) and then enables or disables said IRQ number ,  based on state
 * @param  IRQ_number   GPIO port name (GPIOA...GPIOH) or a GPIO_REGDEF_t pointer to the GPIO port base address 
 * @param   state       0 or 1 indicating whether to enable or disable IRQ
 * @return  none
 */
void GPIO_IRQ_config(uint8_t IRQ_number , uint8_t state)
{
        if (state == 1) {
                if (IRQ_number <= 31) {
                        *NVIC_ISER0 |= (1 << IRQ_number);
                
                }else if (IRQ_number > 31 && IRQ_number < 64) {
                        uint32_t offset = (IRQ_number % 32);
                        *NVIC_ISER1 |= (1 << offset);

                
                }else if (IRQ_number >= 64 && IRQ_number < 96) {
                        uint32_t offset = (IRQ_number % 64);
                        *NVIC_ISER2 |= (1 << offset);
                
                }
        
        }else {
                if (IRQ_number <= 31) {
                        if (IRQ_number <= 31) {
                        *NVIC_ICER0 |= (1 << IRQ_number);
                
                }else if (IRQ_number > 31 && IRQ_number < 64) {
                        uint32_t offset = (IRQ_number % 32);
                        *NVIC_ICER1 |= (1 << offset);
                
                }else if (IRQ_number >= 64 && IRQ_number < 96) {
                        uint32_t offset = (IRQ_number % 64);
                        *NVIC_ICER2 |= (1 << offset);
                
                }
        
        }
    
}
}; 
// a call-back function that does something whenever an interrupt happens 
void GPIO_IRQ_handle(uint8_t pin)
{
    // clear the EXTI PR Register corresponding with the pin number
    if (EXTI -> PR & (1  << pin) ) {
        // clear pending register bit
        // by setting it to 1
        EXTI -> PR |= (1 << pin); 
    }

}; 

void GPIO_IRQ_priority_config(uint8_t IRQ_number , uint8_t priority)
{
        uint8_t iprx = IRQ_number / 4;
        uint8_t iprx_section = IRQ_number % 4;
        uint8_t shift_amount = (8 *  iprx_section) + (8 - NO_PRIORITY_BITS_IMPLEMENTED);
        *(NVIC_PR_BASE_ADDR + (iprx * 4)) |= (priority << shift_amount );
        
};

/**
 * @brief  This function takes a pointer to a GPIO port , and returns the bit mask required to activate said port in the SYSCFG_EXTICRx register
 *
 * @param  GPIO_port  GPIO port name (GPIOA...GPIOH) or a GPIO_REGDEF_t pointer to the GPIO port base address 
 *
 * @return  uint8_t       bit mask to activate the port in the SYSCFG_EXTICRx register
 */

uint8_t GPIO_BASEADDR_TO_CODE(GPIO_REGDEF_t * port)
{
        if (port == GPIOA) {
                return EXTI_GPIOA_PATTERN;
        
        }else if (port == GPIOB) {
                return EXTI_GPIOB_PATTERN;
        
        }else if (port == GPIOC) {
                return EXTI_GPIOC_PATTERN;
        
        }else if (port == GPIOD) {
                return EXTI_GPIOD_PATTERN;
        
        }else if (port == GPIOE) {
                return EXTI_GPIOE_PATTERN;
        
        }else if (port == GPIOF) {
                return EXTI_GPIOF_PATTERN;
        
        }else if (port == GPIOG) {
                return EXTI_GPIOG_PATTERN;
        
        }else if (port == GPIOH) {
                return EXTI_GPIOH_PATTERN;
        
        }else {
                return 0;
        }
        
};