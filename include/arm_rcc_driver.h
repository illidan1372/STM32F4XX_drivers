#ifndef ARM_RCC_DRIVER_H
#define ARM_RCC_DRIVER_H

typedef enum
{
    RCC_OK = 0,
    RCC_ERROR_INVALID_CONFIG,
    RCC_ERROR_TIMEOUT
} RCC_Status_t;

RCC_Status_t RCC_HSI_enable(void);
RCC_Status_t RCC_HSE_enable(void);
RCC_Status_t RCC_HSI_disable(void);
RCC_Status_t RCC_HSE_disable(void);

#endif /* ARM_RCC_DRIVER_H */
