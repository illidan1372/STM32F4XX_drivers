#ifndef INC_ARM_STM32F446XX_H_
#define INC_ARM_STM32F446XX_H_

#include <stdint.h>

                                                   /* Processor specific macros */


//ARM cortex Mx processor  NVIC ISERx register addresses
#define NVIC_ISER0   ((volatile uint32_t *)0xE000E100)
#define NVIC_ISER1   ((volatile uint32_t *)0xE000E104)
#define NVIC_ISER2   ((volatile uint32_t *)0xE000E108)
#define NVIC_ISER3   ((volatile uint32_t *)0xE000E10c)
#define NVIC_ISER4   ((volatile uint32_t *)0xE000E110)
#define NVIC_ISER5   ((volatile uint32_t *)0xE000E114)
#define NVIC_ISER6   ((volatile uint32_t *)0xE000E118)
#define NVIC_ISER7   ((volatile uint32_t *)0xE000E11C)

//ARM cortex Mx processor  NVIC ICERx register addresses
#define NVIC_ICER0   ((volatile uint32_t *)0XE000E180)
#define NVIC_ICER1   ((volatile uint32_t *)0xE000E184)
#define NVIC_ICER2   ((volatile uint32_t *)0xE000E188)
#define NVIC_ICER3   ((volatile uint32_t *)0xE000E18C)


#define NVIC_PR_BASE_ADDR ((volatile uint32_t *)0xE000E400)

// Memory base Addresses

#define flash_base_addr 0x08000000U
#define sram1_base_addr 0x20000000U
#define sram2_base_addr 0x2001C000U
#define rom_base_addr   0x1FFF0000U  
#define sram_base_addr  sram1_base_addr


// AHBx and APBx bus peripheral base addresses

#define PERIPH_BASE_ADDR   0x40000000U 
#define APB1_BASE_ADDR     PERIPH_BASE_ADDR
#define APB2_BASE_ADDR     0x40010000U
#define AHB1_BASE_ADDR     0x40020000U
#define AHB2_BASE_ADDR     0x50000000U

// GPIOx base addresses
#define GPIO_BASE_ADDR     AHB1_BASE_ADDR 
#define GPIOA_BASE_ADDR    ( GPIO_BASE_ADDR + 0x0000 )
#define GPIOB_BASE_ADDR    ( GPIO_BASE_ADDR + 0x0400 )
#define GPIOC_BASE_ADDR    ( GPIO_BASE_ADDR + 0x0800 )
#define GPIOD_BASE_ADDR    ( GPIO_BASE_ADDR + 0x0C00 )
#define GPIOE_BASE_ADDR    ( GPIO_BASE_ADDR + 0x1000 )
#define GPIOF_BASE_ADDR    ( GPIO_BASE_ADDR + 0x1400 )
#define GPIOG_BASE_ADDR    ( GPIO_BASE_ADDR + 0x1800 )
#define GPIOH_BASE_ADDR    ( GPIO_BASE_ADDR + 0x1C00 )

//I2Cx base addresses
#define I2C1_BASE_ADDR     ( APB1_BASE_ADDR  + 0x5400 )
#define I2C2_BASE_ADDR     ( APB1_BASE_ADDR  + 0x5800 )
#define I2C3_BASE_ADDR     ( APB1_BASE_ADDR  + 0x5C00 )

// SPIx base addresses
#define SPI1_BASE_ADDR     ( APB2_BASE_ADDR  + 0x3000)
#define SPI2_BASE_ADDR     ( APB1_BASE_ADDR  + 0x3800)
#define SPI3_BASE_ADDR     ( APB1_BASE_ADDR  + 0x3C00)
#define SPI4_BASE_ADDR     ( APB2_BASE_ADDR  + 0x3400)

//USARTx base addresses
#define USART2_BASE_ADDR     ( APB1_BASE_ADDR  + 0x4400)
#define USART3_BASE_ADDR     ( APB1_BASE_ADDR  + 0x4800)

//UARTx base addresses
#define UART4_BASE_ADDR     ( APB1_BASE_ADDR  + 0x4C00 )
#define UART5_BASE_ADDR     ( APB1_BASE_ADDR  + 0x5000 )

//RCC base address macro
#define RCC_BASE_ADDR ( AHB1_BASE_ADDR + 0X3800 )

// EXTI base address
#define EXTI_BASE_ADDR ( APB2_BASE_ADDR + 0X3C00 )
// SYSCFG base address
#define SYSCFG_BASE_ADDR ( APB2_BASE_ADDR + 0X3800 )

#define NO_PRIORITY_BITS_IMPLEMENTED 4
 
/*  peripheral register definition structures         */

/* this structure , represents the various registers 
 of the GPIOx peripherals of the microcontroller
 in  order to use it, you need to create a pointer to the 
 base address of the GPIO port you need , for example GPIOA,B etc. 
 of type  GPIO_REGDEF_t. for example GPIO_REGDEF_t * GPIOA = (GPIO_REGDEF_t *)GPIOA_BASE_ADDR 
 you can then access each register by doing something like this GPIOA -> ODR
 this would then access the ODR register of GPIOA
 */

 typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;

}GPIO_REGDEF_t ;

/* this is a struct that represents the RCC peripheral registers
of the microcontroller
*/
typedef struct
{
    volatile uint32_t RCC_CR;
    volatile uint32_t RCC_PLL_CFGR;
    volatile uint32_t RCC_CFGR;
    volatile uint32_t RCC_CIR;
    volatile uint32_t RCC_AHB1_RSTR;
    volatile uint32_t RCC_AHB2_RSTR;
    volatile uint32_t RCC_AHB3_RSTR;
    volatile uint32_t RESERVED1;
    volatile uint32_t RCC_APB1_RSTR;
    volatile uint32_t RCC_APB2_RSTR;
    volatile uint32_t RESERVED2;
    volatile uint32_t RESERVED3;
    volatile uint32_t RCC_AHB1_ENR;
    volatile uint32_t RCC_AHB2_ENR;
    volatile uint32_t RCC_AHB3_ENR;
    volatile uint32_t RESERVED4;
    volatile uint32_t RCC_APB1_ENR;
    volatile uint32_t RCC_APB2_ENR;
    volatile uint32_t RESERVED5;
    volatile uint32_t RESERVED6;
    volatile uint32_t RCC_AHB1_LPENR;
    volatile uint32_t RCC_AHB2_LPENR;
    volatile uint32_t RCC_AHB3_LPENR;
    volatile uint32_t RESERVED7;
    volatile uint32_t RCC_APB1_LPENR;
    volatile uint32_t RCC_APB2_LPENR;
    volatile uint32_t RESERVED8;
    volatile uint32_t RESERVED9;
    volatile uint32_t RCC_BDCR;
    volatile uint32_t RCC_CSR;
    volatile uint32_t RESERVED10;
    volatile uint32_t RESERVED11;
    volatile uint32_t RCC_SS_CGR;
    volatile uint32_t RCC_PLLI2_SCFGR;
    volatile uint32_t RCC_PLL_SAI_CFGR;
    volatile uint32_t RCC_DCK_CFGR;
    volatile uint32_t RCC_CK_GATENR;
    volatile uint32_t RCC_DCK_CFGR2;
}RCC_REGDEF_t;


/* this is a struct that represents the EXTI peripheral registers
of the microcontroller
*/
typedef struct
{
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
}EXTI_REGDEF_t;

typedef struct
{
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR[4];
    volatile uint32_t RESERVED1;
    volatile uint32_t RESERVED2;
    volatile uint32_t CMPCR;
    volatile uint32_t RESERVED3;
    volatile uint32_t RESERVED4;
    volatile uint32_t CFGR;
}SYSCFG_REGDEF_t;


/* this is a struct that represents the SPI peripheral registers
of the microcontroller
*/
typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;
}SPI_REGDEF_t;


/*  peripheral definitions 
    these are macros that you can use in order to
    access various peripheral ports without manually declaring
    a pointer and typecasting it 
*/

#define GPIOA ((GPIO_REGDEF_t *)GPIOA_BASE_ADDR)
#define GPIOB ((GPIO_REGDEF_t *)GPIOB_BASE_ADDR)
#define GPIOC ((GPIO_REGDEF_t *)GPIOC_BASE_ADDR)
#define GPIOD ((GPIO_REGDEF_t *)GPIOD_BASE_ADDR)
#define GPIOE ((GPIO_REGDEF_t *)GPIOE_BASE_ADDR)
#define GPIOF ((GPIO_REGDEF_t *)GPIOF_BASE_ADDR)
#define GPIOG ((GPIO_REGDEF_t *)GPIOG_BASE_ADDR)
#define GPIOH ((GPIO_REGDEF_t *)GPIOH_BASE_ADDR)
#define RCC   ((RCC_REGDEF_t  *)RCC_BASE_ADDR)
#define EXTI  ((EXTI_REGDEF_t *)EXTI_BASE_ADDR)
#define SYSCFG ((SYSCFG_REGDEF_t *)SYSCFG_BASE_ADDR)
#define SPI1 ((SPI_REGDEF_t *) SPI1_BASE_ADDR)
#define SPI2 ((SPI_REGDEF_t *) SPI2_BASE_ADDR)
#define SPI3 ((SPI_REGDEF_t *) SPI3_BASE_ADDR)
#define SPI4 ((SPI_REGDEF_t *) SPI4_BASE_ADDR)

/* GPIO clock enable macros
   since enabling the clock for various
   gpio ports , available in the mcu , 
   is a frequently needed action ;
   these macros enable the programmer
   to enable the clock in a single line 
   of code 
*/
#define GPIOA_CLCKEN() ( RCC -> RCC_AHB1_ENR |= (1U << 0 ))
#define GPIOB_CLCKEN() ( RCC -> RCC_AHB1_ENR |= (1U << 1 ))
#define GPIOC_CLCKEN() ( RCC -> RCC_AHB1_ENR |= (1U << 2 ))
#define GPIOD_CLCKEN() ( RCC -> RCC_AHB1_ENR |= (1U << 3 ))
#define GPIOE_CLCKEN() ( RCC -> RCC_AHB1_ENR |= (1U << 4 ))
#define GPIOF_CLCKEN() ( RCC -> RCC_AHB1_ENR |= (1U << 5 ))
#define GPIOG_CLCKEN() ( RCC -> RCC_AHB1_ENR |= (1U << 6 ))
#define GPIOH_CLCKEN() ( RCC -> RCC_AHB1_ENR |= (1U << 7 ))
/* I2C clock enable macros
   since enabling the clock for various
   I2C ports , available in the mcu , 
   is a frequently needed action ;
   these macros enable the programmer
   to enable the clock in a single line 
   of code 
*/
#define I2C1_CLCKEN()  ( RCC -> RCC_APB1_ENR |= (1U << 21 ))
#define I2C2_CLCKEN()  ( RCC -> RCC_APB1_ENR |= (1U << 22 ))
#define I2C3_CLCKEN()  ( RCC -> RCC_APB1_ENR |= (1U << 23 ))
/* SPI clock enable macros
   since enabling the clock for various
   SPI ports , available in the mcu , 
   is a frequently needed action ;
   these macros enable the programmer
   to enable the clock in a single line 
   of code 
*/
#define SPI1_CLCKEN()  ( RCC -> RCC_APB2_ENR |= (1U << 12 ))
#define SPI2_CLCKEN()  ( RCC -> RCC_APB1_ENR |= (1U << 14 ))
#define SPI3_CLCKEN()  ( RCC -> RCC_APB1_ENR |= (1U << 15 ))
#define SPI4_CLCKEN()  ( RCC -> RCC_APB2_ENR |= (1U << 13 ))

/* UART clock enable macros
   since enabling the clock for various
   UART ports , available in the mcu , 
   is a frequently needed action ;
   these macros enable the programmer
   to enable the clock in a single line 
   of code 
*/
#define UART4_CLCKEN()  ( RCC -> RCC_APB1_ENR |= (1U << 19 ))
#define UART5_CLCKEN()  ( RCC -> RCC_APB1_ENR |= (1U << 20 ))
/* USART clock enable macros
   since enabling the clock for various
   USART ports , available in the mcu , 
   is a frequently needed action ;
   these macros enable the programmer
   to enable the clock in a single line 
   of code 
*/
#define USART1_CLCKEN()  ( RCC -> RCC_APB2_ENR |= (1U << 4))
#define USART2_CLCKEN()  ( RCC -> RCC_APB1_ENR |= (1U << 17 ))
#define USART3_CLCKEN()  ( RCC -> RCC_APB1_ENR |= (1U << 18 ))
#define USART6_CLCKEN()  ( RCC -> RCC_APB2_ENR |= (1U << 5))

#define SYSCFG_CLCKEN()  ( RCC -> RCC_APB2_ENR |= (1U << 14))

                        /* Disable macroc. these macros help you disable peripherals in a single line of code*/

    //GPIO disable
#define GPIOA_CLCKDI()  ( RCC -> RCC_AHB1_ENR &= ~(1U << 0 ))
#define GPIOB_CLCKDI()  ( RCC -> RCC_AHB1_ENR &= ~(1U << 1 ))
#define GPIOC_CLCKDI()  ( RCC -> RCC_AHB1_ENR &= ~(1U << 2 ))
#define GPIOD_CLCKDI()  ( RCC -> RCC_AHB1_ENR &= ~(1U<< 3 ))
#define GPIOE_CLCKDI()  ( RCC -> RCC_AHB1_ENR &= ~(1U << 4 ))
#define GPIOF_CLCKDI()  ( RCC -> RCC_AHB1_ENR &= ~(1U << 5 ))
#define GPIOG_CLCKDI()  ( RCC -> RCC_AHB1_ENR &= ~(1U << 6 ))
#define GPIOH_CLCKDI()  ( RCC -> RCC_AHB1_ENR &= ~(1U<< 7 ))

    //SPI disable
#define SPI1_CLCKDI()  ( RCC -> RCC_APB2_ENR &= ~(1U << 12 ))
#define SPI2_CLCKDI()  ( RCC -> RCC_APB1_ENR &= ~(1U << 14 ))
#define SPI3_CLCKDI()  ( RCC -> RCC_APB1_ENR &= ~(1U << 15 ))
#define SPI4_CLCKDI()  ( RCC -> RCC_APB2_ENR &= ~(1U << 13 ))
    //I2C disable
#define I2C1_CLCKDI()  ( RCC -> RCC_APB1_ENR &= ~(1U << 21 ))
#define I2C2_CLCKDI()  ( RCC -> RCC_APB1_ENR &= ~(1U << 22 ))
#define I2C3_CLCKDI()  ( RCC -> RCC_APB1_ENR &= ~(1U << 23 ))
    //USART disable
#define USART1_CLCKDI()  ( RCC -> RCC_APB2_ENR &= ~(1U << 4))
#define USART2_CLCKDI()  ( RCC -> RCC_APB1_ENR &= ~(1U << 17 ))
#define USART3_CLCKDI()  ( RCC -> RCC_APB1_ENR &= ~(1U << 18 ))
#define USART6_CLCKDI()  ( RCC -> RCC_APB2_ENR &= ~(1U << 5))
    //UART disable
#define UART4_CLCKDI()  ( RCC -> RCC_APB1_ENR &= ~(1U << 19 ))
#define UART5_CLCKDI()  ( RCC -> RCC_APB1_ENR &= ~(1U << 20 ))
 
    // THIS MACRO MAPS PORT BASE ADDESS TO BIT PATTERNS
    // THAT GIVES OWNERSHIP OF EXTI LINES TO SPECIFIC PORTS
    // you can find the bit patterns and their explaination 
    // in the SYSCFG section of the reference manual
uint8_t GPIO_BASEADDR_TO_CODE(GPIO_REGDEF_t * port);

#endif