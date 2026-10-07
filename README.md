# STM32F446RE Bare-Metal Drivers

A bare-metal driver library for the STM32F446RE microcontroller written in C without STM32 HAL.

This project is being developed to gain a deep understanding of STM32 peripherals, ARM Cortex-M microcontrollers, and embedded driver development by working directly with hardware registers and the STM32 reference manual.

## Current Status

### Completed

* Memory map definitions
* Peripheral base address definitions
* RCC register mapping and peripheral clock-gating macros

* GPIO driver

  * GPIO initialization
  * GPIO clock control
  * GPIO mode configuration
  * GPIO output type configuration
  * GPIO speed configuration
  * GPIO pull-up/pull-down configuration
  * GPIO alternate function configuration
  * GPIO read/write APIs
  * GPIO status/error handling

* SPI blocking driver

  * Peripheral register mapping
  * Peripheral clock control
  * SPI initialization
  * SPI enable/disable
  * Master/slave configuration
  * Full-duplex configuration
  * Half-duplex configuration and direction control
  * Simplex TX/RX configuration
  * Clock prescaler configuration
  * CPOL/CPHA configuration
  * 8-bit data frame support
  * Software and hardware slave-select management
  * Hardware NSS output control
  * Blocking transmit API
  * Blocking receive API
  * Blocking full-duplex exchange API
  * SPI status/error handling
  * SPI1 full-duplex hardware loopback test

### In Progress / Next

* RCC clock-tree driver

  * HSI clock source configuration
  * HSE clock source configuration
  * System clock source selection
  * Hardware validation on Nucleo-F446RE:
    * HSI -> HSE -> HSI SYSCLK switching completed successfully
    * Final observed state after switching back to HSI and disabling HSE:
      * SW = 00
      * SWS = 00
      * HSEON = 0
      * HSERDY = 0
  * PLL configuration
  * SYSCLK frequency configuration
  * AHB clock configuration
  * APB1 clock configuration
  * APB2 clock configuration
  * Requested-frequency validation
  * Invalid-configuration handling without modifying the active clock tree
  * Clock-frequency query APIs

### Planned

* SPI interrupt mode
* SPI DMA support
* USART/UART driver
* I2C driver
* Timer/PWM driver
* ADC driver
* DMA driver
* CAN driver
* Example applications

## Target Hardware

* STM32F446RE
* ARM Cortex-M4
* Nucleo-F446RE (or compatible STM32F446 board)

## Design Goals

* No STM32 HAL
* Minimal abstraction
* Direct register access
* Educational and easy to understand
* Portable driver architecture

## Build Environment
* PlatformIO
* GCC ARM Embedded Toolchain
* OpenOCD
* GDB

## Project Structure

```text
.
├── include/              # Header files
│   ├── arm_gpio_driver.h
│   ├── arm_spi_driver.h
│   ├── arm_stm32f446xx.h
│   └── arm_nucleof446re.h
├── src/                  # Source files
│   ├── arm_gpio_driver.c
│   ├── arm_spi_driver.c
│   ├── arm_nucleof446re.c
│   └── main.c
├── lib/
├── test/
├── platformio.ini
└── README.md
```

## Disclaimer

This project is under active development and APIs may change as the drivers evolve.
