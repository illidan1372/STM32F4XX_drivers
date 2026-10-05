#include "arm_rcc_driver.h"
#include "arm_nucleof446re.h"

int main(void)
{
    RCC_Status_t status;

    status = NUCLEO_F446RE_HSE_enable();
    if (status != RCC_OK)
    {
        user_LED_set(0);
        while (1);
    }

    status = RCC_SYSCLK_select(RCC_SYSCLK_HSE);
    if (status != RCC_OK)
    {
        user_LED_set(0);
        while (1);
    }

    status = RCC_SYSCLK_select(RCC_SYSCLK_HSI);
    if (status != RCC_OK)
    {
        user_LED_set(0);
        while (1);
    }

    status = RCC_HSE_disable();
    if (status != RCC_OK)
    {
        user_LED_set(0);
        while (1);
    }

    user_LED_set(1);

    while (1);
}