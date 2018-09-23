#ifndef GPIO_H
#define GPIO_H

#define GPIO_A (volatile uint32_t*) 0x40020000
#define GPIO_B (volatile uint32_t*) 0x40020400
#define GPIO_C (volatile uint32_t*) 0x40020800

#define GPIO_A_EN (1<<0)
#define GPIO_B_EN (1<<1)
#define GPIO_C_EN (1<<2)

#define RCC_BASE (volatile uint32_t*) 0x40023800
#define RCC_AHB1 (volatile uint32_t*) 0x40023830
#define RCC_APB1 (volatile uint32_t*) 0x40023840
#define RCC_APB2 (volatile uint32_t*) 0x40023844

#define GPIO_MODER 0x0
#define GPIO_IDR 0x4
#define GPIO_ODR 0x5
#define GPIO_BSRR 0x6

#endif
