# STM32F446RE Bare-Metal Drivers

A bare-metal driver library for the STM32F446RE microcontroller written in C without STM32 HAL.

This project is being developed to gain a deep understanding of STM32 peripherals, ARM Cortex-M microcontrollers, and embedded driver development by working directly with hardware registers and the STM32 reference manual.

## Current Status

### Completed

* Memory map definitions
* Peripheral base address definitions
* RCC register definitions
* GPIO driver

  * GPIO initialization
  * GPIO clock control
  * GPIO mode configuration
  * GPIO output type configuration
  * GPIO speed configuration
  * GPIO pull-up/pull-down configuration
  * GPIO alternate function configuration
  * GPIO read/write APIs

### In Progress

* SPI driver

  * Peripheral register mapping
  * Clock control
  * SPI initialization
  * Master/slave configuration
  * Bus mode configuration
  * Clock prescaler configuration
  * CPOL/CPHA configuration
  * Data frame format configuration
  * Software slave management

### Planned

* SPI transmit/receive APIs
* SPI interrupt mode
* SPI DMA support
* I2C driver
* USART driver
* Timer driver
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

## Disclaimer

This project is under active development and APIs may change as the drivers evolve.
