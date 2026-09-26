#ifndef INC_ARM_SPI_DRIVER_H_
#define INC_ARM_SPI_DRIVER_H_

#include "stdint.h"
#include "arm_stm32f446xx.h"
#include <stdint.h>

/* SPI driver operation result */
typedef enum
{
    SPI_OK = 0,
    SPI_ERROR_NULL_POINTER,
    SPI_ERROR_INVALID_PORT,
    SPI_ERROR_INVALID_CONFIG
} SPI_Status_t;


// this struct can be used to configure a SPI peripheral registers
typedef struct
{
    uint8_t SPI_device_mode;
    uint8_t SPI_bus_config;
    uint8_t SPI_SCLK_speed;
    uint8_t SPI_dff;
    uint8_t SPI_CPOL;
    uint8_t SPI_CPHA;
    uint8_t SPI_ssm;
    uint8_t SPI_ssoe;

}SPI_CONFIG_t ;

typedef struct 
{
    SPI_REGDEF_t *pSPIx;
    SPI_CONFIG_t  SPI_config;

}SPI_HANDLE_t ;

// a function to control the spi peripheral clock register
// state means enable clock or disable clock
SPI_Status_t SPI_clk_cfg(SPI_REGDEF_t *pSPIx , uint8_t state);                                                         
/**
 * @brief  Initialize an SPI peripheral from the supplied configuration.
 *
 *         Builds CR1 from the supplied configuration, configures hardware
 *         NSS output behavior through SSOE in CR2, enables the peripheral
 *         clock, and configures the required board GPIO pins.
 *
 *         The SPI peripheral remains disabled after initialization
 *         (SPE = 0).
 *
 * @param  pSPIx  Non-null SPI handle containing a non-null SPI peripheral
 *                pointer and a valid SPI configuration.
 *
 * @return SPI_OK on success, SPI_ERROR_NULL_POINTER for a null handle or
 *         peripheral pointer, SPI_ERROR_INVALID_CONFIG for an invalid or
 *         incompatible configuration, or an error returned by clock/GPIO
 *         configuration.
 */
SPI_Status_t SPI_init(SPI_HANDLE_t *pSPIx);

/**
 * @brief  Enable an initialized SPI peripheral.
 *
 *         Sets the SPE bit in CR1 without changing the peripheral
 *         configuration.
 *
 * @param  pSPIx  Non-null SPI handle containing SPI1, SPI2, SPI3, or SPI4.
 *
 * @return SPI_OK on success, SPI_ERROR_NULL_POINTER for a null handle or
 *         peripheral pointer, or SPI_ERROR_INVALID_PORT for an unsupported
 *         peripheral.
 */
SPI_Status_t SPI_enable(SPI_HANDLE_t *pSPIx);

/**
 * @brief  Disable an SPI peripheral.
 *
 *         Clears the SPE bit in CR1 without changing the peripheral
 *         configuration.
 *
 * @param  pSPIx  Non-null SPI handle containing SPI1, SPI2, SPI3, or SPI4.
 *
 * @return SPI_OK on success, SPI_ERROR_NULL_POINTER for a null handle or
 *         peripheral pointer, or SPI_ERROR_INVALID_PORT for an unsupported
 *         peripheral.
 */
SPI_Status_t SPI_disable(SPI_HANDLE_t *pSPIx);
// a function to de-initialise spi port
void SPI_deinit(SPI_HANDLE_t *pSPIx);

void SPI_send_data(SPI_REGDEF_t *pSPIx , uint8_t tx_buffer[] , uint32_t length);

void SPI_receive_data(SPI_REGDEF_t *pSPIx , uint8_t rx_buffer[] , uint32_t length);

void SPI_IRQ_config(uint8_t IRQ_number  , uint8_t state);
// a call-back function that does something whenever an interrupt happens 
void SPI_IRQ_handle(SPI_HANDLE_t *pSPI_handle);

void SPI_IRQ_priority_config(uint8_t IRQ_number , uint8_t priority);

// takes a SPIx(1,2,3 ,etc) and connects the physical pins on the board to MCU SPI ports
// is automatically called by SPI_init() function
// current pins are setup according to NUCLEO f446re board
// you can change the ports and pins in arm_nucleof446re.h file
SPI_Status_t SPI_GPIO_pin_setup(SPI_REGDEF_t *pSPIx , uint8_t ssm);

                                                                    /* SPI specific macros*/


                                                                    // spi init macros

                                                                  

  // @spi_modes

#define SPI_MODE_FULL_DUPLEX           0
#define SPI_MODE_HALF_DUPLEX           1
#define SPI_MODE_SIMPLEX_RX_ONLY       2
#define SPI_MODE_SIMPLEX_TX_ONLY       3

  // @spi_dataframe_formats

#define SPI_DFF_8_BIT         0
#define SPI_DFF_16_BIT        1

  // @spi_slave_select_modes

#define SPI_SSM_SOFTWARE      0
#define SPI_SSM_HARDWARE      1

// @spi_nss_direction

#define SPI_NSS_INPUT          0
#define SPI_NSS_OUTPUT         1

 // @spi_device_mode

#define SPI_DEVICE_MODE_SLAVE   0
#define SPI_DEVICE_MODE_MASTER  1

// @spi clock_speed

// OPTIONS FOR SPI PERIPHERAL CLOCK SPEED ARE PERIPHERAL CLOCK DIVIDED BY 2,4,8,16 ETC AS PER THE USER MANUAL
#define SPI_SCLK_DIV2           0
#define SPI_SCLK_DIV4           1
#define SPI_SCLK_DIV8           2
#define SPI_SCLK_DIV16          3
#define SPI_SCLK_DIV32          4
#define SPI_SCLK_DIV64          5
#define SPI_SCLK_DIV128         6
#define SPI_SCLK_DIV256         7

// @spi_clock_phase / CPHA
#define SPI_CPHA_FIRST_EDGE     0
#define SPI_CPHA_SECOND_EDGE    1

// @spi_clock_polarity / CPOL
#define SPI_CPOL_LOW      0
#define SPI_CPOL_HIGH     1



#endif