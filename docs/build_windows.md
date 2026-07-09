# Build QQDeBreath VST3 On Windows

## Requirements

Install:

1. Visual Studio 2022 with the `Desktop development with C++` workload.
2. CMake 3.22 or newer.
3. Git, if you plan to clone JUCE.
4. JUCE 7 or newer.

In this workspace, CMake is not on `PATH`, but Visual Studio Build Tools
provides CMake here:

```text
C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
```

Use this full path if `cmake` is not recognized in PowerShell.

JUCE was not found in the current workspace during Phase 5. Configure stops at
`find_package(JUCE)` until JUCE is installed or placed in `plugin/external/JUCE`.

## JUCE Setup

Use one of these options.

Option A: place JUCE inside the plugin tree:

```powershell
cd plugin\external
git clone https://github.com/juce-framework/JUCE.git
```

Expected local path:

```text
plugin/external/JUCE/CMakeLists.txt
```

Option B: keep JUCE elsewhere and pass `JUCE_DIR` to CMake:

```powershell
cmake -S plugin -B plugin\build -G "Visual Studio 17 2022" -A x64 -DJUCE_DIR="D:\SDKs\JUCE"
```

`JUCE_DIR` should point to a JUCE install/export that provides JUCE's CMake
package, or use Option A.

## Configure Visual Studio Project

From the repository root:

```powershell
cmake -S plugin -B plugin\build -G "Visual Studio 17 2022" -A x64
```

If JUCE is not in `plugin/external/JUCE`, pass `-DJUCE_DIR=...` as shown above.

If `cmake` is not on `PATH`, use:

```powershell
& "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" `
  -S plugin `
  -B plugin\build `
  -G "Visual Studio 17 2022" `
  -A x64
```

## Build

```powershell
cmake --build plugin\build --config Release --target QQDeBreath_VST3
```

If using the Visual Studio bundled CMake:

```powershell
& "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" `
  --build plugin\build `
  --config Release `
  --target QQDeBreath_VST3
```

You can also open:

```text
plugin/build/QQDeBreath.sln
```

and build the `QQDeBreath_VST3` target in Visual Studio.

## VST3 Output Path

The expected Release output path is typically:

```text
plugin/build/QQDeBreath_artefacts/Release/VST3/QQDeBreath.vst3
```

Exact paths can vary slightly by JUCE/CMake version. Search under:

```text
plugin/build/QQDeBreath_artefacts
```

## Install To The Windows VST3 Folder

Common user-wide VST3 folder:

```text
%COMMONPROGRAMFILES%\VST3
```

Typical full path:

```text
C:\Program Files\Common Files\VST3
```

Copy the built plugin folder:

```powershell
Copy-Item -Recurse -Force `
  "plugin\build\QQDeBreath_artefacts\Release\VST3\QQDeBreath.vst3" `
  "$env:COMMONPROGRAMFILES\VST3\QQDeBreath.vst3"
```

Administrator permission may be required.

## DAW Scan

Cubase / Nuendo:

1. Copy `QQDeBreath.vst3` to the VST3 folder.
2. Open Cubase.
3. Open Studio > VST Plug-in Manager.
4. Rescan plug-ins if needed.
5. Insert `QQDeBreath` on an audio track.
6. Confirm the editor opens and displays:

```text
QQDeBreath
Phase 5 VST3 Shell
Audio passthrough only
```

Other DAWs:

1. Ensure the DAW scans `C:\Program Files\Common Files\VST3`.
2. Rescan plug-ins.
3. Insert `QQDeBreath` on an audio track.

## Phase 5 Pass Criteria

Phase 5 passes when:

1. CMake configure succeeds.
2. Visual Studio build succeeds.
3. `QQDeBreath.vst3` is generated.
4. The plugin loads without crashing.
5. Audio passes through unchanged.
6. Placeholder parameters save and restore in the DAW project.
7. The editor opens.
