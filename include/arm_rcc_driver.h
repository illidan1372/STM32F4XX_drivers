#ifndef ARM_RCC_DRIVER_H
#define ARM_RCC_DRIVER_H

#include <stdint.h>

/* Allowed numeric factors */
#define RCC_PLL_M_MIN             2U
#define RCC_PLL_M_MAX             63U
#define RCC_PLL_N_MIN             50U
#define RCC_PLL_N_MAX             432U

/* Main PLL frequency constraints, in Hz */
#define RCC_PLL_VCO_INPUT_MIN_HZ  1000000U
#define RCC_PLL_VCO_INPUT_MAX_HZ  2000000U
#define RCC_PLL_VCO_OUTPUT_MIN_HZ 100000000U
#define RCC_PLL_VCO_OUTPUT_MAX_HZ 432000000U
#define RCC_PLL_P_OUTPUT_MAX_HZ   180000000U

typedef enum
{
    RCC_OK = 0,
    RCC_ERROR_INVALID_CONFIG,
    RCC_ERROR_TIMEOUT
} RCC_Status_t;

typedef enum
{
    RCC_HSE_CRYSTAL = 0,
    RCC_HSE_EXTERNAL_CLOCK
} RCC_HSE_Source_t;

typedef enum
{
    RCC_SYSCLK_HSI   = 0U,
    RCC_SYSCLK_HSE   = 1U,
    RCC_SYSCLK_PLL_P = 2U,
    RCC_SYSCLK_PLL_R = 3U
} RCC_SYSCLK_Source_t;

typedef enum
{
    RCC_PLL_SOURCE_HSI = 0U,
    RCC_PLL_SOURCE_HSE = 1U
} RCC_PLL_Source_t;

/* Unshifted PLLP register encodings, not the actual divisors. */
typedef enum
{
    RCC_PLL_P_DIV2 = 0U,
    RCC_PLL_P_DIV4 = 1U,
    RCC_PLL_P_DIV6 = 2U,
    RCC_PLL_P_DIV8 = 3U
} RCC_PLL_P_Divider_t;

/* PLLQ encodes the divisor directly. */
typedef enum
{
    RCC_PLL_Q_DIV2  = 2U,
    RCC_PLL_Q_DIV3  = 3U,
    RCC_PLL_Q_DIV4  = 4U,
    RCC_PLL_Q_DIV5  = 5U,
    RCC_PLL_Q_DIV6  = 6U,
    RCC_PLL_Q_DIV7  = 7U,
    RCC_PLL_Q_DIV8  = 8U,
    RCC_PLL_Q_DIV9  = 9U,
    RCC_PLL_Q_DIV10 = 10U,
    RCC_PLL_Q_DIV11 = 11U,
    RCC_PLL_Q_DIV12 = 12U,
    RCC_PLL_Q_DIV13 = 13U,
    RCC_PLL_Q_DIV14 = 14U,
    RCC_PLL_Q_DIV15 = 15U
} RCC_PLL_Q_Divider_t;

/* PLLR encodes the divisor directly. */
typedef enum
{
    RCC_PLL_R_DIV2 = 2U,
    RCC_PLL_R_DIV3 = 3U,
    RCC_PLL_R_DIV4 = 4U,
    RCC_PLL_R_DIV5 = 5U,
    RCC_PLL_R_DIV6 = 6U,
    RCC_PLL_R_DIV7 = 7U
} RCC_PLL_R_Divider_t;

RCC_Status_t RCC_HSI_enable(void);
RCC_Status_t RCC_HSE_enable(RCC_HSE_Source_t source);
RCC_Status_t RCC_HSI_disable(void);
RCC_Status_t RCC_HSE_disable(void);
RCC_Status_t RCC_SYSCLK_select(RCC_SYSCLK_Source_t source);

/*
 * Select HSI or HSE through PLLSRC without enabling or disabling clocks.
 * Returns RCC_ERROR_INVALID_CONFIG for an invalid source, any enabled PLL
 * (PLL, PLLI2S or PLLSAI), or a selected oscillator whose RDY bit is clear.
 * PLLSRC is unchanged on error. Returns RCC_OK on success.
 */
RCC_Status_t RCC_PLL_source_select(RCC_PLL_Source_t source);

/*
 * Configure M/N/P/Q/R together while the main PLL is disabled.
 * source_hz is the frequency of the oscillator already selected by
 * RCC_PLL_source_select().
 * Validate all factors and resulting VCO/P frequencies before writing.
 * Preserve PLLSRC and reserved bits; leave the register unchanged on error.
 * Peripheral-specific output requirements remain the caller's responsibility.
 */
RCC_Status_t RCC_PLL_configure(uint32_t source_hz,
                               uint32_t m,
                               uint32_t n,
                               RCC_PLL_P_Divider_t p,
                               RCC_PLL_Q_Divider_t q,
                               RCC_PLL_R_Divider_t r);

/*
 * Check the stored factor ranges and require the selected oscillator ready.
 * The caller must ensure frequency constraints are satisfied beforehand
 * (RCC_PLL_configure() is intended to perform those checks).
 * Enable the main PLL and wait for PLLRDY, with a timeout. An already locked
 * PLL returns RCC_OK after validation. Invalid configuration leaves registers
 * unchanged; a timeout leaves PLLON set, matching oscillator enable behavior.
 */
RCC_Status_t RCC_PLL_enable(void);

/*
 * Reject disabling when PLL_P or PLL_R is the active or requested SYSCLK
 * source. Otherwise disable and wait for PLLRDY to clear, with a timeout.
 * The caller must first move or stop peripheral consumers.
 */
RCC_Status_t RCC_PLL_disable(void);

#endif /* ARM_RCC_DRIVER_H */
