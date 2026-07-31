#include "main.h"
#include <stdint.h>

void SystemClock_Config(void);

int32_t sys_add(int32_t a, int32_t b)
{
    return a + b;
}

int32_t sys_sub(int32_t a, int32_t b)
{
    return a - b;
}

void svc_handler_c(uint32_t *svc_args)
{
    uint8_t svc_number;
    uint32_t pc;

    pc = svc_args[6];
    svc_number = ((uint8_t *)pc)[-2];

    switch (svc_number)
    {
        case 0:
            svc_args[0] = sys_add((int32_t)svc_args[0], (int32_t)svc_args[1]);
            break;
        case 1:
            svc_args[0] = sys_sub((int32_t)svc_args[0], (int32_t)svc_args[1]);
            break;
        default:
            svc_args[0] = (uint32_t)-1;
            break;
    }
}

__attribute__((naked)) void SVC_Handler(void)
{
    __asm volatile (
        "tst lr, #4\n\t"
        "ite eq\n\t"
        "mrseq r0, msp\n\t"
        "mrsne r0, psp\n\t"
        "b svc_handler_c\n\t"
    );
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    volatile uint32_t res1;
    volatile uint32_t res2;

    register uint32_t r0 __asm__("r0") = 15;
    register uint32_t r1 __asm__("r1") = 27;
    __asm volatile ("svc #0" : "+r"(r0) : "r"(r1) : "memory");
    res1 = r0;

    r0 = 50;
    r1 = 20;
    __asm volatile ("svc #1" : "+r"(r0) : "r"(r1) : "memory");
    res2 = r0;

    while (1)
    {
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}