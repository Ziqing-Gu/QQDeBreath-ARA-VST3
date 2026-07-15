# QQDeBreath ARA/VST3/AU 1.13

This local Plan A build improves editing and window interaction without changing analysis or audio rendering.

## Changes

- Added `Ctrl+Z` and `Ctrl+Shift+Z` for region Undo and Redo.
- Unified Global and Selected Auto Apply into one synchronized preference.
- Enabled resizing in both ARA and plain VST3 contexts and raised the maximum size limit.
- Added a compact two-row toolbar layout for narrow editor windows.
- Added left-button region-body dragging with neighbor and timeline constraints.
- Region moves are stored as one Undo step per drag.

The native C++ analyzer, embedded model, EQ DSP, monitoring, and stem rendering are unchanged.
