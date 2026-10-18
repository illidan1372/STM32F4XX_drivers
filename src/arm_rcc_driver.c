#include "arm_rcc_driver.h"
#include "arm_stm32f446xx.h"

#define RCC_STARTUP_TIMEOUT 100000U
#define RCC_HSI_HZ 16000000U
#define RCC_HSE_MAX_HZ 50000000U
#define RCC_SCALE1_NORMAL_MAX_HZ 168000000U
#define RCC_HCLK_MAX_HZ 180000000U

static RCC_Status_t RCC_wait_bits(volatile uint32_t *reg,
                                  uint32_t mask, uint32_t expected)
{
    uint32_t timeout_period = RCC_STARTUP_TIMEOUT;

    while ((*reg & mask) != expected)
    {
        if (timeout_period == 0U)
        {
            return RCC_ERROR_TIMEOUT;
        }
        timeout_period -= 1U;
    }
    return RCC_OK;
}

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

   /*
 * Build a mask containing the ON bits for PLL, PLLI2S and PLLSAI.
 * The bitwise OR operator '|' combines the three individual bit masks
 * into one mask so they can all be checked with a single '&' operation.
 *
 * If any of those ON bits are set in RCC_CR, the result of the '&'
 * operation will be non-zero, meaning at least one PLL is currently enabled.
 */
    
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

RCC_Status_t RCC_PLL_configure(uint32_t source_hz,
                               uint32_t m,
                               uint32_t n,
                               RCC_PLL_P_Divider_t p,
                               RCC_PLL_Q_Divider_t q,
                               RCC_PLL_R_Divider_t r)
{
    uint64_t vco_numerator;
    uint32_t p_divisor;
    uint32_t pll_config;

    /*
     * PLLP uses register encodings 0..3 for actual divisors 2, 4, 6, 8.
     * RCC_PLL_P_DIV8 is the enum value 3, not the number 8.
     * Casting p to uint32_t also rejects negative enum inputs: e.g., -1
     * becomes a large unsigned number and fails the upper-bound check.
     */
    if ((m < RCC_PLL_M_MIN) || (m > RCC_PLL_M_MAX)
        || (n < RCC_PLL_N_MIN) || (n > RCC_PLL_N_MAX)
        || ((uint32_t)p > (uint32_t)RCC_PLL_P_DIV8)
        || ((uint32_t)q < (uint32_t)RCC_PLL_Q_DIV2)
        || ((uint32_t)q > (uint32_t)RCC_PLL_Q_DIV15)
        || ((uint32_t)r < (uint32_t)RCC_PLL_R_DIV2)
        || ((uint32_t)r > (uint32_t)RCC_PLL_R_DIV7))
    {
        return RCC_ERROR_INVALID_CONFIG;
    }

    /*
     * PLLP stores an encoding, not the divisor itself:
     * p = 0, 1, 2, 3 represents division by 2, 4, 6, 8 respectively.
     * These divisors follow P = 2 * (p + 1); for example, encoding 1
     * (RCC_PLL_P_DIV4) gives P = 2 * (1 + 1) = 4.
     */
    p_divisor = 2U * ((uint32_t)p + 1U);
    /*
     * VCO output frequency is f_VCO = (source_hz * N) / M.
     * Save the numerator here; the frequency checks below compare it
     * against limits multiplied by M, avoiding integer division/truncation.
     * Cast source_hz to uint64_t BEFORE multiplying: e.g. 16 MHz * 336
     * = 5,376,000,000, which overflows uint32_t even though dividing
     * by M = 16 gives a valid 336 MHz VCO frequency.
     */
    vco_numerator = (uint64_t)source_hz * n;

    /*
     * VCO input = source_hz / M; VCO output = source_hz * N / M.
     * Cross-multiply the limits to avoid truncating fractional frequencies.
     * Use 64-bit products because intermediate values can exceed 32 bits.
     */
    if (((uint64_t)source_hz < (uint64_t)RCC_PLL_VCO_INPUT_MIN_HZ * m)
        || ((uint64_t)source_hz > (uint64_t)RCC_PLL_VCO_INPUT_MAX_HZ * m)
        || (vco_numerator < (uint64_t)RCC_PLL_VCO_OUTPUT_MIN_HZ * m)
        || (vco_numerator > (uint64_t)RCC_PLL_VCO_OUTPUT_MAX_HZ * m)
        || (vco_numerator > (uint64_t)RCC_PLL_P_OUTPUT_MAX_HZ * m * p_divisor))
    {
        return RCC_ERROR_INVALID_CONFIG;
    }

    /* PLLRDY must also clear: PLLON alone does not confirm shutdown. */
    if ((RCC->RCC_CR & (RCC_CR_PLLON_MASK | RCC_CR_PLLRDY_MASK)) != 0U)
    {
        return RCC_ERROR_INVALID_CONFIG;
    }

    if (((RCC->RCC_CFGR & RCC_CFGR_SW_MASK) >= RCC_SYSCLK_PLL_P)
        || ((RCC->RCC_CFGR & RCC_CFGR_SWS_MASK)
            >= ((uint32_t)RCC_SYSCLK_PLL_P << RCC_CFGR_SWS_OFFSET)))
    {
        return RCC_ERROR_INVALID_CONFIG;
    }

    if (((RCC->RCC_PLL_CFGR & RCC_PLLCFGR_PLLSRC_MASK) == 0U)
        && (source_hz != RCC_HSI_HZ))
    {
        return RCC_ERROR_INVALID_CONFIG;
    }

    /*
     * RM0390 permits VOS changes only with PLL off and SYSCLK on HSI/HSE.
     * Read back the clock enable before accessing PWR. Scale 1 becomes
     * active after PLL enable; RCC_SYSCLK_select() waits for VOSRDY.
     */
    RCC->RCC_APB1_ENR |= RCC_APB1ENR_PWREN_MASK;
    (void)RCC->RCC_APB1_ENR;
    *PWR_CR_REG = (*PWR_CR_REG & ~PWR_CR_VOS_MASK) | PWR_CR_VOS_SCALE1;
    (void)*PWR_CR_REG;

    /* Replace only the factors, preserving PLLSRC and reserved bits. */
    pll_config = RCC->RCC_PLL_CFGR;
    pll_config &= ~(RCC_PLLCFGR_PLLM_MASK | RCC_PLLCFGR_PLLN_MASK
                    | RCC_PLLCFGR_PLLP_MASK | RCC_PLLCFGR_PLLQ_MASK
                    | RCC_PLLCFGR_PLLR_MASK);
    pll_config |= (m << RCC_PLLCFGR_PLLM_OFFSET)
                  | (n << RCC_PLLCFGR_PLLN_OFFSET)
                  | ((uint32_t)p << RCC_PLLCFGR_PLLP_OFFSET)
                  | ((uint32_t)q << RCC_PLLCFGR_PLLQ_OFFSET)
                  | ((uint32_t)r << RCC_PLLCFGR_PLLR_OFFSET);
    RCC->RCC_PLL_CFGR = pll_config;

    return RCC_OK;
}

/*
 * TODO[POST-TIMER]:
 * Replace the iteration-count PLL ready wait with a timer-based timeout
 * once the timer drivers are ready, so the timeout measures elapsed time
 * rather than a CPU-speed-dependent number of loop iterations.
 */
RCC_Status_t RCC_PLL_enable(void)
{
    uint32_t timeout_period = RCC_STARTUP_TIMEOUT;
    uint32_t pll_config = RCC->RCC_PLL_CFGR;
    uint32_t m = (pll_config & RCC_PLLCFGR_PLLM_MASK)
                 >> RCC_PLLCFGR_PLLM_OFFSET;
    uint32_t n = (pll_config & RCC_PLLCFGR_PLLN_MASK)
                 >> RCC_PLLCFGR_PLLN_OFFSET;
    uint32_t q = (pll_config & RCC_PLLCFGR_PLLQ_MASK)
                 >> RCC_PLLCFGR_PLLQ_OFFSET;
    uint32_t r = (pll_config & RCC_PLLCFGR_PLLR_MASK)
                 >> RCC_PLLCFGR_PLLR_OFFSET;
    uint32_t source_ready_mask;

    /* Every two-bit PLLP encoding is valid; check the other factors. */
    if ((m < RCC_PLL_M_MIN) || (m > RCC_PLL_M_MAX)
        || (n < RCC_PLL_N_MIN) || (n > RCC_PLL_N_MAX)
        || (q < RCC_PLL_Q_DIV2) || (q > RCC_PLL_Q_DIV15)
        || (r < RCC_PLL_R_DIV2) || (r > RCC_PLL_R_DIV7))
    {
        return RCC_ERROR_INVALID_CONFIG;
    }

    /*
     * Hardware defines PLLSRC = 0 as HSI and PLLSRC = 1 as HSE.
     * Isolate this bit to identify the PLL input oscillator, then select
     * its RDY flag so we can check that it is ready before enabling PLL.
     * This only prepares a mask; it does not change the clock source.
     */
    if ((pll_config & RCC_PLLCFGR_PLLSRC_MASK) == 0U)
    {
        source_ready_mask = (1U << RCC_CR_HSIRDY_OFFSET);
    }
    else
    {
        source_ready_mask = (1U << RCC_CR_HSERDY_OFFSET);
    }

    if ((RCC->RCC_CR & source_ready_mask) == 0U)
    {
        return RCC_ERROR_INVALID_CONFIG;
    }

    /*
     * Set PLLON (bit 24) to enable the main PLL, preserving the other bits.
     * Enabling starts the PLL; the wait below checks PLLRDY for lock.
     */
    RCC->RCC_CR |= RCC_CR_PLLON_MASK;

    /* This also handles a PLL that is already enabled and locking. */
    while ((RCC->RCC_CR & RCC_CR_PLLRDY_MASK) == 0U)
    {
        if (timeout_period == 0U)
        {
            return RCC_ERROR_TIMEOUT;
        }

        timeout_period -= 1U;
    }

    return RCC_OK;
}

RCC_Status_t RCC_SYSCLK_select(RCC_SYSCLK_Source_t source,
                               uint32_t hse_hz,
                               uint32_t supply_mv)
{
    uint32_t source_ready_mask;
    uint32_t sysclk_hz;
    uint32_t latency_step_hz;
    uint32_t latency;
    uint32_t old_latency;
    uint32_t pll_config;
    uint32_t pll_input_hz;
    uint32_t pll_m;
    uint32_t pll_n;
    uint32_t pll_divisor;
    uint32_t clock_config;
    uint64_t pll_numerator;
    uint32_t pll_denominator;
    RCC_Status_t status;

    if ((supply_mv < 1800U) || (supply_mv > 3600U))
    {
        return RCC_ERROR_INVALID_CONFIG;
    }

    switch (source)
    {
        case RCC_SYSCLK_HSI:
            sysclk_hz = RCC_HSI_HZ;
            source_ready_mask = (1U << RCC_CR_HSIRDY_OFFSET);
            break;

        case RCC_SYSCLK_HSE:
            if ((hse_hz == 0U) || (hse_hz > RCC_HSE_MAX_HZ))
            {
                return RCC_ERROR_INVALID_CONFIG;
            }
            sysclk_hz = hse_hz;
            source_ready_mask = (1U << RCC_CR_HSERDY_OFFSET);
            break;

        case RCC_SYSCLK_PLL_P:
        case RCC_SYSCLK_PLL_R:
            pll_config = RCC->RCC_PLL_CFGR;
            pll_input_hz = RCC_HSI_HZ;
            source_ready_mask = (1U << RCC_CR_HSIRDY_OFFSET);
            if ((pll_config & RCC_PLLCFGR_PLLSRC_MASK) != 0U)
            {
                if ((hse_hz == 0U) || (hse_hz > RCC_HSE_MAX_HZ))
                {
                    return RCC_ERROR_INVALID_CONFIG;
                }
                pll_input_hz = hse_hz;
                source_ready_mask = (1U << RCC_CR_HSERDY_OFFSET);
            }
            source_ready_mask |= RCC_CR_PLLON_MASK | RCC_CR_PLLRDY_MASK;
            pll_m = (pll_config & RCC_PLLCFGR_PLLM_MASK) >> RCC_PLLCFGR_PLLM_OFFSET;
            pll_n = (pll_config & RCC_PLLCFGR_PLLN_MASK) >> RCC_PLLCFGR_PLLN_OFFSET;
            pll_divisor = 2U * (((pll_config & RCC_PLLCFGR_PLLP_MASK)
                                >> RCC_PLLCFGR_PLLP_OFFSET) + 1U);
            if (source == RCC_SYSCLK_PLL_R)
            {
                pll_divisor = (pll_config & RCC_PLLCFGR_PLLR_MASK)
                              >> RCC_PLLCFGR_PLLR_OFFSET;
            }
            if ((pll_m < RCC_PLL_M_MIN) || (pll_m > RCC_PLL_M_MAX)
                || (pll_n < RCC_PLL_N_MIN) || (pll_n > RCC_PLL_N_MAX)
                || (pll_divisor < 2U))
            {
                return RCC_ERROR_INVALID_CONFIG;
            }
            pll_numerator = (uint64_t)pll_input_hz * pll_n;
            pll_denominator = pll_m * pll_divisor;
            if (((uint64_t)pll_input_hz < (uint64_t)RCC_PLL_VCO_INPUT_MIN_HZ * pll_m)
                || ((uint64_t)pll_input_hz > (uint64_t)RCC_PLL_VCO_INPUT_MAX_HZ * pll_m)
                || (pll_numerator < (uint64_t)RCC_PLL_VCO_OUTPUT_MIN_HZ * pll_m)
                || (pll_numerator > (uint64_t)RCC_PLL_VCO_OUTPUT_MAX_HZ * pll_m)
                || (pll_numerator > (uint64_t)RCC_HCLK_MAX_HZ * pll_denominator))
            {
                return RCC_ERROR_INVALID_CONFIG;
            }
            /* Round up so a fractional Hz cannot underestimate Flash latency. */
            sysclk_hz = (uint32_t)((pll_numerator + pll_denominator - 1U)
                                  / pll_denominator);
            if ((*PWR_CR_REG & PWR_CR_VOS_MASK) != PWR_CR_VOS_SCALE1)
            {
                return RCC_ERROR_INVALID_CONFIG;
            }
            break;

        default:
            return RCC_ERROR_INVALID_CONFIG;
    }

    if ((RCC->RCC_CR & source_ready_mask) != source_ready_mask)
    {
        return RCC_ERROR_INVALID_CONFIG;
    }

    /* SW is the request; SWS is the actual source. Reject a pending switch. */
    clock_config = RCC->RCC_CFGR;
    if ((clock_config & RCC_CFGR_SW_MASK)
        != ((clock_config & RCC_CFGR_SWS_MASK) >> RCC_CFGR_SWS_OFFSET))
    {
        return RCC_ERROR_INVALID_CONFIG;
    }

    if (sysclk_hz > RCC_SCALE1_NORMAL_MAX_HZ)
    {
        if ((supply_mv <= 2100U)
            || (((*PWR_CSR_REG & PWR_CSR_ODSWRDY_MASK) == 0U)
                && ((clock_config & RCC_CFGR_SW_MASK) >= RCC_SYSCLK_PLL_P)))
        {
            return RCC_ERROR_INVALID_CONFIG;
        }
    }

    if (source >= RCC_SYSCLK_PLL_P)
    {
        RCC->RCC_APB1_ENR |= RCC_APB1ENR_PWREN_MASK;
        (void)RCC->RCC_APB1_ENR;
        status = RCC_wait_bits(PWR_CSR_REG, PWR_CSR_VOSRDY_MASK, PWR_CSR_VOSRDY_MASK);
        if (status != RCC_OK)
        {
            return status;
        }
    }

    /*
     * Over-drive is a two-stage handshake: ODEN -> ODRDY, then
     * ODSWEN -> ODSWRDY. Do not switch SYSCLK on either timeout.
     * The caller must stop peripheral consumers before this transition.
     */
    if ((sysclk_hz > RCC_SCALE1_NORMAL_MAX_HZ)
        && ((*PWR_CSR_REG & PWR_CSR_ODSWRDY_MASK) == 0U))
    {
        *PWR_CR_REG |= PWR_CR_ODEN_MASK;
        status = RCC_wait_bits(PWR_CSR_REG, PWR_CSR_ODRDY_MASK, PWR_CSR_ODRDY_MASK);
        if (status != RCC_OK)
        {
            return status;
        }
        *PWR_CR_REG |= PWR_CR_ODSWEN_MASK;
        status = RCC_wait_bits(PWR_CSR_REG, PWR_CSR_ODSWRDY_MASK,
                               PWR_CSR_ODSWRDY_MASK);
        if (status != RCC_OK)
        {
            return status;
        }
    }

    /*
     * RM0390 Flash table: each additional wait state allows another
     * 20/22/24/30 MHz depending on VDD. At a voltage boundary, use the
     * more conservative column. Final HCLK equals SYSCLK because HPRE = /1.
     */
    latency_step_hz = 20000000U;
    if (supply_mv > 2700U)
    {
        latency_step_hz = 30000000U;
    }
    else if (supply_mv > 2400U)
    {
        latency_step_hz = 24000000U;
    }
    else if (supply_mv > 2100U)
    {
        latency_step_hz = 22000000U;
    }
    /* Equivalent to ceil(HCLK / step) - 1, including exact boundaries. */
    latency = (sysclk_hz - 1U) / latency_step_hz;
    old_latency = *FLASH_ACR_REG & FLASH_ACR_LATENCY_MASK;
    if (supply_mv <= 2100U)
    {
        *FLASH_ACR_REG &= ~FLASH_ACR_PRFTEN_MASK;
    }
    /* Extra wait states must be effective before the CPU clock rises. */
    if (latency > old_latency)
    {
        *FLASH_ACR_REG = (*FLASH_ACR_REG & ~FLASH_ACR_LATENCY_MASK) | latency;
        status = RCC_wait_bits(FLASH_ACR_REG, FLASH_ACR_LATENCY_MASK, latency);
        if (status != RCC_OK)
        {
            return status;
        }
    }

    /*
     * Apply APB /4 and /2 before the switch: even at 180 MHz, APB1 stays
     * at 45 MHz and APB2 at 90 MHz. Prescalers take up to 16 AHB cycles
     * to take effect; volatile reads keep this settling interval present.
     */
    RCC->RCC_CFGR = (RCC->RCC_CFGR & ~(RCC_CFGR_PPRE1_MASK | RCC_CFGR_PPRE2_MASK))
                   | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;
    for (uint32_t settling_cycles = 0U; settling_cycles < 16U; settling_cycles += 1U)
    {
        (void)RCC->RCC_CFGR;
    }
    RCC->RCC_CFGR = (RCC->RCC_CFGR & ~RCC_CFGR_SW_MASK) | (uint32_t)source;
    status = RCC_wait_bits(&RCC->RCC_CFGR, RCC_CFGR_SWS_MASK,
                           (uint32_t)source << RCC_CFGR_SWS_OFFSET);
    if (status != RCC_OK)
    {
        return status;
    }
    /* Keep the old AHB divider until SWS confirms the target source. */
    RCC->RCC_CFGR &= ~RCC_CFGR_HPRE_MASK;
    for (uint32_t settling_cycles = 0U; settling_cycles < 16U; settling_cycles += 1U)
    {
        (void)RCC->RCC_CFGR;
    }
    /* Reduce wait states only after the source and AHB divider settle. */
    if (latency < old_latency)
    {
        *FLASH_ACR_REG = (*FLASH_ACR_REG & ~FLASH_ACR_LATENCY_MASK) | latency;
        return RCC_wait_bits(FLASH_ACR_REG, FLASH_ACR_LATENCY_MASK, latency);
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
