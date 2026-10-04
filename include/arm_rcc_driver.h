#ifndef ARM_RCC_DRIVER_H
#define ARM_RCC_DRIVER_H

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

RCC_Status_t RCC_HSI_enable(void);
RCC_Status_t RCC_HSE_enable(RCC_HSE_Source_t source);
RCC_Status_t RCC_HSI_disable(void);
RCC_Status_t RCC_HSE_disable(void);
RCC_Status_t RCC_SYSCLK_select(RCC_SYSCLK_Source_t source);

#endif /* ARM_RCC_DRIVER_H */
