#include "../../hal/touch_hal.h"
#include "board.h"
#include <Arduino.h>

// 本板无触摸屏。用 BOOT 键(IO0) 翻译成"屏幕中心假触摸"驱动切页。
// 为什么这样做：ui.cpp 的 global_click_cb 切换 splash<->usage 只认 LVGL CLICKED
//   事件，而该事件来自 main.cpp::my_touch_cb -> touch_hal_read 的 pressed。让本函数
//   在 BOOT 按下时返回 pressed=true + 屏幕中心坐标，即可不改 shared code 实现切页。
// 归属：BOOT(IO0) 专用于此切页；input.cpp 的 PRIMARY 因此返回 false 避免抢同一 GPIO。

void touch_hal_init(void) {
    pinMode(BTN_BACK_GPIO, INPUT_PULLUP);
}

void touch_hal_read(uint16_t* x, uint16_t* y, bool* pressed) {
    *x = LCD_WIDTH / 2;
    *y = LCD_HEIGHT / 2;
    *pressed = (digitalRead(BTN_BACK_GPIO) == LOW);  // 上拉，低电平=按下
}
