# QQDeBreath ARA 1.09 Release Notes

ARA 1.09 focuses on Reaper host compatibility. The native Breath analyzer,
embedded QQDeBreathTool 1.11 model, region rules, and stem rendering behavior
are unchanged.

## Changes

- The editor now distinguishes ARA and regular VST3 operation from the
  processor's actual JUCE ARA binding. An ARA-capable host no longer causes a
  normal VST3 insert to display the ARA Load workflow instead of Record.
- ARA playback now converts Audio Source samples from the source sample rate to
  the host playback sample rate with linear interpolation.
- ARA region gain, Fade, Breath Norm, Global Gain, Global EQ, and selected
  Breath processing continue to use source-domain sample positions after the
  sample-rate conversion.
- All ARA 1.07 recording, EQ, default-preset, and project-state behavior is
  retained.

## Validation

- Windows Release unified VST3/ARA target builds and installs successfully.
- Source-rate conversion and actual-ARA-binding paths compile successfully.
- Reaper ARA playback and regular VST3 UI selection require final host-side
  confirmation because Reaper is not installed on the development machine.
- No Breath analysis algorithm or model changes.