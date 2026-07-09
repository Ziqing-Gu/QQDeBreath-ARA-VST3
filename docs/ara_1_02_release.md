# QQDeBreath ARA 1.02 Release Notes

Date: 2026-07-08

## Scope

This is the QQDeBreath ARA/VST3 plugin prototype release. It is not the
standalone/exe app version.

Plugin display/version label:

```text
ARA 1.02
```

VST3 module version:

```text
1.0.2
```

## Analysis Backend

ARA 1.02 replaces the normal bridge analysis path with native C++ analysis:

```text
QQDeBreath.vst3
  -> native C++ feature extraction
  -> embedded HistGradientBoosting model
  -> native C++ region post-processing
```

The native analyzer embeds the QQDeBreathTool 1.11 Breath model:

```text
D655AF2BFB260866DE74319D179FDA0B007E6539711C572967BC0FC709E42AFE
```

The old `qq_debreath_bridge.exe` target remains available for debugging, but
the plugin's normal Analyze button does not call `QQDeBreathTool.exe`.

## Native Coverage

Implemented in C++:

```text
prepare_for_analysis equivalent
28 feature columns
StandardScaler
HistGradientBoostingClassifier tree inference
probability_to_regions
spectral_breath_regions
merge_time_regions
snap_regions_to_low_points
filter_auto_breath_regions
normalize_regions
```

Noize automatic detection is still not enabled by default. Noize remains a
manual/editable first-class region type and is supported by monitoring/export.

## Verification

Build:

```text
powershell -NoProfile -ExecutionPolicy Bypass -File .\plugin\build-ara-vst3.ps1
```

Native probe:

```text
plugin/build-vs/Release/qq_debreath_native_probe.exe
```

Python 1.11 vs C++ 1.02 comparison:

```text
2024/Vocal.wav
  Python regions: 29
  C++ regions:    29
  matched:        29
  average IoU:    0.999246
  max start diff: 0.000729 s
  max end diff:   0.007250 s

水母银河/水母银河 - Vocal.wav
  Python regions: 42
  C++ regions:    42
  matched:        42
  average IoU:    0.998875
  max start diff: 0.004989 s
  max end diff:   0.004989 s
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
