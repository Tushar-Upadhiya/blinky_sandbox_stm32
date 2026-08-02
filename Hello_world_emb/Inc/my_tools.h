#pragma once
#define RCC_BASE 0x40023800UL
#define GPIOA_BASE 0x40020000UL
#define USART2_BASE 0x40004400UL

#define RCC_AHB1ENR (*(volatile uint32_t*)(RCC_BASE+0x30UL)) 
#define RCC_APB1ENR (*(volatile uint32_t*)(RCC_BASE+0x40UL)) 

#define  RCC_AHB1ENR_GPIOAEN (1U<<0)
#define RCC_APB1ENR_USART2EN (1U << 17)

#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE+0x00UL))
#define GPIOA_AFRL  (*(volatile uint32_t *)(GPIOA_BASE+0x20UL))

#define USART2_SR           (*(volatile uint32_t *)(USART2_BASE + 0x00UL))
#define USART2_DR           (*(volatile uint32_t *)(USART2_BASE + 0x04UL))
#define USART2_BRR          (*(volatile uint32_t *)(USART2_BASE + 0x08UL))
#define USART2_CR1          (*(volatile uint32_t *)(USART2_BASE + 0x0CUL))


