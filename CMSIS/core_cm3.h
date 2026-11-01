/**
 * 本闪灯工程用的精简 Cortex-M3 定义。
 * 兼容 Keil ARMCC v5 / ARMClang（AC6）。
 */
#ifndef CORE_CM3_H
#define CORE_CM3_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __STATIC_INLINE
#define __STATIC_INLINE static __inline
#endif

/* NVIC / SCB（仅保留本工程可能用到的子集） */
typedef struct {
  volatile uint32_t ISER[8];
  uint32_t RESERVED0[24];
  volatile uint32_t ICER[8];
  uint32_t RESERVED1[24];
  volatile uint32_t ISPR[8];
  uint32_t RESERVED2[24];
  volatile uint32_t ICPR[8];
  uint32_t RESERVED3[24];
  volatile uint32_t IABR[8];
  uint32_t RESERVED4[56];
  volatile uint8_t  IP[240];
  uint32_t RESERVED5[644];
  volatile uint32_t STIR;
} NVIC_Type;

typedef struct {
  volatile const uint32_t CPUID;
  volatile uint32_t ICSR;
  volatile uint32_t VTOR;
  volatile uint32_t AIRCR;
  volatile uint32_t SCR;
  volatile uint32_t CCR;
  volatile uint8_t  SHP[12];
  volatile uint32_t SHCSR;
  volatile uint32_t CFSR;
  volatile uint32_t HFSR;
  volatile uint32_t DFSR;
  volatile uint32_t MMFAR;
  volatile uint32_t BFAR;
  volatile uint32_t AFSR;
} SCB_Type;

#define SCS_BASE            (0xE000E000UL)
#define NVIC_BASE           (SCS_BASE + 0x0100UL)
#define SCB_BASE            (SCS_BASE + 0x0D00UL)

#define NVIC                ((NVIC_Type *)NVIC_BASE)
#define SCB                 ((SCB_Type *)SCB_BASE)

#if defined(__CC_ARM) /* ARM Compiler 5：__nop 等为编译器内建 */
  #ifndef __NOP
  #define __NOP() __nop()
  #endif
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /* ARMClang */
  __STATIC_INLINE void __NOP(void) { __asm volatile ("nop"); }
  __STATIC_INLINE void __disable_irq(void) { __asm volatile ("cpsid i" ::: "memory"); }
  __STATIC_INLINE void __enable_irq(void) { __asm volatile ("cpsie i" ::: "memory"); }
#else
  __STATIC_INLINE void __NOP(void) { __asm volatile ("nop"); }
  __STATIC_INLINE void __disable_irq(void) { __asm volatile ("cpsid i" ::: "memory"); }
  __STATIC_INLINE void __enable_irq(void) { __asm volatile ("cpsie i" ::: "memory"); }
#endif

#ifdef __cplusplus
}
#endif

#endif /* CORE_CM3_H */
