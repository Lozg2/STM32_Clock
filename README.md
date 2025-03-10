# STM32 通过 UART 实现 RTC 和时钟控制

本项目展示了如何使用 STM32 微控制器通过 UART（通用异步收发传输器）与 PC 进行通信，设置和读取实时时钟（RTC），并根据特定时间控制 LED。

## 概述

本项目包含以下功能：

1. **UART 通信**：STM32 通过 UART 与 PC 通信，接收命令并发送响应。
2. **RTC（实时时钟）**：使用 RTC 来跟踪时间。可以通过 UART 命令设置和读取时间。
3. **时钟控制**：通过 UART 设置特定时间，控制连接到 GPIO 引脚 PA2 的 LED。

## 硬件要求

- STM32F10x 微控制器
- UART 转 USB 转换器（用于连接 PC）
- 连接到 GPIO 引脚 PA2 的 LED
- 基本的 STM32 开发板

## 软件要求

- STM32 标准外设库
- 串口终端软件（如 PuTTY、Tera Term）用于 PC 通信

## 项目结构

- **main.c**：主应用程序代码，处理 UART 通信、RTC 配置和 LED 控制。
- **RTC.h/c**：用于初始化和读取 RTC 的函数。
- **Serial.h/c**：用于初始化 UART 和处理串口通信的函数。

## UART 命令

通过 UART 支持以下命令：

1. **SET_TIME**：设置 RTC 时间。
   - 格式：`SET_TIME YYYYMMDDHHMMSS`
   - 示例：`SET_TIME 20231025123000` 将时间设置为 2023 年 10 月 25 日 12:30:00。

2. **SET_CLOCK**：设置控制 LED 的时钟时间。
   - 格式：`SET_CLOCK HHMMSS`
   - 示例：`SET_CLOCK 123000` 将时钟设置为 12:30:00。

3. **GET_TIME**：读取当前 RTC 时间。
   - 命令：`GET_TIME`
   - 响应：当前 RTC 时间，格式为 `YYYYMMDDHHMMSS`。

4. **GET_CLOCK**：读取当前时钟时间。
   - 命令：`GET_CLOCK`
   - 响应：当前时钟时间，格式为 `HHMMSS`。

## LED 控制

- 连接到 GPIO 引脚 PA2 的 LED 会在指定的时钟时间点亮，并在 30 秒后熄灭。
- LED 的状态（开/关）通过 UART 发送。

## 使用方法

1. 使用 UART 转 USB 转换器将 STM32 连接到 PC。
2. 在 PC 上打开串口终端软件（如 PuTTY），并配置为适当的波特率（如 9600）。
3. 发送上述命令以设置或读取时间，并控制 LED。

## 示例

1. 设置 RTC 时间：
   ```
   SET_TIME 20231025123000
   ```
   响应：RTC 时间设置为 2023 年 10 月 25 日 12:30:00。

2. 设置时钟时间：
   ```
   SET_CLOCK 123000
   ```
   响应：时钟时间设置为 12:30:00。

3. 读取 RTC 时间：
   ```
   GET_TIME
   ```
   响应：`20231025123000`

4. 读取时钟时间：
   ```
   GET_CLOCK
   ```
   响应：`123000`

5. LED 将在 12:30:00 点亮，并在 12:30:30 熄灭。

## 代码说明

- **RTC_Init()**：初始化 RTC 模块。
- **Serial_Init()**：初始化 UART 通信。
- **RTC_ReadTime()**：读取当前 RTC 时间。
- **Serial_SendPacket()**：通过 UART 发送数据包。
- **GPIO_SetBits() / GPIO_ResetBits()**：控制 GPIO 引脚的高低电平，用于控制 LED。

## 注意事项

- 确保 UART 的波特率与代码中设置的波特率一致。
- 时钟时间的设置需要考虑时区差异（代码中已处理为 UTC+8）。

## 贡献

欢迎提交问题和拉取请求以改进本项目。

## 许可证

本项目采用 MIT 许可证。详情请参阅 [LICENSE](LICENSE) 文件。
