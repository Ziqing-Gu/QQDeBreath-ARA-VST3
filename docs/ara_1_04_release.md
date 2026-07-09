# QQDeBreath ARA 1.04 Release Notes

Date: 2026-07-09

## Scope

This is the QQDeBreath ARA/VST3 plugin prototype release. It is not the
standalone/exe app version.

Plugin display/version label:

```text
ARA 1.04
```

VST3 module version:

```text
1.0.4
```

## Changes

ARA 1.04 focuses on Breath EQ workflow:

```text
Load ARA -> renamed to Load
Main page EQ checkbox -> enables/disables Breath EQ
Breath EQ button -> opens dedicated EQ page
X -> returns to main waveform page
EQ page Loop -> previews current/next Breath
Dynamic spectrum -> Pre/Post EQ comparison
HP/LP toggles removed from the header
Click graph edges -> creates HP/LP cuts
```

## Processing

Breath EQ remains Breath-only:

```text
Breath raw split
  -> optional Breath Norm
  -> Breath Adjust
  -> Breath EQ
  -> monitor and Breath.wav export
```

Voice and Noize are not EQ-processed. Detection, region type, and region
boundaries are not changed by EQ.

## Loop Behavior

ARA mode:

```text
Loop requests host cycle range and playback start for the current/next Breath.
```

Plain VST3 mode:

```text
Loop controls QQDeBreath's internal preview position only.
The DAW main playhead is not moved.
```

## Current ARA Multi-Clip Limitation

The current editor/analyze model is still single-source based. If a Cubase track
has multiple separate audio events, the plugin can play them through the ARA
renderer, but the waveform/analyze UI loads one ARA audio source at a time.

Correct multi-event support requires a new composite ARA timeline layer:

```text
collect playback regions
map each event to source + playback time + modification offset
render a timeline composite for display/analyze
map edited regions back to the owning source/event
persist per-source regions plus timeline placement
```

## Build Output

Local VST3 bundle:

```text
plugin/build-vs/QQDeBreath_artefacts/Release/VST3/QQDeBreath.vst3
```

Installed VST3 bundle:

```text
C:\Program Files\Common Files\VST3\QQDeBreath.vst3
```

Build command:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\plugin\build-ara-vst3.ps1
```

Build result:

```text
Passed. QQDeBreath.vst3 was installed to the common VST3 folder.
```
