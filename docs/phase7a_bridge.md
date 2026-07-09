# Phase 7A C++ Console Bridge

Phase 7A adds an independent C++ console bridge:

```text
qq_debreath_bridge.exe
```

The bridge calls the Python/exe implementation through:

```text
QQDeBreathTool.exe analyze-for-plugin
```

In ARA `1.01`, this bridge was integrated into the unified QQDeBreath VST3/ARA
target and was called from a background analysis thread.

From ARA `1.02` onward, the normal plugin Analyze path uses native C++ analysis
inside `QQDeBreath.vst3`. The bridge remains available as a debug/compatibility
tool, but the plugin does not require `QQDeBreathTool.exe` for normal analysis.

## Build

Build only the bridge:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\plugin\build-bridge.ps1
```

Output:

```text
plugin/build-vs/Release/qq_debreath_bridge.exe
```

The bridge is a JUCE `juce_core` console executable. On Windows it uses `wmain`
so command-line arguments are received as UTF-16, which preserves Chinese paths.

## Required Tool Version

`QQDeBreathTool.exe` must be built from a source version that supports:

```text
analyze-for-plugin
--breath-gain-db
```

For ARA `1.01`, the expected tool was QQDeBreathTool `1.11`.

Expected model sha256:

```text
D655AF2BFB260866DE74319D179FDA0B007E6539711C572967BC0FC709E42AFE
```

The PyInstaller spec was updated with:

```text
hiddenimports += ['debreath_tool_app']
```

This is required because `qq_debreath.core.legacy` dynamically imports
`debreath_tool_app`.

## Command

```powershell
.\plugin\build-vs\Release\qq_debreath_bridge.exe `
  --tool .\dist\QQDeBreathTool\QQDeBreathTool.exe `
  --input .\exe_smoke_test.wav `
  --output-json .\plugin\bridge_test\normal_final\result.json `
  --out-dir .\plugin\bridge_test\normal_final\out `
  --threshold 0.86 `
  --fade-ms 10 `
  --breath-target-db -6 `
  --breath-gain-db 0 `
  --detect-noize 0 `
  --timeout-ms 600000
```

Optional bridge argument:

```text
--model path\to\model.joblib
```

Debug-only validation argument:

```text
--validate-json-only --output-json result.json
```

## Success Output

The bridge prints:

```text
sample_rate
duration
breath_count
noize_count
vocal_only
breath
noize
```

Observed normal test result:

```text
sample_rate: 44100
duration: 2.0
breath_count: 0
noize_count: 0
vocal_only: plugin/bridge_test/normal_final/out/exe_smoke_test_Vocal Only.wav
breath: plugin/bridge_test/normal_final/out/exe_smoke_test_Breath.wav
noize: plugin/bridge_test/normal_final/out/exe_smoke_test_Noize.wav
```

## Error Handling

Implemented checks:

```text
tool missing
input missing
out-dir missing or not writable
subprocess start failure
subprocess timeout
subprocess exit code non-zero
result.json missing
result.json status = error
schema_version mismatch
regions missing
files missing
files.vocal_only / files.breath / files.noize missing
```

Observed error tests:

```text
missing tool -> ERROR: tool does not exist
missing input -> ERROR: input does not exist
timeout -> ERROR: subprocess timed out
out-dir is a file -> ERROR: out-dir is not a directory
missing result.json -> ERROR: result.json does not exist
status error -> ERROR: tool returned status=error code=MODEL_NOT_FOUND
schema mismatch -> ERROR: schema_version mismatch
missing regions -> ERROR: Missing required JSON field: regions
missing files -> ERROR: Missing required JSON field: files
```

## Chinese Path Test

The bridge was tested with:

```text
input: plugin/bridge_test/中文 路径 输入/测试 音频.wav
output-json: plugin/bridge_test/中文 输出 目录/result.json
out-dir: plugin/bridge_test/中文 输出 目录/out stems
```

Observed result:

```text
PASS
vocal_only: ...\中文 输出 目录\out stems\测试 音频_Vocal Only.wav
breath: ...\中文 输出 目录\out stems\测试 音频_Breath.wav
noize: ...\中文 输出 目录\out stems\测试 音频_Noize.wav
```

## Breath Gain

The bridge passes:

```text
--breath-gain-db
```

The normal test used:

```text
--breath-gain-db 0
```

## Notes For Plugin Integration

The plugin must not call this bridge from the audio thread.

ARA `1.01` and the plain VST3 recorder called the bridge from a background
worker and then parsed/cached the result for real-time-safe playback. ARA
`1.02` keeps the same real-time rule but replaces the bridge call with native
C++ analysis.
