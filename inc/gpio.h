#ifndef GPIO_H
#define GPIO_H

#define TIM_2  (volatile uint32_t*) 0x40000000

#define TIM_DIER 0x3
#define TIM_SR 0x4
#define TIM_CNT 0x9
#define TIM_ARR 0x11

#define GPIO_A (volatile uint32_t*) 0x40020000
#define GPIO_B (volatile uint32_t*) 0x40020400
#define GPIO_C (volatile uint32_t*) 0x40020800
#define GPIO_D (volatile uint32_t*) 0x40020C00

#define GPIO_A_EN (1<<0)
#define GPIO_B_EN (1<<1)
#define GPIO_C_EN (1<<2)

#define RCC_BASE (volatile uint32_t*) 0x40023800
#define RCC_AHB1 (volatile uint32_t*) 0x40023830
#define RCC_APB1 (volatile uint32_t*) 0x40023840
#define RCC_APB2 (volatile uint32_t*) 0x40023844

#define EXTI_BASE (volatile uint32_t*) 0x40013C00
#define EXTI_IMR 0x0
#define EXTI_RTSR 0x2
#define EXTI_FTSR 0x3
#define EXTI_SWIER 0x4
#define EXTI_PR 0x5

#define SYSCFG_CR1 (volatile uint32_t*) 0x40013808

#define NVIC_ISER0 (volatile uint32_t*) 0xE000E100

#define STK_CTRL         (volatile uint32_t)0xE000E010
#define STK_LOAD         (volatile uint32_t)0xE000E014
#define STK_VAL         (volatile uint32_t*)0xE000E018
#define STK_CALIB        (volatile uint32_t)0xE000E01C
#define    CLKSOURCE 1<<2    // Use processor clock (16 Mhz)
#define    TICKINT 1<<1
#define    ENABLE 1<<0

#define GPIO_MODER 0x0
#define GPIO_PUPDR 0X3
#define GPIO_IDR 0x4
#define GPIO_ODR 0x5
#define GPIO_BSRR 0x6

#endif
