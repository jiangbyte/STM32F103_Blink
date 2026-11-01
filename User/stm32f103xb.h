/**
 * STM32F103xB（C8）最小寄存器映射，供 GPIO / RCC 闪灯示例使用。
 */
#ifndef STM32F103XB_H
#define STM32F103XB_H

#include <stdint.h>
#include "core_cm3.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FLASH_BASE            (0x08000000UL)
#define SRAM_BASE             (0x20000000UL)
#define PERIPH_BASE           (0x40000000UL)

#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)

#define GPIOC_BASE            (APB2PERIPH_BASE + 0x00001000UL)
#define RCC_BASE              (AHBPERIPH_BASE + 0x00001000UL)
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x00002000UL)

typedef struct {
  volatile uint32_t CRL;
  volatile uint32_t CRH;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t BRR;
  volatile uint32_t LCKR;
} GPIO_TypeDef;

typedef struct {
  volatile uint32_t CR;
  volatile uint32_t CFGR;
  volatile uint32_t CIR;
  volatile uint32_t APB2RSTR;
  volatile uint32_t APB1RSTR;
  volatile uint32_t AHBENR;
  volatile uint32_t APB2ENR;
  volatile uint32_t APB1ENR;
  volatile uint32_t BDCR;
  volatile uint32_t CSR;
} RCC_TypeDef;

typedef struct {
  volatile uint32_t ACR;
  volatile uint32_t KEYR;
  volatile uint32_t OPTKEYR;
  volatile uint32_t SR;
  volatile uint32_t CR;
  volatile uint32_t AR;
  volatile uint32_t RESERVED;
  volatile uint32_t OBR;
  volatile uint32_t WRPR;
} FLASH_TypeDef;

#define GPIOC                 ((GPIO_TypeDef *)GPIOC_BASE)
#define RCC                   ((RCC_TypeDef *)RCC_BASE)
#define FLASH                 ((FLASH_TypeDef *)FLASH_R_BASE)

/* RCC_CR */
#define RCC_CR_HSION          (1u << 0)
#define RCC_CR_HSIRDY         (1u << 1)

/* RCC_CFGR */
#define RCC_CFGR_SW_HSI       (0u << 0)
#define RCC_CFGR_SWS_HSI      (0u << 2)
#define RCC_CFGR_SWS          (3u << 2)

/* RCC_APB2ENR */
#define RCC_APB2ENR_IOPCEN    (1u << 4)

/* GPIOC->CRH 中引脚 13 的 MODE/CNF（位 23:20） */
#define GPIO_CRH_MODE13_Pos   20u
#define GPIO_CRH_MODE13_Msk   (0x3u << GPIO_CRH_MODE13_Pos)
#define GPIO_CRH_CNF13_Pos    22u
#define GPIO_CRH_CNF13_Msk    (0x3u << GPIO_CRH_CNF13_Pos)

#ifdef __cplusplus
}
#endif

#endif /* STM32F103XB_H */
