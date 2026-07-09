# QQDeBreath ARA 1.03 Release Notes

Date: 2026-07-08

## Scope

This is the QQDeBreath ARA/VST3 plugin prototype release. It is not the
standalone/exe app version.

Plugin display/version label:

```text
ARA 1.03
```

VST3 module version:

```text
1.0.3
```

## Breath EQ

ARA 1.03 adds a global Breath EQ prototype:

```text
Main page -> Breath EQ button -> dedicated EQ page -> X returns to main page
```

The EQ is applied only to the Breath component:

```text
Breath raw split
  -> optional Breath Norm
  -> Breath Adjust
  -> Breath EQ
  -> monitor and Breath.wav export
```

Voice and Noize are not EQ-processed. Detection, region type, and region
boundaries are not changed by EQ.

The EQ page supports:

```text
Enable/disable Breath EQ
High-pass handle
Low-pass handle
Up to six Bell bands
Breath spectrum display
Double-click to create a Bell band
Drag a band to change frequency/gain
Mouse wheel over a band to adjust Q
Right-click a band to remove it
Alt-click to reset EQ
```

## State

Breath EQ state is saved in:

```text
VST3 processor state
ARA document playback state
```

ARA project reopen should restore the loaded source, analysis regions, monitor
settings, Breath Norm/Adjust/Fade settings, and Breath EQ state.

## Build Output

Local VST3 bundle:

```text
plugin/build-vs/QQDeBreath_artefacts/Release/VST3/QQDeBreath.vst3
```

Installed VST3 bundle:

```text
C:\Program Files\Common Files\VST3\QQDeBreath.vst3
```

## Verification

Build command:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\plugin\build-ara-vst3.ps1
```

Build result:

```text
Passed. QQDeBreath.vst3 was installed to the common VST3 folder.
```

Manual DAW checks still required:

```text
1. ARA: Load ARA source, Analyze, open Breath EQ page, edit EQ, close with X.
2. ARA: Confirm Breath monitor changes when EQ changes.
3. ARA: Export stems and confirm Breath.wav contains EQ processing.
4. VST3: Record, Analyze, open Breath EQ page, edit EQ, close with X.
5. VST3: Save/reopen DAW project and confirm EQ state is restored.
```
