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

## PLL and System Clock Setup

`RCC_PLL_configure()` validates the PLL factors and VCO/PLLP frequencies,
then selects regulator voltage scale 1 while the main PLL is disabled.
Both the active and requested system clock must be HSI or HSE. It does not
enable the PLL or change SYSCLK. HSI input frequency must be 16 MHz.

`RCC_SYSCLK_select(source, hse_hz, supply_mv)` takes the actual external clock
frequency in Hz and the guaranteed supply voltage in millivolts (1800–3600).
`hse_hz` is ignored when using HSI directly or as the PLL input. Use the lowest
guaranteed voltage over the operating conditions; this function does not measure
VDD. At voltage-table boundaries the lower-voltage column is used conservatively.
The internal regulator must be enabled, not bypassed.

The clock switch derives the PLL frequency from its registers, requires its
oscillator and PLL to be ready, and waits for voltage scale 1 readiness. Above
168 MHz it enables Over-drive and waits for both ready flags; this requires
VDD above 2.1 V and HSI/HSE as the current SYSCLK when entering Over-drive.
Call during initialization, with peripheral consumers stopped and their clocks
disabled before an Over-drive transition, as required by RM0390.

Flash wait states are raised and read back before increasing HCLK and lowered
only after the new clock and AHB divider take effect. Prefetch is disabled at
VDD <= 2.1 V. Every successful switch explicitly selects AHB `/1`, APB1 `/4`,
and APB2 `/2`, keeping the buses within their 180/45/90 MHz limits. These fixed
dividers also apply to HSI/HSE switches; reconfigure peripheral timings and
software timebases accordingly. This driver does not update CMSIS
`SystemCoreClock` or SysTick. Voltage scale and Over-drive are retained when
switching down to a slower clock.

For example, with an already enabled 8 MHz HSE and a guaranteed 3.3 V supply,
check each return status in this sequence:

```c
status = RCC_PLL_source_select(RCC_PLL_SOURCE_HSE);
status = RCC_PLL_configure(8000000U, 4U, 180U,
                           RCC_PLL_P_DIV2, RCC_PLL_Q_DIV8, RCC_PLL_R_DIV2);
status = RCC_PLL_enable();
status = RCC_SYSCLK_select(RCC_SYSCLK_PLL_P, 8000000U, 3300U);
```

This produces HCLK = 180 MHz, APB1 = 45 MHz, APB2 = 90 MHz, and 5 Flash wait
states. PLLQ is 45 MHz in this example and is not suitable for a 48 MHz USB clock.
Invalid arguments are rejected before changing clock settings. A readiness
timeout can leave power settings, increased Flash latency, or conservative bus
dividers applied. A SYSCLK-switch timeout leaves the request pending: inspect
SW/SWS before retrying or disabling a clock. Timeouts currently count iterations,
not elapsed time. Hardware validation of the high-speed sequence is still needed.

Reference: [STM32F446 RM0390](https://www.st.com/resource/en/reference_manual/dm00135183-stm32f446xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf),
sections on Flash latency, voltage scaling, Over-drive, and RCC prescalers.

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
