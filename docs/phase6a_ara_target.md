# Phase 6A QQDeBreath_ARA Target

Phase 6A adds a separate ARA-capable VST3 target while preserving the existing
plain VST3 target.

## Targets

Plain VST3 target:

```text
QQDeBreath
Generated build target: QQDeBreath_VST3
Product: QQDeBreath
```

ARA target:

```text
QQDeBreath_ARA
Generated build target: QQDeBreath_ARA_VST3
Product: QQDeBreath_ARA
```

The plain VST3 target must remain a normal audio passthrough shell. The ARA
target is built, installed, tested, and debugged separately.

## Build

Build the ARA target:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\plugin\build-ara-vst3.ps1
```

Build the plain VST3 target:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\plugin\build-vst3.ps1
```

## ARA SDK

ARA SDK path:

```text
plugin/external/ARA_SDK
```

CMake variable:

```text
QQDEBREATH_ARA_SDK_PATH=plugin/external/ARA_SDK
```

## Output

ARA VST3 bundle:

```text
plugin/build-vs/QQDeBreath_ARA_artefacts/Release/VST3/QQDeBreath_ARA.vst3
```

Installed ARA VST3 bundle:

```text
C:\Program Files\Common Files\VST3\QQDeBreath_ARA.vst3
```

The generated `moduleinfo.json` contains:

```text
Category: ARA Main Factory Class
Name: QQDeBreath_ARA
```

The plain `QQDeBreath.vst3` manifest does not contain an ARA factory class.

## UI

The Phase 6A ARA UI intentionally contains only:

```text
QQDeBreath ARA
Phase 6A Source Reader
Reload Source
Source metadata
Temp wav export status
```

It does not show Breath regions, Noize regions, waveform editing, Python
analysis, bridge calls, recording, or export controls.

## Reload Source

`Reload Source`:

1. Reads the current ARA editor view.
2. Uses the effective selected playback region when available.
3. Falls back to the first audio source in the current ARA document.
4. Reads source metadata:
   - name
   - persistent ID
   - sample rate
   - channel count
   - sample count
   - duration
5. Exports the complete ARA audio source to a temp WAV.
6. Updates the UI with source metadata and export status.

## Temp WAV

Temp WAV directory:

```text
%TEMP%\QQDeBreath\ARA\Phase6A
```

File naming:

```text
<source-name>_<source-fingerprint>.wav
```

## Source Fingerprint

Phase 6A uses a lightweight source fingerprint for identity display.

The fingerprint is FNV-1a 64-bit over:

```text
persistent_id
sample_rate
channel_count
sample_count
```

This is not a full audio-content hash. It is only a Phase 6A source identity
marker. Later phases may add SHA-256 over exported WAV content if bridge/cache
validation needs it.

## Cubase Checklist

The user must manually verify:

1. Cubase scans `QQDeBreath_ARA.vst3`.
2. `QQDeBreath_ARA` appears in the Audio Extensions menu for an audio event.
3. The extension can be applied to an audio event.
4. The plugin editor opens.
5. The UI shows `QQDeBreath ARA` and `Phase 6A Source Reader`.
6. Clicking `Reload Source` shows sample rate, channel count, samples, duration, and source fingerprint.
7. Clicking `Reload Source` writes a temp WAV under `%TEMP%\QQDeBreath\ARA\Phase6A`.
8. Saving and reopening the Cubase project does not crash.
9. The separate plain `QQDeBreath` VST3 still loads as a normal VST3.

## Out Of Scope

Phase 6A does not:

```text
call Python
call bridge
analyze Breath
show Breath / Noize regions
draw a waveform editor
record audio
export final stems
```
