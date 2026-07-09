# QQDeBreath ARA 1.05 Release Notes

Date: 2026-07-09

## Summary

ARA 1.05 focuses on multi-event ARA loading and finer Breath-region shaping.

## Changes

- `Load` can combine multiple selected ARA playback regions into one temporary waveform.
- Composite ARA regions are mapped back to individual ARA sources for playback persistence.
- Added per-Breath `Adjust` page with -30 dB to +30 dB Gain.
- Added per-Breath EQ state and project serialization.
- Global Breath EQ HP/LP nodes can be removed with right-click.
- Main waveform display now reflects Breath Adjust/Norm/Fade/EQ processing.
- Exported `Breath.wav` applies global Breath EQ and per-Breath Gain/EQ.

## Limits

- Multi-event ARA Load currently requires the selected events to use the same sample rate.
- Per-Breath EQ playback is applied when a playback block only contains one active custom-EQ Breath region. Export and waveform rendering use the per-region EQ path directly.
