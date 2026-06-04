#include "board.h"
#include <Arduino.h>

// 本板无 I2C 外设（无触摸/PMU/IMU/IO 扩展），无需 Wire.begin 或扩展器释放。
// 保留空实现以满足 main.cpp 的 extern "C" void board_init(void) 调用契约
// （main.cpp 在 setup() 最先调用它，早于 display/touch 等 HAL 初始化）。
extern "C" void board_init(void) {
    // 无操作。
}
