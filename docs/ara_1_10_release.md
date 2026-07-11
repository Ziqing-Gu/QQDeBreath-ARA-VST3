# QQDeBreath ARA 1.10 Release Notes

ARA 1.10 focuses on eliminating realtime playback stalls and crackle reported
when Reaper plays audio loaded through ARA. The native Breath analyzer,
QQDeBreathTool 1.11 model, region rules, and stem rendering behavior are
unchanged.

## Realtime Playback Changes

- ARA source analysis state is cached per Audio Source and refreshed only when
  the document state revision changes.
- Playback parameters use a separate revision cache.
- Audio-thread state refresh uses a non-blocking try-lock. If the message thread
  is editing state, playback continues with the previous valid snapshot.
- Breath Norm peaks are calculated from the loaded waveform outside the audio
  callback and persisted with the ARA document state.
- The renderer no longer scans a complete Breath region during playback.
- Unchanged Global and Selected Breath EQ states are compared structurally;
  realtime EQ preparation no longer serializes JSON every audio block.
- Temporary audio buffers clear only the active channels and sample range.
- Source-rate conversion and Reaper's actual ARA/VST3 mode detection from 1.09
  are retained.

## Compatibility

- Existing 1.09 ARA project state remains readable. Newly saved 1.10 project
  state also includes the prepared region-peak cache.
- Regular VST3 recording behavior is unchanged.
- Exported Breath.wav still applies Fade, Breath Norm, Global Gain, Global EQ,
  Selected Breath Gain, and enabled Selected Breath EQ.
- Vocal Only.wav and Noize.wav are not affected by Breath Gain or Breath EQ.

## Validation

- Windows Release unified VST3/ARA target configures, builds, and installs.
- Static realtime audit confirms that the ARA renderer no longer calls the
  blocking persistent-state getters or whole-region peak scanner.
- Reaper playback crackle requires final host-side confirmation because Reaper
  is not installed on the development machine.
- No Breath analysis algorithm or model changes.