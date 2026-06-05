#include "../../hal/power_hal.h"
#include "board.h"
#include <Arduino.h>

// 本板无 PMU、无电池、无独立 PWR 按钮，全部返回中性值。
// BOARD_HAS_BATTERY=0 → ui.cpp 隐藏电池指示器。
// PWR 手势（splash 换动画 / usage 调亮度 / 配对）本板暂不可用：仅 BOOT 键
// 且已分配给 touch.cpp 切页。归属：以后若接入额外按键，可在此映射
// power_hal_pwr_pressed 等以驱动 splash_next()/brightness_cycle()。

void power_hal_init(void) {}
void power_hal_tick(void) {}

int  power_hal_battery_pct(void) { return -1; }
bool power_hal_is_charging(void) { return false; }
// 本板 USB-A 直插恒供电。返回 true 让 idle（IDLE_SLEEP_WHEN_CHARGING=false）认为一直插电，
// 永不息屏=屏幕常亮。若以后想让它能息屏，把这里改回 false 或改 idle_cfg.h 的 IDLE_TIMEOUT_MS。
bool power_hal_is_vbus_in(void)  { return true; }
bool power_hal_pwr_pressed(void) { return false; }
bool power_hal_pwr_long_pressed(void) { return false; }
bool power_hal_pwr_released(void) { return false; }
