#include "arm_nucleof446re.h"
#include "arm_stm32f446xx.h"
#include "arm_spi_driver.h"


int main(void){


    SPI_CONFIG_t spi_conf = {0};

    spi_conf.SPI_device_mode = SPI_DEVICE_MODE_MASTER;
    spi_conf.SPI_bus_config = SPI_MODE_FULL_DUPLEX;
    spi_conf.SPI_ssm = SPI_SSM_HARDWARE;
    spi_conf.SPI_dff = SPI_DFF_8_BIT;
    spi_conf.SPI_CPHA = SPI_CPHA_FIRST_EDGE;
    spi_conf.SPI_CPOL = SPI_CPOL_LOW;
    spi_conf.SPI_SCLK_speed = SPI_SCLK_DIV2;
    spi_conf.SPI_ssoe = SPI_NSS_OUTPUT;

    SPI_HANDLE_t spi_handle = {0};
    spi_handle.SPI_config = spi_conf;
    spi_handle.pSPIx = (SPI_REGDEF_t *)SPI1;

    if (SPI_init(&spi_handle) != SPI_OK) {

        while (1) {
            user_LED_toggle();

            for (volatile int i = 0; i < 10000000; i++) {
                ;
            }
        }
    }

    if (SPI_enable(&spi_handle) != SPI_OK) {

        while (1) {
            user_LED_toggle();

            for (volatile int i = 0; i < 10000000; i++) {
                ;
            }
        }
    }

    uint8_t spi_rx[8] = {0};

    const uint8_t spi_tx[8] = {
        0x00,
        0xFF,
        0xAA,
        0x55,
        0x01,
        0x80,
        0x37,
        0xC3
    };

    if (SPI_data_exchange(&spi_handle,
                          SPI_OPERATION_EXCHANGE,
                          spi_tx,
                          spi_rx,
                          8u) != SPI_OK) {

        while (1) {
            user_LED_toggle();

            for (volatile int i = 0; i < 10000000; i++) {
                ;
            }
        }
    }

    uint8_t is_equal = 1;

    for (int i = 0; i < 8; i++) {

        if (spi_rx[i] != spi_tx[i])
        {
            is_equal = 0;
        }
    }

    while (1) {

    if (is_equal == 0) {

        user_LED_toggle();

        for (volatile int i = 0; i < 10000000; i++) {
            ;
        }

    } else {

        usr_LED_set(1);
    }
}


};