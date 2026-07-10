# QQDeBreath ARA 1.07 Release Notes

ARA 1.07 focuses on host compatibility, VST3 recording reliability, and EQ
workflow clarity. The analysis algorithm and embedded model are unchanged.

## Changes

- VST3 Record now works as timeline punch-in recording: overlapping positions
  are overwritten, unrecorded positions are filled, and unrelated existing
  recording is preserved.
- Recording storage uses large preallocated capacity instead of resizing the
  complete recording on every audio block, reducing realtime CPU spikes and
  crackle risk in low-buffer hosts and VST3 bridges.
- ARA playback accepts host time in samples when time in seconds is unavailable,
  improving Reaper ARA compatibility.
- ARA/VST3 UI selection now requires an actual ARA source/selection instead of
  treating every ARA-capable editor view as an active ARA extension.
- Selected Breath Gain now changes the purple Post spectrum level while the
  orange Pre spectrum remains before local EQ/Gain.
- Dynamic Global and Selected spectra hold each displayed frequency band's peak
  after playback.
- Global and Selected Auto Apply default to on and are saved by Set as Default.
- Set as Default now asks for confirmation before writing the startup preset.
- Added Clear buttons for Global EQ and Selected Breath EQ. Selected Clear keeps
  the selected Breath Gain.

## Validation

- Windows Release VST3/ARA build must complete successfully.
- Cubase/Nuendo, Reaper, Studio One, and Pro Tools bridge behavior still require
  host-side manual validation. Reaper and Pro Tools are not installed on the
  development machine.
- No Breath analysis algorithm or model changes.