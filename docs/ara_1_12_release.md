# QQDeBreath ARA 1.12 Release Notes

ARA 1.12 addresses persistent crackle during Reaper ARA playback after Load.
The reported failure occurs before analysis and EQ processing, so the fix
targets the ARA source-delivery path rather than the Breath algorithm.

## Realtime Source Cache

- Single-source ARA Load preloads the complete Audio Source on the message
  thread.
- Multi-event Load preloads every unique underlying Audio Source.
- Automatic project restoration rebuilds the source cache before normal
  editor-driven playback.
- The DocumentController owns immutable shared audio buffers keyed by source
  fingerprint.
- The ARA playback renderer takes a revision-cached shared snapshot and reads
  contiguous memory during realtime playback.
- Reaper ARA sample access is no longer called once per audio block after a
  successful Load.
- If preloading fails, Load reports a clear error instead of silently using the
  known-problematic path.

## Retained Behavior

- ARA 1.11 explicit Load/default behavior is retained.
- ARA 1.10 non-blocking state caches and precomputed Breath Norm peaks remain.
- Source-rate conversion, Fade, Norm, Global/Selected Gain and EQ are unchanged.
- The native analyzer and QQDeBreathTool 1.11 model are unchanged.

## Validation

- Windows Release unified VST3/ARA target builds and installs successfully.
- Build and installed binary hashes match.
- Final Reaper confirmation must be performed by the reporting user.