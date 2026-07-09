# QQDeBreath ARA 1.01 Release Notes

Date: 2026-07-08

## Scope

This is the QQDeBreath ARA/VST3 plugin prototype release. It is not the
standalone/exe app version.

Plugin display/version label:

```text
ARA 1.01
```

VST3 module version:

```text
1.0.1
```

## Analysis Backend

ARA 1.01 keeps the existing bridge architecture:

```text
QQDeBreath.vst3
  -> qq_debreath_bridge.exe
  -> QQDeBreathTool.exe analyze-for-plugin
```

Expected app bridge:

```text
QQDeBreathTool 1.11
```

Expected model sha256:

```text
D655AF2BFB260866DE74319D179FDA0B007E6539711C572967BC0FC709E42AFE
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

## Verification

- Bridge smoke test passed with `QQDeBreathTool.exe analyze-for-plugin`.
- Built `QQDeBreath_VST3` successfully.
- `moduleinfo.json` contains:
  - `Audio Module Class`
  - `Component Controller Class`
  - `ARA Main Factory Class`

## Notes

- No C++ algorithm port is included.
- A compatible `QQDeBreathTool.exe` must remain available for analysis.
- ARA and VST3 analysis share the same background bridge call path.
