;******************************************************************************
; STM32F103xB（Cortex-M3）最小启动文件 — Keil MDK / armasm 或 armasm6
;******************************************************************************

Stack_Size      EQU     0x00000400
Heap_Size       EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

; 中断向量表
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; 栈顶
                DCD     Reset_Handler             ; 复位
                DCD     NMI_Handler               ; NMI
                DCD     HardFault_Handler         ; 硬件错误
                DCD     MemManage_Handler         ; 存储器管理
                DCD     BusFault_Handler          ; 总线错误
                DCD     UsageFault_Handler        ; 用法错误
                DCD     0                         ; 保留
                DCD     0                         ; 保留
                DCD     0                         ; 保留
                DCD     0                         ; 保留
                DCD     SVC_Handler               ; SVCall
                DCD     DebugMon_Handler          ; 调试监视
                DCD     0                         ; 保留
                DCD     PendSV_Handler            ; PendSV
                DCD     SysTick_Handler           ; SysTick
                ; 外部中断（F103 中等密度子集）
                DCD     Default_Handler           ; WWDG
                DCD     Default_Handler           ; PVD
                DCD     Default_Handler           ; TAMPER
                DCD     Default_Handler           ; RTC
                DCD     Default_Handler           ; FLASH
                DCD     Default_Handler           ; RCC
                DCD     Default_Handler           ; EXTI0
                DCD     Default_Handler           ; EXTI1
                DCD     Default_Handler           ; EXTI2
                DCD     Default_Handler           ; EXTI3
                DCD     Default_Handler           ; EXTI4
                DCD     Default_Handler           ; DMA1_Channel1
                DCD     Default_Handler           ; DMA1_Channel2
                DCD     Default_Handler           ; DMA1_Channel3
                DCD     Default_Handler           ; DMA1_Channel4
                DCD     Default_Handler           ; DMA1_Channel5
                DCD     Default_Handler           ; DMA1_Channel6
                DCD     Default_Handler           ; DMA1_Channel7
                DCD     Default_Handler           ; ADC1_2
                DCD     Default_Handler           ; USB_HP_CAN1_TX
                DCD     Default_Handler           ; USB_LP_CAN1_RX0
                DCD     Default_Handler           ; CAN1_RX1
                DCD     Default_Handler           ; CAN1_SCE
                DCD     Default_Handler           ; EXTI9_5
                DCD     Default_Handler           ; TIM1_BRK
                DCD     Default_Handler           ; TIM1_UP
                DCD     Default_Handler           ; TIM1_TRG_COM
                DCD     Default_Handler           ; TIM1_CC
                DCD     Default_Handler           ; TIM2
                DCD     Default_Handler           ; TIM3
                DCD     Default_Handler           ; TIM4
                DCD     Default_Handler           ; I2C1_EV
                DCD     Default_Handler           ; I2C1_ER
                DCD     Default_Handler           ; I2C2_EV
                DCD     Default_Handler           ; I2C2_ER
                DCD     Default_Handler           ; SPI1
                DCD     Default_Handler           ; SPI2
                DCD     Default_Handler           ; USART1
                DCD     Default_Handler           ; USART2
                DCD     Default_Handler           ; USART3
                DCD     Default_Handler           ; EXTI15_10
                DCD     Default_Handler           ; RTCAlarm
                DCD     Default_Handler           ; USBWakeUp
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; 复位入口：SystemInit -> __main -> main
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

; 默认异常处理（死循环，可按需改写）
NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT  Default_Handler           [WEAK]
                B       .
                ENDP

                ALIGN

; 用户栈与堆初始区域
                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap\
                LDR     R0, = Heap_Mem
                LDR     R1, = (Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem + Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

                END
