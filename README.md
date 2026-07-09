# QQDeBreath ARA/VST3 Plugin

Current plugin version: `ARA 1.06`

This plugin is the current QQDeBreath ARA/VST3 interactive prototype. It is a
unified VST3 bundle that can be used as:

- ARA Audio Extension source reader in Cubase/Nuendo.
- Plain VST3 recorder when ARA is not available.

## Analysis Backend

ARA 1.06 contains the Breath detection path in native C++ and improves the
Breath EQ workflow with multi-event ARA Load, dynamic Pre/Post spectrum, loop
preview, a main-page EQ enable checkbox, Global Gain, Set as Default, and
per-Breath Adjust/EQ controls.
It also lets newly double-clicked EQ bands be dragged immediately without a
second click.

Normal analysis no longer calls:

```text
qq_debreath_bridge.exe
QQDeBreathTool.exe analyze-for-plugin
```

The embedded native analyzer uses the same QQDeBreathTool 1.11 model data:

```text
D655AF2BFB260866DE74319D179FDA0B007E6539711C572967BC0FC709E42AFE
```

The old bridge remains in the CMake project only as a debug/compatibility tool.

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

Breath EQ:

1. Use the `EQ` checkbox on the main page to enable/disable Breath EQ.
2. Click `Breath EQ` on the main page.
3. Use the dedicated EQ page for HP/LP and up to six Bell bands.
4. Click near the left/right edge of the EQ graph to create HP/LP.
5. Use `Loop` to preview the current/next Breath region.
6. Click the top-right `X` to return to the main waveform page.

Global Gain:

- Global Gain controls the overall Breath component after optional Breath Norm.
- Range: -60 dB to +30 dB.
- The same Global Gain control is available on the main page and the Global EQ
  page.

Set as Default:

- `Set as Default` saves the current global startup preset for new ARA/VST3
  instances.
- Saved global defaults include Monitor Voice/Breath/Noize, Follow, Fade,
  Fade In/Out, Breath Norm, Breath Target, Global Gain, and Global EQ.
- Per-Breath Adjust/EQ is not included in the default preset.
- DAW project state always has priority. When a Cubase/Nuendo project is
  reopened, QQDeBreath restores that project's saved audio source, regions,
  edits, EQ, Gain, and monitor state instead of overwriting them with the
  global default preset.
- This is not the standalone app's global last-session recovery model.

Per-Breath Adjust/EQ:

1. Click a Breath region on the waveform.
2. Click `Adjust`.
3. Use the dedicated page to set that Breath region's Gain from -30 dB to +30 dB.
4. Enable the per-region Breath EQ when that specific Breath needs separate EQ.
5. Click the top-right `X` to return to the main waveform page.

## Important Notes

- Breath analysis is native C++ in ARA 1.06.
- C++ region output was compared against the Python 1.11 implementation on
  representative vocal samples.
- Noize remains a first-class manual region type in the UI and renderer.
- Breath EQ only processes the Breath component. Voice, Noize, detection, and
  region boundaries are not changed by EQ.
- The global default preset is only used for fresh plugin instances that have
  no DAW-saved state yet.
- ARA `Load` can combine multiple selected events into a temporary composite
  waveform when the selected sources use the same sample rate. The resulting
  regions are also mapped back to the individual ARA sources for playback.
- Audio thread code must not run analysis; analysis runs on the background
  analysis thread.

## Build

Build and install the unified VST3/ARA plugin:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\plugin\build-ara-vst3.ps1
```

The local build output is under:

```text
plugin/build-vs/QQDeBreath_artefacts/Release/VST3/QQDeBreath.vst3
```

The installed VST3 path is usually:

```text
C:\Program Files\Common Files\VST3\QQDeBreath.vst3
```
