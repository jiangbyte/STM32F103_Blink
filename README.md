# STM32F103_Blink

面向 **STM32F103C8T6（Blue Pill）** 的最小闪灯工程：寄存器级点亮板载 **PC13 LED**，用于确认工具链、ST-LINK 与芯片基本运行。

支持两套工作流：

- **CMake + Arm GNU GCC + OpenOCD**（`cmake-gcc/`）— 推荐，本 README 以 Debian / Linux 为主
- **Keil MDK + ARM Compiler 6**（`MDK-ARM/`）

## 特性

- 不依赖 CubeMX / HAL，仅精简 CMSIS 与寄存器定义
- 时钟使用内部 **HSI 8MHz**
- 目标器件：STM32F103C8（Flash 64KB / RAM 20KB）
- 调试下载：ST-LINK（SWD / HLA）
- PC13 LED 低电平点亮；节奏约 **1 秒长亮 → 快闪 3 次** 循环

## 目录

```text
STM32F103_Blink/
  README.md
  User/
    main.c                  # PC13 闪烁
    stm32f103xb.h
  CMSIS/
    core_cm3.h
  cmake-gcc/
    CMakeLists.txt
    toolchain-arm-none-eabi.cmake
    startup_stm32f103xb.s   # GNU 汇编启动文件
    STM32F103C8Tx_FLASH.ld
    openocd.cfg             # ST-LINK + stm32f1x（hla_swd）
  MDK-ARM/
    Blink.uvprojx           # Keil 工程入口
    startup_stm32f103xb.s
    Blink.sct
  .vscode/                  # Cortex-Debug 一键调试
```

## 方式一：CMake + GCC + OpenOCD（Debian / Linux）

### 安装工具

```bash
sudo apt-get update
sudo apt-get install -y \
  cmake ninja-build \
  gcc-arm-none-eabi binutils-arm-none-eabi \
  libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib \
  openocd gdb-multiarch
```

确认：

```bash
cmake --version
arm-none-eabi-gcc --version
openocd -v
ninja --version
gdb-multiarch --version
```

用户需能访问 ST-LINK USB 设备（通常在 `plugdev` 组）。插上后可用：

```bash
lsusb | grep -i 0483
```

### 编译

在仓库根目录：

```bash
cmake -S cmake-gcc -B cmake-gcc/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$PWD/cmake-gcc/toolchain-arm-none-eabi.cmake" \
  -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-gcc/build
```

产物：`cmake-gcc/build/Blink.elf`、`Blink.hex`、`Blink.bin`

### 烧录（ST-LINK）

```bash
openocd -f cmake-gcc/openocd.cfg \
  -c "program cmake-gcc/build/Blink.elf verify reset exit"
```

成功时应看到 `Verified OK` 与 `Resetting Target`。

说明：工程内 `openocd.cfg` 对 ST-LINK/V2 使用 `transport select hla_swd`（不要写成裸的 `swd`）。

### Cursor / VS Code 图形化调试

1. 安装扩展：**Cortex-Debug**（`marus25.cortex-debug`）、建议同时装 **CMake Tools**
2. 用 Cursor / VS Code 打开本仓库根目录
3. 接好 ST-LINK 与板子
4. Run and Debug → 选 **Blink (OpenOCD + ST-LINK)** → F5

会先编 Debug 版，再经 OpenOCD 下载并停在 `main`。Debian 下 GDB 使用 `gdb-multiarch`。

## 方式二：Keil MDK（Windows）

1. 用 Keil μVision5 打开 `MDK-ARM/Blink.uvprojx`
2. Rebuild
3. 经 ST-LINK 连接板子后 Download

## Windows 下的 CMake / GCC（可选）

也可用 winget 安装工具链后，在仓库根目录编译与烧录（路径按本机调整）：

```text
winget install --id Kitware.CMake -e
winget install --id Arm.GnuArmEmbeddedToolchain -e
winget install --id xpack-dev-tools.openocd-xpack -e
winget install --id Ninja-build.Ninja -e
```

```bash
cmake -S cmake-gcc -B cmake-gcc/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$PWD/cmake-gcc/toolchain-arm-none-eabi.cmake" \
  -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-gcc/build
openocd -f cmake-gcc/openocd.cfg \
  -c "program cmake-gcc/build/Blink.elf verify reset exit"
```

## 许可

[MIT](./LICENSE)
