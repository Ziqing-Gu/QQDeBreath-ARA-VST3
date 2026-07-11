# QQDeBreath ARA/VST3 Plugin

Current plugin version: `ARA 1.12`

This plugin is the current QQDeBreath ARA/VST3 interactive prototype. It is a
unified VST3 bundle that can be used as:

- ARA Audio Extension source reader in Cubase/Nuendo.
- Plain VST3 recorder when ARA is not available.

## Analysis Backend

ARA 1.12 contains the Breath detection path in native C++ and retains the
multi-event ARA Load, dynamic Pre/Post spectrum, loop preview, Global Gain,
Set as Default, and per-Breath Adjust/EQ workflow. Its ARA playback renderer
is revised for Reaper: persistent state and playback parameters are revision
cached, lock acquisition never waits on the audio thread, Breath Norm peaks
are prepared outside playback, and unchanged EQ states no longer allocate or
serialize JSON per audio block.

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

- Re-arming Record keeps the existing timeline capture. Newly recorded samples
  overwrite overlapping positions and fill previously unrecorded positions.
- Recording storage is preallocated in large blocks to avoid per-audio-block
  buffer growth on the realtime thread.

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
  instances and for an explicit ARA `Load`.
- Clicking ARA `Load` applies the saved global defaults before loading the
  selected source. Per-Breath Gain/EQ and the previous analysis are cleared.
- Saved global defaults include Monitor Voice/Breath/Noize, Follow, Fade,
  Fade In/Out, Breath Norm, Breath Target, Global Gain, Global EQ, and the
  Global/Selected Auto Apply preferences.
- A confirmation dialog is shown before the default preset is overwritten.
- Per-Breath Adjust/EQ is not included in the default preset.
- DAW project state has priority during automatic project restore and editor
  reopen. QQDeBreath restores that project's saved audio source, regions,
  edits, EQ, Gain, and monitor state instead of overwriting them with the
  global default preset.
- The explicit `Load` command is the intentional reset boundary: it starts the
  newly loaded source from the saved global defaults.
- This is not the standalone app's global last-session recovery model.

Per-Breath Adjust/EQ:

- The purple Post spectrum includes the selected Breath Gain, while the Pre
  spectrum remains the level before local EQ/Gain.
- Dynamic Pre/Post spectra hold the peak of each displayed frequency band after
  playback stops.
- Global EQ and Selected Breath EQ each provide a `Clear` button. Selected
  Clear removes only the local EQ and preserves the local Gain.

1. Click a Breath region on the waveform.
2. Click `Adjust`.
3. Use the dedicated page to set that Breath region's Gain from -30 dB to +30 dB.
4. Enable the per-region Breath EQ when that specific Breath needs separate EQ.
5. Click the top-right `X` to return to the main waveform page.

## Important Notes

- Breath analysis is native C++ in ARA 1.12.
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
- Host mode is selected from the processor's actual ARA binding. A host merely
  exposing ARA-capable editor APIs no longer makes a regular VST3 instance show
  the ARA-only workflow.
- ARA playback resamples Audio Source samples to the current host playback
  sample rate. This avoids silent playback when the source and Reaper project
  use different sample rates.
- ARA Load and project restore preload each Audio Source into an immutable
  memory cache. Realtime playback reads this cache instead of asking the host
  ARA sample-access API for every audio block.
- ARA playback keeps a revision-cached source/region snapshot. If the UI is
  updating state, the audio thread keeps the previous snapshot instead of
  waiting for a lock.
- Breath Norm region peaks are computed from the loaded waveform on the
  message thread and saved with the ARA project state; playback never scans a
  whole Breath region to discover its peak.
- Unchanged Global/Selected EQ settings are compared structurally and do not
  create JSON strings in the realtime callback.
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
