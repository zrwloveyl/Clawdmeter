#pragma once
#include <Arduino.h>

struct UsageData {
    float session_pct;       // 5-hour window utilization (0-100)
    int session_reset_mins;  // minutes until session resets
    float weekly_pct;        // 7-day window utilization (0-100)
    int weekly_reset_mins;   // minutes until weekly resets
    char status[16];         // "allowed" or "limited"
    char activity[24];       // Claude 实时工作状态(Thinking/Bash/Edit/Idle…)，经 CC hook->sender 提供
    bool ok;                 // data parse succeeded
    bool valid;              // false until first successful parse
};
