#pragma once

// Waveshare ESP32-S3-LCD-1.47 — USB-A dongle, 1.47" ST7789V 172x320 SPI LCD.
// 与本项目其它板不同：这是普通 4 线 SPI TFT（非 QSPI AMOLED），无触摸/无 PMU/无 IMU。
// 引脚来源：Waveshare ESP32-S3-LCD-1.47 官方 wiki；列偏移待实测确认。

#define BOARD_NAME           "Waveshare LCD 1.47"

// ---- Display geometry ----
// 物理面板 ST7789 固有 172x320（竖）。本板 USB-A 横插，display.cpp 用 rotation=1 旋转 90°，
// 逻辑显示为横屏 320x172（LVGL/UI/caps 全部用逻辑尺寸 LCD_WIDTH/HEIGHT）。
// 改朝向：改 display.cpp 的 rotation（1/3 对应两个横向）+ 此处 LCD_WIDTH/HEIGHT 互换。
#define PANEL_W              172   // 物理面板宽（送给 Arduino_ST7789 构造）
#define PANEL_H              320   // 物理面板高
#define LCD_WIDTH            320   // 逻辑（旋转后）宽
#define LCD_HEIGHT           172   // 逻辑（旋转后）高

// ---- ST7789 4 线 SPI 引脚（非 QSPI；来源 skill/wiki）----
#define LCD_MOSI             45
#define LCD_SCLK             40
#define LCD_CS               42
#define LCD_DC               41
#define LCD_RST              39
#define LCD_BL               48    // 背光：analogWrite PWM（ST7789 无亮度寄存器）

// ST7789 列/行偏移：172 宽是 240x320 控制器 GRAM 的可视区，需居中偏移。
// (240-172)/2 = 34；竖屏 row 偏移 0。待实测微调（出现黑边/错位时改这里）。
#define LCD_COL_OFFSET       34
#define LCD_ROW_OFFSET       0

// ---- Buttons ----
// 仅 BOOT(IO0) 确定可用；分配给 touch.cpp 做假触摸切页（见该文件说明）。
#define BTN_BACK_GPIO        0

// ---- Capability flags（全 0：无次按钮/旋转/IMU/电池/IO 扩展）----
// caps.cpp 据此生成 BoardCaps；shared code 用 BoardCaps 隐藏可选功能，零 #ifdef。
#define BOARD_HAS_SECONDARY_BUTTON 0
#define BOARD_HAS_ROTATION         0
#define BOARD_HAS_IMU              0
#define BOARD_HAS_BATTERY          0
#define BOARD_HAS_IO_EXPANDER      0
