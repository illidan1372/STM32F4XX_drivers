#include "arm_spi_driver.h"
#include "arm_stm32f446xx.h"
#include "arm_gpio_driver.h"
#include "arm_nucleof446re.h"
#include <stdint.h>
#include <stddef.h>

/**
 * @brief  Configure the board GPIO pins for SPI1, SPI2, or SPI3.
 *
 *         Pin selections come from arm_nucleof446re.h. GPIO clocks are enabled
 *         and pins are configured for their SPI alternate function. NSS is
 *         configured only when hardware slave select is requested.
 *
 * @param  pSPIx  SPI1, SPI2, or SPI3 (non-null); SPI4 pins are unsupported.
 * @param  ssm    SPI_SSM_HARDWARE or SPI_SSM_SOFTWARE.
 *
 * @return SPI_OK on success, SPI_ERROR_NULL_POINTER for a null port,
 *         SPI_ERROR_INVALID_PORT for an unsupported SPI port, or
 *         SPI_ERROR_INVALID_CONFIG for an invalid mode or GPIO setup failure.
 */
SPI_Status_t SPI_GPIO_pin_setup(SPI_REGDEF_t *pSPIx, uint8_t ssm)
{
    if (pSPIx == NULL) {
        return SPI_ERROR_NULL_POINTER;
    }
    if (pSPIx != SPI1 && pSPIx != SPI2 && pSPIx != SPI3) {
        return SPI_ERROR_INVALID_PORT;
    }
    if (ssm != SPI_SSM_HARDWARE && ssm != SPI_SSM_SOFTWARE) {
        return SPI_ERROR_INVALID_CONFIG;
    }

    gpio_pinconfig_t spi_pin_config = {0};
    gpio_handle_t spi_pin_gpio_handle = {0};

    spi_pin_config.gpio_pinmode = GPIO_MODE_ALTFN;
    spi_pin_config.gpio_pinpupdcontrol = GPIO_NO_PUPD;
    spi_pin_config.gpio_pinoptype = GPIO_OT_TYPE_PP;
    spi_pin_config.gpio_pinspeed = GPIO_SPEED_VERYHIGH;

    if (pSPIx == SPI1)
    {
        //enable GPIO clock 
        GPIO_clk_cfg(GPIOA, 1U);
        spi_pin_config.gpio_pinaltfunmode = SPI1_AF;

        if (ssm == SPI_SSM_HARDWARE)
        {
            spi_pin_config.gpio_pinnumber = SPI1_NSS_PIN;
            spi_pin_gpio_handle.pGPIOx = SPI1_NSS_PORT;
            spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
            if (GPIO_init(&spi_pin_gpio_handle) != GPIO_OK) return SPI_ERROR_INVALID_CONFIG;
        }

        spi_pin_config.gpio_pinnumber = SPI1_MISO_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI1_MISO_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        if (GPIO_init(&spi_pin_gpio_handle) != GPIO_OK) return SPI_ERROR_INVALID_CONFIG;

        spi_pin_config.gpio_pinnumber = SPI1_MOSI_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI1_MOSI_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        if (GPIO_init(&spi_pin_gpio_handle) != GPIO_OK) return SPI_ERROR_INVALID_CONFIG;

        spi_pin_config.gpio_pinnumber = SPI1_SCK_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI1_SCK_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        if (GPIO_init(&spi_pin_gpio_handle) != GPIO_OK) return SPI_ERROR_INVALID_CONFIG;
    }
    else if (pSPIx == SPI2)
    {
        GPIO_clk_cfg(GPIOB, 1U);
        spi_pin_config.gpio_pinaltfunmode = SPI2_AF;

        if (ssm == SPI_SSM_HARDWARE)
        {
            spi_pin_config.gpio_pinnumber = SPI2_NSS_PIN;
            spi_pin_gpio_handle.pGPIOx = SPI2_NSS_PORT;
            spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
            if (GPIO_init(&spi_pin_gpio_handle) != GPIO_OK) return SPI_ERROR_INVALID_CONFIG;
        }

        spi_pin_config.gpio_pinnumber = SPI2_MISO_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI2_MISO_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        if (GPIO_init(&spi_pin_gpio_handle) != GPIO_OK) return SPI_ERROR_INVALID_CONFIG;

        spi_pin_config.gpio_pinnumber = SPI2_MOSI_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI2_MOSI_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        if (GPIO_init(&spi_pin_gpio_handle) != GPIO_OK) return SPI_ERROR_INVALID_CONFIG;

        spi_pin_config.gpio_pinnumber = SPI2_SCK_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI2_SCK_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        if (GPIO_init(&spi_pin_gpio_handle) != GPIO_OK) return SPI_ERROR_INVALID_CONFIG;
    }
    else if (pSPIx == SPI3)
{
    GPIO_clk_cfg(GPIOB, 1U);

    spi_pin_config.gpio_pinaltfunmode = SPI3_AF;

    if (ssm == SPI_SSM_HARDWARE)
    {
        GPIO_clk_cfg(GPIOA, 1U);

        spi_pin_config.gpio_pinnumber = SPI3_NSS_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI3_NSS_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        if (GPIO_init(&spi_pin_gpio_handle) != GPIO_OK) return SPI_ERROR_INVALID_CONFIG;
    }

        spi_pin_config.gpio_pinnumber = SPI3_MISO_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI3_MISO_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        if (GPIO_init(&spi_pin_gpio_handle) != GPIO_OK) return SPI_ERROR_INVALID_CONFIG;

        spi_pin_config.gpio_pinnumber = SPI3_MOSI_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI3_MOSI_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        if (GPIO_init(&spi_pin_gpio_handle) != GPIO_OK) return SPI_ERROR_INVALID_CONFIG;

        spi_pin_config.gpio_pinnumber = SPI3_SCK_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI3_SCK_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        if (GPIO_init(&spi_pin_gpio_handle) != GPIO_OK) return SPI_ERROR_INVALID_CONFIG;
    }
    return SPI_OK;
}


/**
 * @brief  Enable or disable the peripheral clock for SPI1-SPI4.
 *
 * @param  pSPIx  SPI1, SPI2, SPI3, or SPI4 (non-null).
 * @param  state  1 to enable the clock; 0 to disable it.
 *
 * @return SPI_OK on success, SPI_ERROR_NULL_POINTER for a null port,
 *         SPI_ERROR_INVALID_PORT for an unsupported port, or
 *         SPI_ERROR_INVALID_CONFIG for a state other than 0 or 1.
 */
SPI_Status_t SPI_clk_cfg(SPI_REGDEF_t *pSPIx, uint8_t state)
{
    if (pSPIx == NULL) return SPI_ERROR_NULL_POINTER;
    if (pSPIx != SPI1 && pSPIx != SPI2 && pSPIx != SPI3 && pSPIx != SPI4) {
        return SPI_ERROR_INVALID_PORT;
    }
    if (state != 0U && state != 1U) return SPI_ERROR_INVALID_CONFIG;

    if (state == 1)
    {
        if (pSPIx == SPI1)
        {
            SPI1_CLCKEN();
        }
        else if (pSPIx == SPI2)
        {
            SPI2_CLCKEN();
        }
        else if (pSPIx == SPI3)
        {
            SPI3_CLCKEN();
        }
        else if (pSPIx == SPI4)
        {
            SPI4_CLCKEN();
        }
    }
    else
    {
        if (pSPIx == SPI1)
        {
            SPI1_CLCKDI();
        }
        else if (pSPIx == SPI2)
        {
            SPI2_CLCKDI();
        }
        else if (pSPIx == SPI3)
        {
            SPI3_CLCKDI();
        }
        else if (pSPIx == SPI4)
        {
            SPI4_CLCKDI();
        }
    }
    return SPI_OK;
}


/**
 * @brief  Configure SPI CR1 and the board GPIO pins from a handle.
 *
 *         Enables the SPI peripheral clock and configures GPIO pins, then writes
 *         CR1 from a zero-valued local configuration. SPI remains disabled
 *         (SPE = 0). If pin setup fails, the SPI clock may remain enabled.
 *
 * @param  pSPIx  Non-null handle with a non-null SPI port pointer. Board pin
 *                setup currently supports SPI1-SPI3.
 *
 * @return SPI_OK on success, SPI_ERROR_NULL_POINTER for a null handle or port,
 *         or the error status returned by SPI_clk_cfg or SPI_GPIO_pin_setup.
 */

 SPI_Status_t SPI_init(SPI_HANDLE_t *pSPIx)
{
    if (pSPIx == NULL || pSPIx->pSPIx == NULL) {
        return SPI_ERROR_NULL_POINTER;
    }
    SPI_REGDEF_t *spi_port = pSPIx->pSPIx;

    uint8_t device_mode        = pSPIx->SPI_config.SPI_device_mode;
    uint8_t bus_config         = pSPIx->SPI_config.SPI_bus_config;
    uint8_t clock_speed        = pSPIx->SPI_config.SPI_SCLK_speed;
    uint8_t data_frame_format  = pSPIx->SPI_config.SPI_dff;
    uint8_t clock_polarity     = pSPIx->SPI_config.SPI_CPOL;
    uint8_t clock_phase        = pSPIx->SPI_config.SPI_CPHA;
    uint8_t slave_select_mode  = pSPIx->SPI_config.SPI_ssm;

    /* Enable peripheral clock */
    SPI_Status_t status = SPI_clk_cfg(spi_port, 1U);
    if (status != SPI_OK) return status;

    
    /* Local copy of CR1 configuration */
    uint16_t cr1_register = 0;

    /* Configure bus mode */
    switch (bus_config)
    {
        case SPI_MODE_FULL_DUPLEX:
            break;

        case SPI_MODE_SIMPLEX_TX_ONLY:
            break;

        case SPI_MODE_HALF_DUPLEX:
            cr1_register |= (1U << SPI_CR1_BIDIMODE_OFFSET);
            break;

        case SPI_MODE_SIMPLEX_RX_ONLY:
            cr1_register |= (1U << SPI_CR1_RXONLY_OFFSET);   /* RXONLY */
            break;

        default:
            return SPI_ERROR_INVALID_CONFIG;
    }

    /* Remaining CR1 configuration goes here */

   /* Device mode: master or slave */
switch (device_mode)
{
    case SPI_DEVICE_MODE_MASTER:
        cr1_register |= (1U << SPI_CR1_MSTR_OFFSET);   /* MSTR = 1 */
        break;

    case SPI_DEVICE_MODE_SLAVE:
        /* MSTR = 0 */
        break;

    default:
        break;
}

/* Slave select management */
if (device_mode == SPI_DEVICE_MODE_MASTER)
{
    switch (slave_select_mode)
    {
        case SPI_SSM_HARDWARE:
            break;

        case SPI_SSM_SOFTWARE:
            cr1_register |= (1U << SPI_CR1_SSM_OFFSET);   
            cr1_register |= (1U << SPI_CR1_SSI_OFFSET);   
            break;

        default:
            break;
    }
}

  /* Data frame format selection*/

switch (data_frame_format) {
        case SPI_DFF_16_BIT:
           cr1_register |= (1U << 11); /* CR1 DFF: 1 selects 16-bit frames; 0 selects 8-bit frames. */
           break;
        case SPI_DFF_8_BIT:
           break;

        default:
           break;

}

 /* Clock speed setup*/
 /* BR[2:0]: Baud rate control
000: fPCLK/2
001: fPCLK/4
010: fPCLK/8
011: fPCLK/16
100: fPCLK/32
101: fPCLK/64
110: fPCLK/128
111: fPCLK/256
*/
switch (clock_speed)
{
    case SPI_SCLK_DIV2:
        cr1_register |= SPI_CR1_BR_DIV2;
        break;
    case SPI_SCLK_DIV4:
        cr1_register |= SPI_CR1_BR_DIV4;
        break;
    case SPI_SCLK_DIV8:
        cr1_register |= SPI_CR1_BR_DIV8;
        break;
    case SPI_SCLK_DIV16:
        cr1_register |= SPI_CR1_BR_DIV16;
        break;
    case SPI_SCLK_DIV32:
        cr1_register |= SPI_CR1_BR_DIV32;
        break;
    case SPI_SCLK_DIV64:
        cr1_register |= SPI_CR1_BR_DIV64;
        break;
    case SPI_SCLK_DIV128:
        cr1_register |= SPI_CR1_BR_DIV128;
        break;
    case SPI_SCLK_DIV256:
        cr1_register |= SPI_CR1_BR_DIV256;
        break;
    default:
        break;
}


/* clock phase setup*/

    switch (clock_phase)
    {
        case SPI_CPHA_FIRST_EDGE :
            break;

        case SPI_CPHA_SECOND_EDGE:
            cr1_register |= (1U << 0);   /* CPHA = 1 */
            break;

        default:
            break;
    }
   /*clock polarity setup*/

   switch (clock_polarity)
{
    case SPI_CPOL_LOW:
        /* CPOL = 0 */
        break;

    case SPI_CPOL_HIGH:
        cr1_register |= (1U << 1);   /* CPOL = 1 */
        break;

    default:
        break;
}

/* Connect board's pins to SPI pins of the MCU */
status = SPI_GPIO_pin_setup(spi_port, slave_select_mode);
if (status != SPI_OK) return status;

// write the register to the actual hardware
spi_port->CR1 = cr1_register;

return SPI_OK;
}