# QQDeBreath ARA 1.06 Release Notes

ARA 1.06 is a focused Breath EQ interaction polish release.

## Changes

- Double-clicking the EQ graph to create a Bell band now immediately starts
  dragging that newly created band while the mouse button remains held.
- Renamed the global Breath `Adjust` control to `Global Gain`.
- Expanded Global Gain range to -60 dB through +30 dB.
- Added the same Global Gain control to the Global EQ page.
- Added `Set as Default` for ARA/VST3 global startup defaults.
- Updated plugin metadata and UI version labels to ARA 1.06.

## Notes

- No analysis algorithm changes.
- No model changes.
- Per-Breath Adjust/EQ is not included in `Set as Default`.
- DAW project state remains authoritative when reopening a saved project; global
  defaults are only applied to fresh plugin instances without saved state.
- No rendering or export logic changes.
