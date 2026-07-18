# QQDeBreath ARA/VST3/AU 1.14

This local Plan A build ports the ARA monitoring fix that previously removed the same crackle failure from QQEasyTool.

## Root Cause

QQDeBreath 1.13 kept a block-level fallback that restored the complete dry input whenever the ARA renderer produced a silent block. Silence can be intentional after Voice/Breath/Noise routing. Replacing it with unrelated dry input creates discontinuities at audio-block boundaries and can be heard as crackle.

## Changes

- ARA renderer output is now authoritative; intentional silence is no longer replaced by dry input.
- A source without analysis state is classified as Voice and follows `Monitor Voice`.
- Composite ARA mappings persist explicit empty-region states.
- Clearing analysis propagates to mapped persistent and realtime state, preventing stale routing from surviving.

The native analyzer, embedded QQDeBreathTool 1.11 model, EQ, Fade, Breath Norm, Global/Selected Gain, waveform editing, and stem export are unchanged.

## Host Validation

1. Load the same reported audio in ARA mode and play through previously noisy sections.
2. Test before and after Analyze.
3. Toggle Voice/Breath/Noise individually and in combinations.
4. Clear Analysis and confirm the source returns to Voice-only classification without crackle.
5. Save, close, reopen the project, then repeat playback.
6. Also verify plain VST3 recording/monitoring remains clean.