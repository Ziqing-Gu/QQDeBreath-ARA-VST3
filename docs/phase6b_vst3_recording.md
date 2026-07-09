# Phase 6B Plain VST3 Recording

Phase 6B gives the plain `QQDeBreath` VST3 target a minimal recording loop.

This is separate from `QQDeBreath_ARA`.

## Audio Source Rule

Plain VST3 does not read DAW ARA Audio Events.

Its audio source is:

```text
the current plugin instance's recorded input buffer
```

This is per-project and per-plugin-instance state. It must not use the
Standalone/exe global last-session restore model.

## Target

Plain target:

```text
QQDeBreath
Generated build target: QQDeBreath_VST3
Product: QQDeBreath
```

The ARA target remains separate:

```text
QQDeBreath_ARA
Generated build target: QQDeBreath_ARA_VST3
Product: QQDeBreath_ARA
```

## UI

The plain VST3 editor now shows:

```text
Record
Stop Recording
Clear Recording
Export Temp WAV
recorded sample rate
recorded channels
recorded samples
recorded duration
temp wav path
status
```

No waveform editor, Breath analysis, Noize regions, Python bridge, or ARA logic
is present in this phase.

## Record / Stop / Clear

`Record`:

1. Clears any previous recorded buffer in this plugin instance.
2. Stores the current host sample rate.
3. Enables input capture.
4. The audio thread appends the plugin input buffer to the instance buffer.
5. Audio passthrough remains active.

`Stop Recording`:

1. Disables input capture.
2. Leaves the recorded buffer in memory.

`Clear Recording`:

1. Stops recording.
2. Clears the recorded buffer.
3. Clears dropped-record-block counters.

## Recorded Buffer State

The recorded buffer is stored in:

```text
QQDeBreathAudioProcessor::recordedBuffer
```

Metadata:

```text
recordedSampleRate
channel count
sample count
duration
is recording
dropped record block count
```

The audio callback uses a try-lock when appending. If the UI or host state
serialization is holding the recording lock, the audio callback skips that block
and increments the dropped-block counter instead of blocking.

## Project Save / Restore

`getStateInformation()` writes:

```text
magic: QQDeBreathVST3State
version: 2
AudioProcessorValueTreeState XML
hasRecording
recordedSampleRate
channel count
sample count
float32 channel data
```

`setStateInformation()` restores:

```text
parameters
recorded buffer
recorded sample rate
channel count
sample count
```

This is DAW project state only. It does not read:

```text
Standalone last session
last opened wav
global exe settings
```

Phase 6B includes a safety cap of 20 minutes at 48 kHz per channel for buffer
state persistence. Longer recordings are kept in memory during the session but
are not serialized into the DAW project state by this minimal implementation.

## Temp WAV Export

`Export Temp WAV` writes the recorded buffer to:

```text
%TEMP%\QQDeBreath\VST3\Phase6B
```

Filename format:

```text
QQDeBreath_recorded_YYYYMMDD_HHMMSS.wav
```

Format:

```text
WAV
32-bit float
recorded channel count
recorded sample rate
```

## Build

Build and install the plain VST3:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\plugin\build-vst3.ps1
```

Build and install the ARA target separately:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\plugin\build-ara-vst3.ps1
```

## Manual DAW Test Checklist

Plain VST3:

1. Open Cubase, REAPER, or another VST3 host.
2. Rescan VST3 plugins if needed.
3. Insert `QQDeBreath` as a normal VST3 insert on an audio track.
4. Confirm the UI shows `Phase 6B VST3 Recorder`.
5. Play audio through the track.
6. Click `Record`.
7. Confirm sample count and duration increase.
8. Click `Stop Recording`.
9. Confirm recorded info remains visible.
10. Click `Export Temp WAV`.
11. Confirm a WAV appears in `%TEMP%\QQDeBreath\VST3\Phase6B`.
12. Click `Clear Recording`.
13. Confirm recorded samples return to zero.
14. Record again, save the DAW project, close it, reopen it.
15. Confirm the recorded buffer info restores in that plugin instance.

ARA target sanity check:

1. Confirm `QQDeBreath_ARA` still appears as an Audio Extension.
2. Confirm `Reload Source` still works on an audio event.
