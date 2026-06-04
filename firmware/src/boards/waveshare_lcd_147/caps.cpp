#include "../../hal/board_caps.h"
#include "board.h"

// 运行时板描述；字段值全部由 board.h 的宏驱动（与 template/caps.cpp 同构）。
// button_count = 1（仅 BOOT）。注意：BOOT 的 input_hal_is_held 在 input.cpp
// 返回 false，BOOT 实际通过 touch.cpp 当作切页"假触摸"，不是 HID 按钮。
static const BoardCaps caps = {
    .name = BOARD_NAME,
    .width = LCD_WIDTH,
    .height = LCD_HEIGHT,
    .button_count = (uint8_t)(1 + BOARD_HAS_SECONDARY_BUTTON),
    .has_rotation = (bool)BOARD_HAS_ROTATION,
    .has_battery  = (bool)BOARD_HAS_BATTERY,
    .has_imu      = (bool)BOARD_HAS_IMU,
};

const BoardCaps& board_caps(void) { return caps; }
