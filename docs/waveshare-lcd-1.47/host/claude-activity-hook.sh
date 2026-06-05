#!/bin/bash
# claude-activity-hook.sh
#
# Claude Code lifecycle hook. Writes the current activity to
# ~/.cache/claude-activity.json so the ESP32 Clawdmeter can show what Claude is
# doing right now (the sender reads it). Wire it to UserPromptSubmit / PreToolUse
# / Stop in ~/.claude/settings.json (see README).
#
# Rules:
#   - must be fast
#   - must NOT print to stdout (a UserPromptSubmit hook's stdout enters the
#     model context). Only write the file, then exit 0.
#
# Note: PostToolUse is deliberately NOT handled. A tool finishes in tens of ms;
# if PostToolUse wrote "Thinking" it would instantly overwrite the tool name and
# the sender's sampling would almost never catch the real tool. So the tool name
# is kept until the next PreToolUse (next tool) or Stop (Idle).
input=$(cat)
ev=$(printf '%s' "$input"   | jq -r '.hook_event_name // empty' 2>/dev/null)
tool=$(printf '%s' "$input" | jq -r '.tool_name // empty'       2>/dev/null)
case "$ev" in
  UserPromptSubmit)  label="Thinking" ;;
  PreToolUse)        label="${tool:-Tool}" ;;
  Stop|SubagentStop) label="Idle" ;;
  *) exit 0 ;;   # PostToolUse etc: keep the previous tool state
esac
mkdir -p "$HOME/.cache"
printf '{"label":"%s","ts":%s}\n' "$label" "$(date +%s)" > "$HOME/.cache/claude-activity.json" 2>/dev/null
exit 0
