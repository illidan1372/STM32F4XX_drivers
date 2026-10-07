#include "arm_rcc_driver.h"
#include "arm_stm32f446xx.h"

#define RCC_STARTUP_TIMEOUT 100000U

/*
 * TODO[POST-TIMER]:
 * Replace iteration-based oscillator startup timeouts
 * with a time-based timeout mechanism.
 */

RCC_Status_t RCC_HSI_enable(void)
{
    uint32_t timeout_period = RCC_STARTUP_TIMEOUT;

    RCC->RCC_CR |= (1U << RCC_CR_HSION_OFFSET);

    while ((RCC->RCC_CR & (1U << RCC_CR_HSIRDY_OFFSET)) == 0U)
    {
        if (timeout_period == 0U)
        {
            return RCC_ERROR_TIMEOUT;
        }

        timeout_period -= 1U;
    }

    return RCC_OK;
}

static RCC_Status_t RCC_HSE_crystal_enable(void);
static RCC_Status_t RCC_HSE_bypass_enable(void);

RCC_Status_t RCC_HSE_enable(RCC_HSE_Source_t source)
{
    switch (source)
    {
        case RCC_HSE_CRYSTAL:
            return RCC_HSE_crystal_enable();

        case RCC_HSE_EXTERNAL_CLOCK:
            return RCC_HSE_bypass_enable();

        default:
            return RCC_ERROR_INVALID_CONFIG;
    }
}

static RCC_Status_t RCC_HSE_crystal_enable(void)
{
    uint32_t timeout_period = RCC_STARTUP_TIMEOUT;

    RCC->RCC_CR &= ~(1U << RCC_CR_HSEBYP_OFFSET);
    RCC->RCC_CR |= (1U << RCC_CR_HSEON_OFFSET);

    while ((RCC->RCC_CR & (1U << RCC_CR_HSERDY_OFFSET)) == 0U)
    {
        if (timeout_period == 0U)
        {
            return RCC_ERROR_TIMEOUT;
        }

        timeout_period -= 1U;
    }

    return RCC_OK;
}

static RCC_Status_t RCC_HSE_bypass_enable(void)
{
    uint32_t timeout_period = RCC_STARTUP_TIMEOUT;

    RCC->RCC_CR |= (1U << RCC_CR_HSEBYP_OFFSET);
    RCC->RCC_CR |= (1U << RCC_CR_HSEON_OFFSET);

    while ((RCC->RCC_CR & (1U << RCC_CR_HSERDY_OFFSET)) == 0U)
    {
        if (timeout_period == 0U)
        {
            return RCC_ERROR_TIMEOUT;
        }

        timeout_period -= 1U;
    }

    return RCC_OK;
}

RCC_Status_t RCC_PLL_source_select(RCC_PLL_Source_t source)
{
    switch (source)
    {
        case RCC_PLL_SOURCE_HSI:
        case RCC_PLL_SOURCE_HSE:
            break;

        default:
            return RCC_ERROR_INVALID_CONFIG;
    }

    /* PLLSRC is shared by PLL, PLLI2S and PLLSAI. */
    if ((RCC->RCC_CR & ((1U << RCC_CR_PLLON_OFFSET)
                      | (1U << RCC_CR_PLLI2SON_OFFSET)
                      | (1U << RCC_CR_PLLSAION_OFFSET))) != 0U)
    {
        return RCC_ERROR_INVALID_CONFIG;
    }

    if (source == RCC_PLL_SOURCE_HSI)
    {
        if ((RCC->RCC_CR & (1U << RCC_CR_HSIRDY_OFFSET)) == 0U)
        {
            return RCC_ERROR_INVALID_CONFIG;
        }

        RCC->RCC_PLL_CFGR &= ~RCC_PLLCFGR_PLLSRC_MASK;
    }
    else
    {
        if ((RCC->RCC_CR & (1U << RCC_CR_HSERDY_OFFSET)) == 0U)
        {
            return RCC_ERROR_INVALID_CONFIG;
        }

        RCC->RCC_PLL_CFGR |= RCC_PLLCFGR_PLLSRC_MASK;
    }

    return RCC_OK;
}

RCC_Status_t RCC_SYSCLK_select(RCC_SYSCLK_Source_t source)
{
    uint32_t timeout_period = RCC_STARTUP_TIMEOUT;

    switch (source)
    {
        case RCC_SYSCLK_HSI:
        case RCC_SYSCLK_HSE:
        case RCC_SYSCLK_PLL_P:
        case RCC_SYSCLK_PLL_R:
            break;

        default:
            return RCC_ERROR_INVALID_CONFIG;
    }

    /* Clear SW[1:0]. */
    RCC->RCC_CFGR &= ~RCC_CFGR_SW_MASK;

    /* Write the requested source into SW[1:0]. */
    RCC->RCC_CFGR |= ((uint32_t)source << RCC_CFGR_SW_OFFSET);

    /* Wait until SWS[1:0] confirms the selected SYSCLK source. */
    while ((RCC->RCC_CFGR & RCC_CFGR_SWS_MASK)
           != ((uint32_t)source << RCC_CFGR_SWS_OFFSET))
    {
        if (timeout_period == 0U)
        {
            return RCC_ERROR_TIMEOUT;
        }

        timeout_period -= 1U;
    }

    return RCC_OK;
}

RCC_Status_t RCC_HSI_disable(void)
{
    uint32_t timeout_period = RCC_STARTUP_TIMEOUT;

    /*
     * TODO[POST-RCC2]:
     * Reject this operation when HSI is the active SYSCLK source
     * or is required by the active clock configuration.
     */
    RCC->RCC_CR &= ~(1U << RCC_CR_HSION_OFFSET);

    while ((RCC->RCC_CR & (1U << RCC_CR_HSIRDY_OFFSET)) != 0U)
    {
        if (timeout_period == 0U)
        {
            return RCC_ERROR_TIMEOUT;
        }

        timeout_period -= 1U;
    }

    return RCC_OK;
}

RCC_Status_t RCC_HSE_disable(void)
{
    uint32_t timeout_period = RCC_STARTUP_TIMEOUT;

    /*
     * TODO[POST-RCC2]:
     * Reject this operation when HSE is the active SYSCLK source
     * or is required by the active clock configuration.
     */
    RCC->RCC_CR &= ~(1U << RCC_CR_HSEON_OFFSET);

    while ((RCC->RCC_CR & (1U << RCC_CR_HSERDY_OFFSET)) != 0U)
    {
        if (timeout_period == 0U)
        {
            return RCC_ERROR_TIMEOUT;
        }

        timeout_period -= 1U;
    }

    return RCC_OK;
}
