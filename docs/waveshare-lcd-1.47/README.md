# Clawdmeter — Waveshare ESP32-S3-LCD-1.47 (landscape) port

A port of [Clawdmeter](https://github.com/HermannBjorgvin/Clawdmeter) to the
**Waveshare ESP32-S3-LCD-1.47** (ST7789V 172×320 SPI), mounted **landscape
(320×172)**. It shows your live Claude Code usage **and what Claude is doing
right now**, as Clawd pixel animations.

> 简体中文说明：[README.zh-CN.md](README.zh-CN.md)

| Usage | Coding | Thinking | Idle |
|:---:|:---:|:---:|:---:|
| ![usage](images/usage.png) | ![coding](images/coding.png) | ![think](images/think.png) | ![idle](images/idle.png) |

## Features
- **Usage screen** — 5-hour & 7-day rate-limit %, progress bars, reset countdown.
- **Live activity (the headline)** — the Clawd animation reflects what Claude is
  doing in real time:

  | Claude is… | Clawd… |
  |---|---|
  | Idle | sleeps |
  | Thinking | head-on-hand |
  | Bash / Edit / Write | codes at a laptop |
  | Read / Grep / Glob | thinks (reading = processing) |
  | WebFetch / WebSearch | looks surprised (going online) |
  | Task (subagent) | dances |

- **Always-on**, **USB-serial** data path (no BLE pairing).

## Hardware
- **Waveshare ESP32-S3-LCD-1.47**: ST7789V 172×320, ESP32-S3R8, 8 MB PSRAM, 16 MB flash, USB-A plug.
- Display pins: `MOSI=45 SCLK=40 CS=42 DC=41 RST=39 BL=48`, backlight via PWM.
- Mounted **landscape** (USB-A horizontal) → `display rotation=3` in
  `firmware/src/boards/waveshare_lcd_147/display.cpp` (use `1` for the other way up).

## Build (cloud — recommended)
The board is a PlatformIO env. A push triggers GitHub Actions; you only flash.

```yaml
# already in this repo: .github/workflows/build-lcd147.yml
- run: pip install --upgrade platformio
- run: pio run -d firmware -e waveshare_lcd_147
```

Artifact: `firmware/.pio/build/waveshare_lcd_147/firmware.bin`.
(Local build if you must: same `pio run` line — needs PlatformIO, ~4 min first time.)

## Flash
```bash
# single firmware
esptool --port /dev/ttyACM0 --baud 921600 write-flash 0x10000 firmware.bin

# or app1 slot (dual-OTA; keeps whatever is in app0)
esptool --port /dev/ttyACM0 --baud 921600 write-flash 0x310000 firmware.bin
```
> The firmware is IDF5 (pioarduino). A stock IDF4 bootloader was observed to boot
> it fine, so flashing only the app (not 0x0) usually works.

## Host side — feed data to the board
Everything is in [`host/`](host/). Three small pieces:

**1. Usage** — point your Claude Code statusLine at `claude-usage-statusline.sh`
(it writes `~/.cache/claude-usage.json`). In `~/.claude/settings.json`:
```json
"statusLine": { "type": "command", "command": "bash /path/to/host/claude-usage-statusline.sh" }
```
> Already using another statusLine (e.g. a HUD plugin)? Don't replace it — write a
> tiny wrapper that tees stdin to both, and point statusLine at the wrapper:
> ```bash
> input=$(cat)
> printf '%s' "$input" | bash /path/to/host/claude-usage-statusline.sh >/dev/null 2>&1
> printf '%s' "$input" | bash /path/to/your-existing-statusline.sh
> ```

**2. Activity** — wire `claude-activity-hook.sh` to hooks in `settings.json`:
```json
"hooks": {
  "UserPromptSubmit": [ { "hooks": [ { "type":"command","command":"bash /path/to/host/claude-activity-hook.sh" } ] } ],
  "PreToolUse":  [ { "matcher":"*", "hooks": [ { "type":"command","command":"bash /path/to/host/claude-activity-hook.sh" } ] } ],
  "Stop":        [ { "hooks": [ { "type":"command","command":"bash /path/to/host/claude-activity-hook.sh" } ] } ]
}
```

**3. Sender** — `claude-clawd-sender.py` reads both caches and sends one JSON line
over serial every 2 s.
```bash
pip install pyserial
python3 host/claude-clawd-sender.py          # CLAWD_PORT=/dev/ttyACM0 by default
```
Or install as a service: edit `User`/path in `host/claude-clawd.service`, then
`sudo cp host/claude-clawd.service /etc/systemd/system/ && sudo systemctl enable --now claude-clawd`.

## Data flow
```
Claude Code --statusLine--> ~/.cache/claude-usage.json    --+
            --hooks-------> ~/.cache/claude-activity.json  --+
                                                             +-- sender (2s) --serial--> ESP32
ESP32: parse JSON -> LVGL usage screen + Clawd activity animation
```

## Serial protocol (board side)
One JSON line @ 115200, e.g.:
```json
{"s":42,"sr":200,"w":18,"wr":9000,"st":"allowed","ok":true,"act":"Bash"}
```
`s`/`w` = session/weekly %, `sr`/`wr` = minutes to reset, `act` = current activity.
The board also accepts plain commands: `scr usage`, `scr splash`, `screenshot`.

## Credits
Port of [Clawdmeter](https://github.com/HermannBjorgvin/Clawdmeter) by HermannBjorgvin.
Clawd pixel art via claudepix. See the upstream repo for its licensing note
(proprietary fonts / mascot) — the same applies here.
