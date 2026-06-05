#!/usr/bin/env bash
# claude-usage-statusline.sh
#
# Claude Code statusLine script. Parses `rate_limits` from the stdin JSON that
# Claude Code feeds the statusLine, writes ~/.cache/claude-usage.json for the
# ESP32 Clawdmeter sender, and prints a one-line usage summary to the terminal.
#
# Missing data is never faked: if rate_limits is absent we keep the previous
# cache (so the board doesn't flash 0%).
exec python3 -c '
import sys, json, os, time
CACHE = os.path.expanduser("~/.cache/claude-usage.json")

cu_s = cu_w = -1
cu_sr_ts = cu_wr_ts = 0
try:
    d  = json.load(sys.stdin)
    rl = d.get("rate_limits") or {}
    fh = rl.get("five_hour")  or {}
    sd = rl.get("seven_day")  or {}
    if "used_percentage" in fh:
        cu_s = int(fh["used_percentage"]); cu_sr_ts = int(fh.get("resets_at") or 0)
    if "used_percentage" in sd:
        cu_w = int(sd["used_percentage"]); cu_wr_ts = int(sd.get("resets_at") or 0)
except Exception:
    pass

# stop-loss: do not overwrite a good cache with -1 when rate_limits is missing
if cu_s >= 0 or cu_w >= 0:
    try:
        os.makedirs(os.path.dirname(CACHE), exist_ok=True)
        tmp = CACHE + ".tmp"
        with open(tmp, "w") as f:
            json.dump({"cu_s": cu_s, "cu_w": cu_w,
                       "cu_sr_ts": cu_sr_ts, "cu_wr_ts": cu_wr_ts,
                       "ts": int(time.time())}, f)
        os.replace(tmp, CACHE)
    except Exception:
        pass

s = (str(cu_s) + "%") if cu_s >= 0 else "-"
w = (str(cu_w) + "%") if cu_w >= 0 else "-"
sys.stdout.write(f"S {s} | W {w}")
'
