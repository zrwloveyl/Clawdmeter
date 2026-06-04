#include "../../hal/display_hal.h"
#include "board.h"
#include <Arduino.h>
#include <Arduino_GFX_Library.h>

// 微雪 ESP32-S3-LCD-1.47：ST7789V 172x320，普通 4 线 SPI（非 QSPI AMOLED）。
// 与 boards/waveshare_amoled_18/display.cpp 的关键区别：
//   1. 总线用 Arduino_ESP32SPI（4 线 SPI），驱动用 Arduino_ST7789（非 Arduino_OLED 子类）。
//   2. 背光走 analogWrite PWM(GPIO48)。为什么：ST7789 没有 AMOLED 的亮度寄存器，
//      不能用 gfx->setBrightness（那是 Arduino_OLED 专有）。analogWrite 在 ESP32
//      Arduino Core 2.x/3.x 都可用（内部封装 LEDC），比直接调 ledc* API 跨版本稳。
//   3. 172 宽是 240 panel 的可视区，构造时传列偏移 LCD_COL_OFFSET 居中（board.h）。

static Arduino_DataBus* bus = nullptr;
static Arduino_GFX*     gfx = nullptr;

void display_hal_init(void) {
    // Arduino_ESP32SPI(dc, cs, sck, mosi, miso=未接)
    bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCLK, LCD_MOSI, GFX_NOT_DEFINED);
    // Arduino_ST7789(bus, rst, rotation, IPS, w, h, col_off1, row_off1, col_off2, row_off2)
    // rotation=1：USB-A 横插，逻辑横屏 320x172。构造传物理面板尺寸 PANEL_W×PANEL_H，
    // Arduino_GFX 内部按 rotation 映射坐标。列偏移是物理 172 宽方向的居中偏移。
    // 若旋转方向反了（上下颠倒），把 1 改成 3。
    gfx = new Arduino_ST7789(bus, LCD_RST, 1 /*rotation 90, USB横插*/, true /*IPS*/,
                             PANEL_W, PANEL_H,
                             LCD_COL_OFFSET, LCD_ROW_OFFSET,
                             LCD_COL_OFFSET, LCD_ROW_OFFSET);
}

void display_hal_begin(void) {
    gfx->begin();
    gfx->fillScreen(0x0000);
    pinMode(LCD_BL, OUTPUT);
    analogWrite(LCD_BL, 200);   // 默认亮度（0..255）
}

void display_hal_set_brightness(uint8_t level) {
    analogWrite(LCD_BL, level);  // PWM 背光，归属 GPIO48；改背光行为改这里
}

void display_hal_fill_screen(uint16_t color) {
    if (gfx) gfx->fillScreen(color);
}

void display_hal_draw_bitmap(int32_t x, int32_t y, int32_t w, int32_t h,
                             const uint16_t* pixels) {
    if (gfx) gfx->draw16bitRGBBitmap(x, y, (uint16_t*)pixels, w, h);
}

void display_hal_tick(void) {
    // 无 IMU 旋转，无需处理。
}

void display_hal_round_area(int32_t* x1, int32_t* y1, int32_t* x2, int32_t* y2) {
    // ST7789 SPI 不要求偶对齐刷新区域（仅 AMOLED CO5300 需要）。保持原样。
    (void)x1; (void)y1; (void)x2; (void)y2;
}
