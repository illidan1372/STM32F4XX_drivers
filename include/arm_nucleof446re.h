#ifndef ARM_NUCLEOF446RE_H_
#define ARM_NUCLEOF446RE_H_

#include <stdint.h>
#include "arm_stm32f446xx.h"
#include "arm_gpio_driver.h"

                                            /* board specific peripheral pins*/



/* SPI1 - AF5 */
#define SPI1_NSS_PORT      GPIOA
#define SPI1_NSS_PIN       PIN_NO_4

#define SPI1_SCK_PORT      GPIOA
#define SPI1_SCK_PIN       PIN_NO_5

#define SPI1_MISO_PORT     GPIOA
#define SPI1_MISO_PIN      PIN_NO_6

#define SPI1_MOSI_PORT     GPIOA
#define SPI1_MOSI_PIN      PIN_NO_7

#define SPI1_AF            5U


/* SPI2 - AF5 */
#define SPI2_NSS_PORT      GPIOB
#define SPI2_NSS_PIN       PIN_NO_12

#define SPI2_SCK_PORT      GPIOB
#define SPI2_SCK_PIN       PIN_NO_13

#define SPI2_MISO_PORT     GPIOB
#define SPI2_MISO_PIN      PIN_NO_14

#define SPI2_MOSI_PORT     GPIOB
#define SPI2_MOSI_PIN      PIN_NO_15

#define SPI2_AF            5U


/* SPI3 - AF6 */
#define SPI3_NSS_PORT      GPIOA
#define SPI3_NSS_PIN       PIN_NO_4

#define SPI3_SCK_PORT      GPIOB
#define SPI3_SCK_PIN       PIN_NO_3

#define SPI3_MISO_PORT     GPIOB
#define SPI3_MISO_PIN      PIN_NO_4

#define SPI3_MOSI_PORT     GPIOB
#define SPI3_MOSI_PIN      PIN_NO_5

#define SPI3_AF            6U


void user_LED_set(uint8_t state);
void user_LED_toggle(void);

#endif