#!/usr/bin/env python3
"""claude-clawd-sender.py

Reads Claude Code usage + activity from ~/.cache and pushes them to the ESP32
Clawdmeter (Waveshare ESP32-S3-LCD-1.47) over USB serial.

Pairs with (see README):
  - a Claude Code statusLine that writes ~/.cache/claude-usage.json   (usage %)
  - Claude Code hooks that write       ~/.cache/claude-activity.json  (activity)

Env:
  CLAWD_PORT   serial device (default /dev/ttyACM0)
"""
import json, time, os
import serial  # pip install pyserial

USAGE    = os.path.expanduser("~/.cache/claude-usage.json")
ACTIVITY = os.path.expanduser("~/.cache/claude-activity.json")
PORT     = os.environ.get("CLAWD_PORT", "/dev/ttyACM0")
BAUD     = 115200
INTERVAL = 2  # seconds; activity needs to feel real-time

def build_payload():
    try:
        with open(USAGE) as f:
            d = json.load(f)
    except Exception:
        return None
    now = time.time()
    def to_mins(ts):
        try:
            m = (float(ts) - now) / 60.0
            return int(m) if m > 0 else 0
        except Exception:
            return -1
    s = int(d.get("cu_s", -1))
    w = int(d.get("cu_w", -1))
    act = ""
    try:
        with open(ACTIVITY) as f:
            act = (json.load(f).get("label", "") or "")[:20]
    except Exception:
        act = ""
    return {
        "s": s if s >= 0 else 0,
        "sr": to_mins(d.get("cu_sr_ts")),
        "w": w if w >= 0 else 0,
        "wr": to_mins(d.get("cu_wr_ts")),
        "st": "allowed",
        "ok": s >= 0,
        "act": act,
    }

def main():
    ser = serial.Serial(PORT, BAUD, timeout=1)
    print(f"[clawd] -> {PORT} @ {BAUD}, every {INTERVAL}s", flush=True)
    while True:
        p = build_payload()
        if p:
            line = json.dumps(p, separators=(",", ":")) + "\n"
            try:
                ser.write(line.encode())
            except Exception as e:
                print("[clawd] serial write failed:", e, flush=True)
        time.sleep(INTERVAL)

if __name__ == "__main__":
    main()
