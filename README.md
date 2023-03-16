# 2022 Robogame 天铭宝宝向前冲队 电控代码

## 代码组织

- 单片机: STM32F429IGT6
- 相机模块: M5Stack UnitV2
- 代码生成: STM32CubeMX
- 构建系统: Unix Makefile
- 语言: C/C++



## 代码结构

```text
|-- Core
|   |-- Inc
|   `-- Src
|-- Drivers
`-- User
```

- Core: 由 STM32CubeMX 生成的代码。其中的 main.c 为主函数，包含了所有的控制循环。
- Drivers: 由 STM32CubeMX 生成的驱动代码。
- User: 所有的外设代码以及一些控制流程代码。（在下方进行介绍）



## 外设代码: User

列表如下。带星号的文件是几乎原样从麻神的仓库中 copy 的。

- `printf.c/h` 串口 printf 的 minimal 实现
- `DR16.cpp/hpp`**\*** 串口遥控器实现



- `Chassis.cpp/hpp` 整车底盘和速度解算
- `Steer.cpp/hpp`**\*** 舵机控制（主要是机械爪的开合）
- `motor.cpp/hpp`**\*** 电机控制
- `PID.cpp/hpp`**\*** PID 控制器实现



- `gyro.cpp/hpp` 陀螺仪相关高级函数（没用上）
- `mpu6050.cpp/hpp` 陀螺仪底层驱动（没用上）



- `search.cpp/hpp` **此文件实现了所有的车辆运行逻辑，包括各个流程的处理逻辑。在下方进行介绍。**



**注：本项目采用了运行 Linux 的 M5Stack UnitV2 摄像头模块进行视觉识别。其与单片机之间的沟通协议非常简单，在此简要介绍：**

（**TODO：串口协议**）



## 逻辑代码: User/search.cpp

提供的函数及其功能如下：

| 函数名         | 简介                                       |
| -------------- | ------------------------------------------ |
| actuator_up    | 抬升机械爪                                 |
| actuator_down  | 降低机械爪                                 |
| actuator_stop  | 停止机械爪运动                             |
| Nudge1         | 做角度微调（右侧）                         |
| Nudge2         | 做角度微调（左侧）                         |
| TurnAtCrossing | 在黑线十字路口处掉头                       |
| NextLane       | 前往下一个交叉路口                         |
| PrevLane       | 前往前一个交叉路口                         |
| GoPickup       | 靠近取壶区并取壶                           |
| GoPutdown      | 靠近取壶区并放下壶                         |
| backoff        | 离开取壶区                                 |
| GoPickupBack   | 靠近高难度取壶区并取壶                     |
| backoffBack    | 离开高难度取壶区                           |
| GoForward      | 设置速度为直行前进                         |
| GoBackward     | 设置速度为直行后退                         |
| GoRight        | 设置速度为直行右侧横向移动                 |
| GoLeft         | 设置速度为直行左侧横向移动                 |
| RRotate        | 右转                                       |
| AdjustR        | 在速度上叠加一个向右转角速度               |
| AdjustL        | 在速度上叠加一个向左转角速度               |
| Stop           | 停止                                       |
| Run1           | 巡线直走（一次循环）                       |
| Run_Forward    | 巡线直走                                   |
| Fix            | 在黑线交叉处调整车辆中心位置               |
| Stage1         | 走出开始区                                 |
| Stage2         | 在第一个十字路口处向右直行                 |
| Stage3         | 在赛道底端停止，并转向调整                 |
| Stage4         | 完成整个冰壶的识别、取、放、调整、发射任务 |