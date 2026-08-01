#pragma once
#define RCC_BASE 0x40023800U

#define RCC_AHB1ENR (*(volatile uint32_t*)(RCC_BASE+0x30UL)) 
#define RCC_APB1ENR (*(volatile uint32_t*)(RCC_BASE+0x40UL)) 

#define  RCC_AHB1ENR_GPIOAEN (1U<<0)
#define RCC_APB1ENR_USART2EN (1U << 17)