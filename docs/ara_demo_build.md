# Phase 5.5 ARA Demo Build

Phase 5.5 validates the ARA toolchain before adding ARA code to QQDeBreath.

This phase does not modify the QQDeBreath plugin, does not call Python, does
not run the QQDeBreath algorithm, and does not implement export or analysis.

## Result

Status: PASS

The JUCE `ARAPluginDemo` VST3 target was configured and built successfully on
2026-07-01.

The generated VST3 manifest contains:

```text
Category: ARA Main Factory Class
Name: ARAPluginDemo
SDKVersion: VST 3.8.0
```

This confirms that the built example is an ARA-capable VST3 plugin.

## SDK Versions And Paths

JUCE:

```text
plugin/build-vs/_deps/juce-src
JUCE version: 8.0.13
```

ARA SDK:

```text
plugin/external/ARA_SDK
ARA SDK tag: releases/2.3.0
ARA SDK commit: 0005c532bb03eb210e7bb490329fc6d7ba1792c7
```

ARA SDK submodules:

```text
ARA_API: 65ec5c43b943a48cb5446f448a0492db6af8534b
ARA_Library: d18a6a5e489816316be84a9de0eaf7307bc1abe4
ARA_Examples: abd7c8aa5854591995e1fbf16f854c65b0998e8d
ARA_Examples/3rdParty/ICST_AudioFile: 148e3c7828ebbe0ddafd29d83e67f91b9458b1bd
ARA_Examples/3rdParty/cpp-base64: 07ae5045d67b5bf6ffb46646b8ac2370eff1ae3e
ARA_Examples/3rdParty/pugixml: 08b3433180727ea2f78fe02e860a08471db1e03c
```

## Example Project

The built example is the JUCE ARA PIP:

```text
plugin/build-vs/_deps/juce-src/examples/Plugins/ARAPluginDemo.h
```

CMake target:

```text
ARAPluginDemo_VST3
```

## Build Script

The reproducible build script is:

```text
plugin/build-ara-demo.ps1
```

Run from the repository root:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\plugin\build-ara-demo.ps1
```

The script uses:

```text
Visual Studio Build Tools: C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools
vcvars64.bat: C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat
CMake: C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
Generator: Visual Studio 17 2022
Platform: x64
Configuration: Release
```

## CMake Configuration

Source:

```text
plugin/build-vs/_deps/juce-src
```

Build directory:

```text
plugin/ara-demo-build
```

Important CMake options:

```text
-DJUCE_BUILD_EXAMPLES=ON
-DJUCE_BUILD_EXTRAS=OFF
-DJUCE_COPY_PLUGIN_AFTER_BUILD=OFF
-DJUCE_GLOBAL_ARA_SDK_PATH=plugin/external/ARA_SDK
```

JUCE uses `JUCE_GLOBAL_ARA_SDK_PATH` to locate the ARA SDK. The path must point
to the ARA SDK umbrella directory that contains:

```text
ARA_API/
ARA_Library/
ARA_Examples/
```

The VST3 SDK used by this JUCE build is JUCE's bundled VST3 SDK under:

```text
plugin/build-vs/_deps/juce-src/modules/juce_audio_processors_headless/format_types/VST3_SDK
```

## Output Path

Built VST3 bundle:

```text
plugin/ara-demo-build/examples/Plugins/ARAPluginDemo_artefacts/Release/VST3/ARAPluginDemo.vst3
```

Built Windows binary:

```text
plugin/ara-demo-build/examples/Plugins/ARAPluginDemo_artefacts/Release/VST3/ARAPluginDemo.vst3/Contents/x86_64-win/ARAPluginDemo.vst3
```

Binary size observed:

```text
7028224 bytes
```

## Install Path

Installed system VST3 bundle:

```text
C:\Program Files\Common Files\VST3\ARAPluginDemo.vst3
```

Installed binary:

```text
C:\Program Files\Common Files\VST3\ARAPluginDemo.vst3\Contents\x86_64-win\ARAPluginDemo.vst3
```

Manual install command:

```powershell
$src = Resolve-Path -LiteralPath ".\plugin\ara-demo-build\examples\Plugins\ARAPluginDemo_artefacts\Release\VST3\ARAPluginDemo.vst3"
$dst = Join-Path $env:COMMONPROGRAMFILES "VST3\ARAPluginDemo.vst3"
if (Test-Path -LiteralPath $dst) { Remove-Item -LiteralPath $dst -Recurse -Force }
Copy-Item -LiteralPath $src -Destination $dst -Recurse -Force
```

## Cubase Manual Verification

Codex cannot fully verify Cubase behavior. The user must confirm these points
inside Cubase or Nuendo:

1. Open Cubase.
2. Open Studio > VST Plug-in Manager.
3. Confirm that Cubase scans `C:\Program Files\Common Files\VST3`.
4. Rescan plugins if needed.
5. Confirm that `ARAPluginDemo` appears as a plugin.
6. Import or record an audio event onto an audio track.
7. Select the audio event.
8. Open the Audio Extensions menu for that event.
9. Confirm that `ARAPluginDemo` appears in the Audio Extensions list.
10. Apply `ARAPluginDemo` to the audio event.
11. Confirm the plugin editor opens.
12. Save the Cubase project.
13. Close and reopen the Cubase project.
14. Confirm the project reopens without a crash.
15. Confirm the ARA extension remains stable after reopening.

Phase 6 may start only after the user confirms the Cubase-side checklist.

## Issues Encountered

### Git Submodule Tooling

The initial command was:

```powershell
git clone --recursive --branch releases/2.3.0 https://github.com/Celemony/ARA_SDK .\plugin\external\ARA_SDK
```

The top-level repository cloned, but submodule initialization initially failed
because Git's Unix helper tools such as `basename` and `sed` were not available
on `PATH`.

Fix:

```powershell
$env:PATH = "C:\Program Files\Git\usr\bin;" + $env:PATH
git -c safe.directory=<absolute ARA_SDK path> -C .\plugin\external\ARA_SDK submodule update --init --recursive --progress
```

### Missing ARAVST3.h

The first build failed with:

```text
Cannot open include file: ARA_API/ARAVST3.h
```

Cause:

```text
ARA_API was still an empty/uninitialized submodule.
```

Fix:

```text
Initialize all ARA SDK submodules, then rerun plugin/build-ara-demo.ps1.
```

### WebView2 Warning

CMake emitted a WebView2 warning while configuring JUCE examples. This warning
only disabled `WebViewPluginDemo` and did not affect `ARAPluginDemo`.

### Code Page Warnings

MSVC emitted C4819 warnings from JUCE/HarfBuzz sources under code page 936.
These warnings were non-fatal and did not prevent VST3 generation.

## Phase 6 Gate

Technical build gate:

```text
PASS: ARAPluginDemo_VST3 built successfully.
PASS: moduleinfo.json contains ARA Main Factory Class.
PASS: ARAPluginDemo.vst3 installed to the system VST3 folder.
```

Manual DAW gate:

```text
PENDING: Cubase scans ARAPluginDemo.
PENDING: Cubase lists ARAPluginDemo in Audio Extensions.
PENDING: ARAPluginDemo can be applied to an audio event.
PENDING: Editor opens.
PENDING: Project save/reopen is stable.
```
