/**
 * Blue Pill（STM32F103C8T6）PC13 板载灯 — HSI 8MHz，寄存器操作。
 * 节奏：约 1 秒长亮 → 快闪 3 次 → 循环。
 * PC13 LED 低电平点亮。
 */
#include "stm32f103xb.h"

/* 使能并选用内部高速时钟 HSI（8MHz） */
static void SystemInit_HSI(void)
{
  RCC->CR |= RCC_CR_HSION;
  while ((RCC->CR & RCC_CR_HSIRDY) == 0u) {
  }

  RCC->CFGR &= ~0x3u; /* 系统时钟开关选 HSI */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {
  }

  /* 8MHz 下 Flash 0 等待周期即可 */
  FLASH->ACR &= ~0x7u;
}

/*
 * 粗延时（毫秒近似）：按 HSI 8MHz 空循环估算，可按实机微调。
 * 约 1200 次内层循环 ≈ 1ms 量级（不同优化等级会有偏差）。
 */
static void delay_ms(uint32_t ms)
{
  while (ms--) {
    volatile uint32_t n = 1200u;
    while (n--) {
      __NOP();
    }
  }
}

static void led_on(void)
{
  GPIOC->BSRR = (1u << (13 + 16)); /* 复位 PC13 → 灯亮 */
}

static void led_off(void)
{
  GPIOC->BSRR = (1u << 13); /* 置位 PC13 → 灯灭 */
}

/* 打开 GPIOC 时钟，配置 PC13 为推挽输出 */
static void led_init(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

  /* CNF=00 通用推挽，MODE=10 输出 2MHz */
  GPIOC->CRH &= ~(GPIO_CRH_MODE13_Msk | GPIO_CRH_CNF13_Msk);
  GPIOC->CRH |= (0x2u << GPIO_CRH_MODE13_Pos);
  led_off();
}

int main(void)
{
  SystemInit_HSI();
  led_init();

  for (;;) {
    /* 约 1 秒长亮 */
    led_on();
    delay_ms(1000u);
    led_off();
    delay_ms(100u);

    /* 快闪 3 次：50ms 亮 / 50ms 灭 */
    {
      uint32_t i;
      for (i = 0u; i < 3u; i++) {
        led_on();
        delay_ms(50u);
        led_off();
        delay_ms(50u);
      }
    }
  }
}

/* 启动文件在进 main 前会调用；时钟放在 main 里做，此处留空 */
void SystemInit(void)
{
}
