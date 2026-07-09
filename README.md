# QQDeBreath ARA/VST3

QQDeBreath ARA/VST3 is an interactive de-breath plugin for separating and
editing vocal Breath / Noise regions inside a DAW. It can read audio through
ARA in Cubase/Nuendo, or work as a plain VST3 recorder when ARA is not
available.

The plugin analyzes vocal audio, displays the waveform, lets the user edit
Breath / Noise regions manually, monitors Voice / Breath / Noise components,
and provides global and per-region Breath EQ controls for shaping the separated
Breath component. In the plugin UI, the Noise component is labeled `Noize` for
compatibility with earlier QQDeBreath project data.

> Non-commercial source release. QQ group: 692973169.

## Version

Current plugin version: `ARA 1.06`

ARA 1.06 contains the native C++ Breath detection path and focuses on the Breath
EQ workflow: multi-event ARA Load, dynamic Pre/Post spectrum display, loop
preview, a main-page EQ enable checkbox, Global Gain, Set as Default, and
per-Breath Adjust/EQ controls.

The embedded native analyzer uses the same QQDeBreathTool 1.11 model data:

```text
D655AF2BFB260866DE74319D179FDA0B007E6539711C572967BC0FC709E42AFE
```

Normal analysis no longer calls:

```text
qq_debreath_bridge.exe
QQDeBreathTool.exe analyze-for-plugin
```

The old bridge remains in the CMake project only as a debug and compatibility
tool.

## About

QQDeBreath ARA/VST3 is the plugin version of QQDeBreathTool, a de-breath and
noise separation utility developed by mixing engineer Gu Ziqing through Codex
with ChatGPT, with compilation and bug-fix assistance from programmer Diao
Xiangyu.

The current Breath model was trained on song excerpts and vocal samples from
multiple singers. Most recordings were captured in professional studios, a
smaller portion in home recording environments, and a subset of samples feature
edge cases including heavy breathiness and severe DC offset distortion.

Special thanks to Jason for contributing additional vocal training samples used
to improve the 1.10 Breath detector inherited by this plugin.

## Features

- ARA Audio Extension source loading in Cubase/Nuendo.
- Plain VST3 recording workflow when ARA is not available.
- Native C++ Breath analysis using the QQDeBreathTool 1.11 model data.
- Waveform display with editable Breath / Noise regions.
- Multi-event ARA Load for selected events with the same sample rate.
- Manual region creation, boundary editing, deletion, and Breath/Noize toggling.
- Voice / Breath / Noize monitor checkboxes.
- Fade In / Fade Out controls for smoother component playback.
- Optional Breath normalization and Breath target level.
- Global Gain control for the separated Breath component.
- Global startup defaults for fresh plugin instances.
- DAW project-state restore for audio source, regions, edits, EQ, Gain, and
  monitor state.

## Breath EQ

Breath EQ only processes the separated Breath component. It does not change the
Voice component, Noize component, detection result, or region boundaries.

Global Breath EQ:

- Use the `EQ` checkbox on the main page to enable or bypass global Breath EQ.
- Click `Breath EQ` to open the dedicated EQ page.
- Use HP/LP filters and up to six Bell bands.
- Click near the left or right edge of the EQ graph to create HP/LP filters.
- Double-click the EQ graph to create a Bell band.
- Newly created bands can be dragged immediately.
- Use the dynamic Pre/Post spectrum display to compare the original Breath
  signal with the processed result.
- Use `Loop` to preview the current or next Breath region while adjusting EQ.
- Use `Global Gain` with EQ to control the final Breath level.
- Click `Set as Default` to save the global EQ and related startup settings for
  new plugin instances.

Per-Breath Adjust/EQ:

- Select a Breath region on the waveform.
- Click `Adjust`.
- Set that region's Breath Gain from -30 dB to +30 dB.
- Enable per-region Breath EQ when one Breath needs separate correction.
- Return to the main waveform page with the top-right `X`.

The global EQ is intended for the overall Breath tone. Per-Breath EQ is intended
for isolated breaths that need special treatment, such as harsh high-frequency
air, low-frequency bumps, or breaths recorded at a different distance from the
microphone.

## Current Workflow

ARA mode:

1. Apply QQDeBreath as a Cubase/Nuendo Audio Extension.
2. Select one or more ARA audio events, then click `Load`.
3. Click `Analyze`.
4. Edit Breath / Noize regions on the waveform.
5. Monitor Voice / Breath / Noize checkboxes.
6. Adjust Fade, Breath Norm, Global Gain, Breath Adjust, and optional Breath EQ.

Plain VST3 mode:

1. Insert QQDeBreath as a VST3 plugin.
2. Click `Record`.
3. Play the DAW timeline; recording starts with DAW playback.
4. Stop DAW playback; recording stops.
5. Click `Analyze`.
6. Edit and monitor regions.

## Repository Contents

- `CMakeLists.txt` - JUCE/CMake plugin project.
- `src/` - VST3/ARA plugin source.
- `src/ara/` - ARA reader/editor integration.
- `src/shared/` - shared analysis, EQ, waveform, and model code.
- `bridge/` - legacy/debug bridge helper.
- `native_probe/` - native analyzer probe tool.
- `external/ARA_SDK` - Celemony ARA SDK submodule.
- `docs/` - build notes and historical release notes.
- `build-ara-vst3.ps1` - Windows build/install helper.
- `build-bridge.ps1` - bridge helper build script.

Local build folders, generated Visual Studio projects, installed VST3 bundles,
private samples, and exported audio files are intentionally not included.

## Build

Clone with submodules:

```powershell
git clone --recurse-submodules https://github.com/Ziqing-Gu/QQDeBreath-ARA-VST3.git
cd QQDeBreath-ARA-VST3
```

Build and install the unified VST3/ARA plugin on Windows:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\build-ara-vst3.ps1
```

The local build output is under:

```text
build-vs\QQDeBreath_artefacts\Release\VST3\QQDeBreath.vst3
```

The installed VST3 path is usually:

```text
C:\Program Files\Common Files\VST3\QQDeBreath.vst3
```

## Non-Commercial License Notice

This repository is released for learning, review, personal use, research, and
non-commercial audio production workflows only.

You may read, study, modify, and build local copies for personal or internal
non-commercial use. You may also share non-commercial modifications with
attribution.

You may not sell this software or derivative versions, include it in a
commercial product or paid service, remove the non-commercial notice, or use
the QQDeBreathTool / QQDeBreath name, icon, or author attribution to imply
endorsement of a modified build.

Commercial licensing or redistribution requires explicit written permission
from the copyright holder.

See [`LICENSE`](LICENSE) for the full license text.
