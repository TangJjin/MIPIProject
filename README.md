# STM32H747 MIPI LCD UI 工程

这是一个基于 **STM32H747XIHx** 的双核嵌入式显示工程。工程使用 Cortex-M7 驱动 MIPI-DSI LCD、外部 SDRAM、触摸屏和 LVGL 图形界面，Cortex-M4 保留为双核启动后的协处理器运行框架。

## 功能概览

- STM32H747 双核启动：Cortex-M7 完成系统初始化后通过硬件信号量唤醒 Cortex-M4
- MIPI-DSI 视频模式驱动 LCD，面板初始化代码针对 ILI9881C10
- LTDC + DMA2D + 外部 SDRAM 帧缓冲
- LVGL 图形界面和 Guider 生成的页面、控件、事件代码
- I2C 触摸屏，支持自动探测 GT9XXX 和 FT5206 控制器
- FreeRTOS 任务：LVGL 刷新任务和基础 LED 测试任务
- USART1 调试输出及 DMA 接收环形缓冲
- Windows/MinGW 下的 LVGL 模拟器工程

## 主要参数

| 项目 | 当前配置 |
| --- | --- |
| MCU | STM32H747XIHx（Cortex-M7 + Cortex-M4） |
| LCD 控制器 | ILI9881C10 |
| MIPI-DSI | 双数据 Lane，Burst Video Mode |
| 物理显示区域 | 1280 × 800 |
| 帧缓冲排列 | 800 × 1280，RGB565 |
| 帧缓冲地址 | `0xD0000000`（外部 SDRAM） |
| 外部 SDRAM | 32 MiB，FMC Bank 2 |
| 触摸接口 | I2C2，支持 GT9XXX / FT5206 |
| 调试串口 | USART1，115200 baud |
| 图形库 | LVGL（仓库内 `lvgl/`） |
| IDE/工具链 | Keil MDK-ARM，STM32H7xx DFP 2.7.0 |

> 显示方向在 LCD 物理坐标和帧缓冲坐标之间旋转 90 度，`APP/LCD/bsp_mipi_lcd.c` 已封装矩形填充时的坐标转换。具体屏幕时序、GPIO 和触摸芯片地址仍需与实际硬件一致。

## 目录结构

```text
.
├── APP/
│   ├── LCD/       MIPI LCD、ILI9881C10 初始化、DMA2D 绘图
│   ├── LED/       LED 控制
│   ├── RTOS/      FreeRTOS 任务
│   ├── SDRAM/     FMC SDRAM 初始化和读写
│   ├── SYSTEM/    BSP 汇总头文件和系统依赖
│   ├── TOUCH/     GT9XXX / FT5206 触摸驱动
│   ├── UART/      USART 辅助和环形缓冲
│   └── UI/        LVGL 应用入口及 Guider 生成代码
├── CM7/           Cortex-M7 的 CubeMX 生成代码和启动入口
├── CM4/           Cortex-M4 的 CubeMX 生成代码和启动入口
├── Common/        双核共用启动代码
├── Drivers/       STM32 HAL、CMSIS 和设备头文件
├── FreeRTOS/      FreeRTOS 内核及 ARM Cortex-M7 移植
├── lvgl/          LVGL 源码
├── MDK-ARM/       Keil 工程、启动文件和 scatter 文件
├── STM32H747_UI/  UI 设计工程及 LVGL Windows 模拟器
└── MIPIProject.ioc  STM32CubeMX 配置文件
```

## 硬件要求

1. STM32H747XIHx 开发板或兼容目标板。
2. 连接到 MIPI-DSI 的 1280 × 800 LCD 面板，面板初始化命令表需匹配 ILI9881C10。
3. 外部 SDRAM，且映射到 `0xD0000000`；帧缓冲需要约 2 MiB（800 × 1280 × 2）。
4. I2C2 触摸控制器。驱动会依次尝试 GT9XXX 地址 `0x14` 和 FT5206 地址 `0x38`（代码中为 HAL 左移后的地址）。
5. 使用 USART1 连接串口工具查看启动日志。

## 使用 Keil 编译和下载

### 环境准备

- 安装 Keil MDK-ARM / µVision。
- 安装与工程匹配的 `Keil.STM32H7xx_DFP.2.7.0`（或确认兼容版本）。
- 安装 ST-LINK 驱动，并准备支持双核下载的调试器。

### 编译

1. 打开 `MDK-ARM/MIPIProject.uvprojx`。
2. 在工程中分别选择 `MIPIProject_CM7` 和 `MIPIProject_CM4` target。
3. 先编译 CM7，再编译 CM4，确认两个 target 均无错误。
4. 生成的中间文件位于 `MDK-ARM/MIPIProject_CM7/` 和 `MDK-ARM/MIPIProject_CM4/`；这些构建产物已在 `.gitignore` 中排除。

### 下载和启动

将 CM7、CM4 镜像按调试器支持的双核下载方式写入芯片。上电后，CM4 会先进入等待状态；CM7 完成时钟、FMC、SDRAM 和外设初始化后，通过 HSEM 释放 CM4。CM7 随后启动 DSI/LTDC/DMA2D、LCD、触摸和 LVGL，并启动 FreeRTOS 调度器。

串口正常时可看到类似日志：

```text
SDRAM init success
DSI/LTDC/DMA2D init done
Touch controller: GT9XXX
Touch init success
```

触摸控制器未检测到或 SDRAM 初始化失败时，程序会进入 `Error_Handler()`，请优先检查供电、复位脚、I2C 连接和 FMC 时序。

## LVGL Windows 模拟器

模拟器位于 `STM32H747_UI/UI_Project/lvgl-simulator/`，用于在没有目标板时预览部分 UI。当前构建说明依赖 MinGW 和 `mingw32-make`。

```powershell
cd STM32H747_UI/UI_Project/lvgl-simulator
mingw32-make -j8
mingw32-make run
```

如果系统找不到 `mingw32-make`，请将 MinGW 的 `bin` 目录加入 `PATH`。模拟器所需的 SDL2 动态库应位于模拟器的 `modules/SDL2` 目录或系统可搜索路径中。模拟器与真实硬件使用的显示驱动、触摸驱动不同，模拟器通过 UI 工程配置运行，不代表目标板已经完成硬件验证。

## 修改 UI

- 设计工程：`STM32H747_UI/UI_Project/UI_Project.guiguider`
- 目标板使用的生成代码：`APP/UI/src/generated/`
- 自定义代码：`APP/UI/src/custom/`
- LVGL 配置扩展：`APP/UI/src/custom/lv_conf_ext.h`

重新生成 UI 后，请检查生成代码是否覆盖了已有的手写修改，并确认字体、图片资源的体积不会超出目标存储空间。

## 关键源码入口

- `CM7/Core/Src/main.c`：M7 启动流程、外设初始化、LVGL 和 FreeRTOS 启动
- `CM4/Core/Src/main.c`：M4 等待 HSEM 并进入运行循环
- `APP/LCD/bsp_mipi_lcd.c`：LCD 复位、DSI 命令、背光、清屏和矩形绘制
- `APP/TOUCH/bsp_touch.c`：触摸控制器探测和坐标读取
- `APP/RTOS/bsp_rtos.c`：LVGL 任务和测试任务
- `MIPIProject.ioc`：STM32CubeMX 外设、引脚和时钟配置

## 常见问题

### LCD 背光亮但没有图像

检查外部 SDRAM 初始化是否成功、帧缓冲地址是否为 `0xD0000000`，以及 DSI/LTDC 时序是否与 LCD 面板规格一致。可以在 `CM7/Core/Src/main.c` 中临时调用 `LCD_Test_Corners()` 验证四角坐标和颜色。

### 触摸无响应

检查 I2C2 的 PB10/PB11、`CTP_RST` 连接和触摸芯片供电。串口日志会报告探测到的控制器类型；不同面板若使用其他控制器，需要扩展 `APP/TOUCH/bsp_touch.c`。

### 重新打开 CubeMX 后代码异常

`MIPIProject.ioc` 是外设配置源文件。重新生成代码前请备份 `USER CODE` 区域，并复核 DSI、LTDC、FMC、DMA2D 以及双核启动配置；LCD 初始化命令表和 BSP 手写代码不应被 CubeMX 覆盖。

## Git 提交建议

仓库已经提供 `.gitignore`，会排除 Keil 编译中间文件、模拟器构建目录和常见 IDE 临时文件。通常只提交源码、配置文件、UI 工程和必要的资源，不建议提交 `.axf`、`.o`、`.dll` 等生成物。

## 许可证

本仓库当前未提供根目录统一的 `LICENSE` 文件。仓库内的 STM32 HAL、CMSIS、FreeRTOS、LVGL 及字体/第三方库各自带有许可证或版权声明；发布到 GitHub 前，请根据实际来源补充项目许可证，并保留第三方组件的原始许可文件。

