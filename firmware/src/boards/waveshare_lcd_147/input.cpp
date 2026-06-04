#include "../../hal/input_hal.h"
#include "board.h"
#include <Arduino.h>

// 本板物理按键只有 BOOT(IO0)，已分配给 touch.cpp 假触摸切页。
// 串口数据通道不需要 BLE HID（Space / Shift+Tab），故两个按钮都返回 false，
// 避免与切页抢同一个 GPIO。main.cpp 中相关 ble_keyboard_* 调用在无 BLE 连接时静默失败。
// 归属：若以后确认有额外可用按键（Btn 待核实），再在此映射 PRIMARY/SECONDARY。

void input_hal_init(void) {
    // BOOT 的 pinMode 已在 touch_hal_init 设置；本板无其它按键需初始化。
}

bool input_hal_is_held(InputButton btn) {
    (void)btn;
    return false;
}
