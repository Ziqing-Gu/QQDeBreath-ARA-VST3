# QQDeBreath ARA/VST3/AU 1.12

This update adds a macOS Audio Unit v2 build for Logic Pro while preserving
the existing Windows and macOS VST3 builds.

## Changes

- Added AU to the macOS JUCE formats for the shared QQDeBreath target.
- Kept Windows restricted to the existing VST3 target.
- Added a Universal 2 AU/ARA GitHub Actions job with arm64 and x86_64
  architecture verification.
- Added Logic Pro ARA 2 and Rosetta usage notes.
- Added a shared macOS installation guide for the Apple Silicon VST3, Intel
  VST3, and Universal 2 AU archives.

The analysis algorithm, model data, region behavior, rendering, and existing
VST3 workflow are unchanged.
