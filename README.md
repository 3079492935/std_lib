# std_lib

[简体中文](#简体中文) | [English](#english)

## 简体中文

面向 STM32 项目的嵌入式 C 驱动库。

### 驱动结构

| 目录 | 职责 |
| --- | --- |
| `std_CC1101/`、`std_BMI270/`、`std_ILI9341/`、`std_LIS2MDLTR/` | 按器件组织的驱动源码和头文件。 |
| `configs/<板级配置>/` | `std_bus.h` 提供 SPI、I²C、GPIO 等接口，`std_busconfig.h` 和 `std_bus.c` 将逻辑编号映射到 STM32 HAL 外设和引脚。 |
| `interface/` | 定时器、UART 和射频数据包辅助代码。 |
| `demo/`、`arithmetic/` | BMI270 使用示例及其使用的校准、四元数等算法。 |

### 运行逻辑

1. **板级绑定。** 父工程先初始化 STM32 外设，再向驱动传入逻辑 SPI 和片选编号。器件驱动负责命令、寄存器和数据格式，`std_bus.h` 将总线操作转发给板级配置里的 HAL 实现。
2. **定时器推进状态。** `STD_TIM_SET_TINK()` 将状态变量置为等待状态，把目标状态和延时加入定时任务表。定时器中断调用 `STD_TIM_IRQ()` 写入到期任务的目标状态，应用循环根据新状态执行下一步器件操作。
3. **CC1101 收发。** 应用反复调用 `STD_CC1101_init()` 推进状态机。初始化依次绑定 SPI、切换片选、复位芯片、逐项写入寄存器表，再设置地址和同步字。接收时检查芯片状态和 RX FIFO，校验长度后将变长包写入 `rf->datas`。发送时以缓冲区首字节为长度，将数据写入 TX FIFO，再轮询芯片状态等待发送完成。检测到接收溢出时，驱动会清空 RX FIFO 并重新进入接收。
4. **BMI270 采样。** `STD_BMI270_Init()` 配置传感器并读取量程，得到加速度和角速度的换算系数。`STD_BMI270_get_acc_gyro()` 批量读取并解析三轴加速度、三轴角速度和传感器时间戳。`demo()` 根据定时状态触发采样，再进行量程换算、陀螺仪校准和四元数姿态计算。

### 跨芯片移植

现有板级配置基于 STM32 HAL。移植到其他 STM32 型号时，可从 `configs/` 下的现有配置入手，调整 SPI、I²C、GPIO、UART 和定时器所用的外设实例及引脚映射。移植到其他 MCU 平台时，需要用目标平台的 SDK 实现驱动所用的 `STD_*` 接口，并调整父工程的构建依赖。

只要总线时序、片选和定时器行为符合驱动预期，同一外设的寄存器操作和状态机通常可以复用。换用不同的传感器或射频芯片时，还需要按新器件的寄存器和通信协议适配驱动。

### 构建

现有 CMake 配置需要父工程提供 `stm32cubemx` 目标，并通过 `STD_LIB_CONFIG` 选择 `configs/` 下的板级配置。`demo/` 中的示例源码会随库一起编译。

当前 CMake 目标包含 CC1101 和 BMI270。ILI9341 和 LIS2MDLTR 源码依赖另一套平台配置，接入时需要单独适配。

### 许可证

本项目采用 Mozilla Public License 2.0（MPL 2.0）。对外分发包含本库的固件时，须向接收者提供本库受 MPL 覆盖的对应源码，包括修改。独立编写且未包含 MPL 代码的应用文件可以保持闭源。详见 [LICENSE](LICENSE)。

BMI270 初始化配置数据来自 [Bosch Sensortec BMI270 Sensor API](https://github.com/boschsensortec/BMI270_SensorAPI/blob/master/bmi270.c)，沿用 BSD-3-Clause。版权和许可全文见 [bmi270_config.inc](std_BMI270/bmi270_config.inc)。分发包含该数据的固件时，须在随附文档或其他材料中保留该声明。

## English

An embedded C driver library for STM32 projects.

### Driver structure

| Directory | Responsibility |
| --- | --- |
| `std_CC1101/`, `std_BMI270/`, `std_ILI9341/`, `std_LIS2MDLTR/` | Device-specific driver sources and headers. |
| `configs/<board-config>/` | `std_bus.h` exposes interfaces for SPI, I²C, and GPIO. `std_busconfig.h` and `std_bus.c` map logical IDs to STM32 HAL peripherals and pins. |
| `interface/` | Timer, UART, and RF packet helpers. |
| `demo/`, `arithmetic/` | A BMI270 example and the calibration and quaternion routines it uses. |

### Runtime flow

1. **Board binding.** The parent project initializes the STM32 peripherals and passes logical SPI and chip-select IDs to the driver. The device driver handles commands, registers, and data formats. `std_bus.h` forwards bus operations to the board-specific HAL implementation.
2. **Timer-driven transitions.** `STD_TIM_SET_TINK()` puts the state variable into a waiting state and queues the target state with a delay. The timer interrupt calls `STD_TIM_IRQ()` to write the target state when the task expires. The application loop uses that state to run the next device operation.
3. **CC1101 transmit and receive.** The application calls `STD_CC1101_init()` repeatedly to advance the state machine. Initialization binds SPI, toggles chip select, resets the chip, writes the register table one entry at a time, and sets the address and sync word. On receive, the driver checks the chip state and RX FIFO, validates the packet length, and copies the variable-length packet into `rf->datas`. On transmit, it uses the first buffer byte as the length, writes the packet to the TX FIFO, and polls the chip until transmission completes. On receive overflow, it flushes the RX FIFO and re-enters receive mode.
4. **BMI270 sampling.** `STD_BMI270_Init()` configures the sensor and reads its range settings to derive accelerometer and gyroscope scale factors. `STD_BMI270_get_acc_gyro()` reads and decodes three-axis acceleration, three-axis angular velocity, and the sensor timestamp in one batch. The `demo()` function samples according to the timer state, then scales the readings, calibrates the gyroscope, and calculates orientation using quaternions.

### Porting to other MCUs

The current board configurations use STM32 HAL. To port to another STM32 model, start from an existing configuration under `configs/` and update the peripheral instances and pin mappings for SPI, I²C, GPIO, UART, and timers. On another MCU platform, use its SDK to implement the `STD_*` interfaces used by the drivers and adjust the parent project's build dependencies.

For the same external device, the register operations and state machine can generally be reused if bus timing, chip select, and timer behavior match the driver's expectations. Switching to a different sensor or radio chip also requires adapting the driver to the new device's registers and communication protocol.

### Build

The CMake configuration expects the parent project to provide a `stm32cubemx` target. Set `STD_LIB_CONFIG` to select a board configuration under `configs/`. The example sources in `demo/` are compiled into the library.

The current CMake target includes CC1101 and BMI270. The ILI9341 and LIS2MDLTR sources use a separate set of platform configuration interfaces and require additional integration.

### License

This project is licensed under the Mozilla Public License 2.0 (MPL 2.0). When distributing firmware that includes this library, make the corresponding MPL-covered source, including modifications, available to recipients. Independently written application files that contain no MPL-covered code may remain proprietary. See [LICENSE](LICENSE).

The BMI270 initialization data comes from the [Bosch Sensortec BMI270 Sensor API](https://github.com/boschsensortec/BMI270_SensorAPI/blob/master/bmi270.c) and retains its BSD-3-Clause license. See [bmi270_config.inc](std_BMI270/bmi270_config.inc) for the copyright and full license notice. When distributing firmware containing this data, reproduce the notice in the accompanying documentation or other materials.
