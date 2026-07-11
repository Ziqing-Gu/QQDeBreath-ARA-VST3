# QQDeBreath ARA 1.11 Release Notes

ARA 1.11 corrects the global-default reset boundary for ARA loading. The
native analyzer, QQDeBreathTool 1.11 model, realtime playback optimization,
recording, EQ processing, and stem rendering are unchanged from ARA 1.10.

## Default and Project State Rules

- A user-initiated ARA Load now applies the saved global default preset before
  loading the selected Audio Source or selected events.
- If no valid global default file exists, explicit Load applies the built-in
  defaults.
- Explicit Load clears the previous analysis and per-Breath Gain/EQ state.
- Closing and reopening the plugin editor keeps the current project state.
- Automatically reopening a DAW project restores that project's saved source,
  regions, edits, global controls, and EQ instead of applying global defaults.
- New regular VST3 instances continue to use global defaults, while restored
  VST3 project instances continue to use their saved instance state.

## Validation

- The implementation starts from the verified ARA 1.10 D-drive backup baseline.
- Windows Release unified VST3/ARA target builds and installs successfully.
- No Breath analysis algorithm or model changes.
- Host-side ARA Load/project-restore behavior requires manual DAW validation.