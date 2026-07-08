#include "arm_spi_driver.h"
#include "arm_stm32f446xx.h"
#include "arm_gpio_driver.h"
#include "arm_nucleof446re.h"
#include <stdint.h>
/**
 * @brief  This function takes a pointer to SPI register and connects the physical pins
 *         on your board to the internal MCU SPI peripheral.
 *
 *         Default board pins for SPI1, SPI2, and SPI3 are selected according to the
 *         STM32 Nucleo-F446RE board configuration.
 *
 *         You can change them in arm_nucleof446re.h to match your own board.
 *
 * @param  pSPIx  SPI port, for example SPI1, SPI2, SPI3
 * @param  ssm    Slave select management mode
 *
 * @return none
 */
void SPI_GPIO_pin_setup(SPI_REGDEF_t *pSPIx, uint8_t ssm)
{
    gpio_pinconfig_t spi_pin_config = {0};
    gpio_handle_t spi_pin_gpio_handle = {0};

    spi_pin_config.gpio_pinmode = GPIO_MODE_ALTFN;
    spi_pin_config.gpio_pinpupdcontrol = GPIO_NO_PUPD;
    spi_pin_config.gpio_pinoptype = GPIO_OT_TYPE_PP;
    spi_pin_config.gpio_pinspeed = GPIO_SPEED_VERYHIGH;

    if (pSPIx == SPI1)
    {
        spi_pin_config.gpio_pinaltfunmode = SPI1_AF;

        if (ssm == SPI_SSM_HARDWARE)
        {
            spi_pin_config.gpio_pinnumber = SPI1_NSS_PIN;
            spi_pin_gpio_handle.pGPIOx = SPI1_NSS_PORT;
            spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
            GPIO_init(&spi_pin_gpio_handle);
        }

        spi_pin_config.gpio_pinnumber = SPI1_MISO_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI1_MISO_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        GPIO_init(&spi_pin_gpio_handle);

        spi_pin_config.gpio_pinnumber = SPI1_MOSI_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI1_MOSI_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        GPIO_init(&spi_pin_gpio_handle);

        spi_pin_config.gpio_pinnumber = SPI1_SCK_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI1_SCK_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        GPIO_init(&spi_pin_gpio_handle);
    }
    else if (pSPIx == SPI2)
    {
        spi_pin_config.gpio_pinaltfunmode = SPI2_AF;

        if (ssm == SPI_SSM_HARDWARE)
        {
            spi_pin_config.gpio_pinnumber = SPI2_NSS_PIN;
            spi_pin_gpio_handle.pGPIOx = SPI2_NSS_PORT;
            spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
            GPIO_init(&spi_pin_gpio_handle);
        }

        spi_pin_config.gpio_pinnumber = SPI2_MISO_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI2_MISO_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        GPIO_init(&spi_pin_gpio_handle);

        spi_pin_config.gpio_pinnumber = SPI2_MOSI_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI2_MOSI_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        GPIO_init(&spi_pin_gpio_handle);

        spi_pin_config.gpio_pinnumber = SPI2_SCK_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI2_SCK_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        GPIO_init(&spi_pin_gpio_handle);
    }
    else if (pSPIx == SPI3)
    {
        spi_pin_config.gpio_pinaltfunmode = SPI3_AF;

        if (ssm == SPI_SSM_HARDWARE)
        {
            spi_pin_config.gpio_pinnumber = SPI3_NSS_PIN;
            spi_pin_gpio_handle.pGPIOx = SPI3_NSS_PORT;
            spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
            GPIO_init(&spi_pin_gpio_handle);
        }

        spi_pin_config.gpio_pinnumber = SPI3_MISO_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI3_MISO_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        GPIO_init(&spi_pin_gpio_handle);

        spi_pin_config.gpio_pinnumber = SPI3_MOSI_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI3_MOSI_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        GPIO_init(&spi_pin_gpio_handle);

        spi_pin_config.gpio_pinnumber = SPI3_SCK_PIN;
        spi_pin_gpio_handle.pGPIOx = SPI3_SCK_PORT;
        spi_pin_gpio_handle.gpio_pinconfig = &spi_pin_config;
        GPIO_init(&spi_pin_gpio_handle);
    }
}


/**
 * @brief  This function takes a SPI port name as well as state (0 or 1) and sets up the clock for that particular SPI port 
 *
 * @param  pSPIx  pointer to SPI peripheral register structure
 * @param  state  0 or 1 , to turn the clock on or off
 *
 * @return        none
 */
void SPI_clk_cfg(SPI_REGDEF_t *pSPIx, uint8_t state)
{
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
}


/**
 * @brief  This function takes a pointer to SPI_HANDLE_t  and sets up a SPI port based on the options in that handle struct
 *
 * @param  pSPIx  pointer to SPI peripheral handle structure
 *
 * @return        none
 */

 void SPI_init(SPI_HANDLE_t *pSPIx)
{
    SPI_REGDEF_t *spi_port = pSPIx->pSPIx;

    uint8_t device_mode        = pSPIx->SPI_config.SPI_device_mode;
    uint8_t bus_config         = pSPIx->SPI_config.SPI_bus_config;
    uint8_t clock_speed        = pSPIx->SPI_config.SPI_SCLK_speed;
    uint8_t data_frame_format  = pSPIx->SPI_config.SPI_dff;
    uint8_t clock_polarity     = pSPIx->SPI_config.SPI_CPOL;
    uint8_t clock_phase        = pSPIx->SPI_config.SPI_CPHA;
    uint8_t slave_select_mode  = pSPIx->SPI_config.SPI_ssm;

    /* Enable peripheral clock */
    SPI_clk_cfg(spi_port, 1);

    
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
            cr1_register |= (1U << 15);   /* BIDIMODE */
            break;

        case SPI_MODE_SIMPLEX_RX_ONLY:
            cr1_register |= (1U << 10);   /* RXONLY */
            break;

        default:
            /* Invalid configuration */
            break;
    }

    /* Remaining CR1 configuration goes here */

   /* Device mode: master or slave */
switch (device_mode)
{
    case SPI_DEVICE_MODE_MASTER:
        cr1_register |= (1U << 2);   /* MSTR = 1 */
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
            /* SSM = 0 */
            break;

        case SPI_SSM_SOFTWARE:
            cr1_register |= (1U << 9);   /* SSM = 1 */
            cr1_register |= (1U << 8);   /* SSI = 1 */
            break;

        default:
            break;
    }
}

  /* Data frame format selection*/

switch (data_frame_format) {
        case SPI_DFF_16_BIT:
           cr1_register |= (1U << 11);
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
switch (clock_speed) {
        case SPI_SCLK_DIV2:
           break;
        case SPI_SCLK_DIV4:
           cr1_register |= (1U << 3);
           break;
        case SPI_SCLK_DIV8:
           cr1_register |= (1U << 4);
           break;
        case SPI_SCLK_DIV16:
           cr1_register |= (1U << 3);
           cr1_register |= (1U << 4);
           break;
        case SPI_SCLK_DIV32:
           cr1_register |= (1U << 5);
           break;
        case SPI_SCLK_DIV64:
           cr1_register |= (1U << 3);
           cr1_register |= (1U << 5);
           break;
         case SPI_SCLK_DIV128:
           cr1_register |= (1U << 4);
           cr1_register |= (1U << 5);
           break;
         case SPI_SCLK_DIV256:
           cr1_register |= (1U << 3);
           cr1_register |= (1U << 4);
           cr1_register |= (1U << 5);
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

// write the register to the actual hardware
 spi_port->CR1 = cr1_register;


/* Connect board's pins to SPI pins of the MCU*/
SPI_GPIO_pin_setup(spi_port, slave_select_mode);

}