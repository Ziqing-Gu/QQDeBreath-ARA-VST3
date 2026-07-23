# QQDeBreath ARA / VST3 / AU 1.15

## 中文

### 修复

同一插件实例关闭并重新打开编辑器后，Breath Norm 勾选状态与 Breath Target 数值不再被已保存的全局默认预设覆盖。

此前“是否已经应用全局默认值”的保护状态属于 Editor 对象。宿主关闭插件窗口时会销毁 Editor，重新打开窗口时又会创建新的 Editor，因此同一个 Processor 实例会被错误地当成新实例。1.15 将该保护状态移动到 Audio Processor，使其生命周期与插件实例一致。

### 保留行为

- 真正的新插件实例仍会应用用户通过 `Set as Default` 保存的全局预设。
- DAW 工程状态仍优先于全局默认值。
- ARA 工程播放参数继续绑定当前 ARA Document。
- VST3 与 AU 参数继续保存在 Processor APVTS 中。
- Global Gain、Global EQ、局部 Breath Gain/EQ 和区域编辑会按工程状态恢复。
- 分析、内嵌模型、监听 DSP、EQ、Fade、Gain 和三轨导出声音未改变。

### 回归验证

新增 `qq_debreath_editor_state_probe`，验证编辑器销毁并重建后，Norm、Target、Global Gain、Global EQ 以及选中 Breath 的 Gain/EQ 保持不变。

## English

### Fix

Closing and reopening the editor for the same plug-in instance no longer resets Breath Norm or Breath Target to the saved global default preset.

The “global defaults already applied” guard previously belonged to the Editor object. Hosts destroy that object when the plug-in window closes and create a new Editor when it reopens, so the same Processor instance was incorrectly treated as a fresh instance. Version 1.15 moves the guard to the Audio Processor, matching the lifetime of the actual plug-in instance.

### Preserved behavior

- A genuinely new plug-in instance still receives the global preset saved with `Set as Default`.
- DAW project state continues to take priority over global defaults.
- ARA playback parameters remain tied to the current ARA document.
- VST3 and AU parameter state remains stored in the Processor APVTS.
- Global Gain, Global EQ, per-Breath Gain/EQ, and region edits restore from project state.
- Analysis, embedded model data, monitoring DSP, EQ, Fade, Gain, and three-stem export audio are unchanged.

### Regression coverage

A new `qq_debreath_editor_state_probe` verifies that Norm, Target, Global Gain, Global EQ, and selected-Breath Gain/EQ survive Editor destruction and recreation.
