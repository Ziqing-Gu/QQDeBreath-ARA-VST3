#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

#include "Parameters.h"
#include "shared/BridgeAnalysis.h"
#include "shared/BreathEq.h"

struct QQDeBreathARASourceInfo
{
    struct PlaybackMapping
    {
        juce::String sourceName;
        juce::String persistentId;
        juce::String sourceFingerprint;
        double sourceSampleRate = 0.0;
        int sourceChannelCount = 0;
        juce::int64 sourceNumSamples = 0;
        double compositeStartSeconds = 0.0;
        double compositeEndSeconds = 0.0;
        double sourceStartSeconds = 0.0;
    };

    juce::String name;
    juce::String persistentId;
    juce::String sourceFingerprint;
    bool isComposite = false;
    double compositePlaybackStartSeconds = 0.0;
    double compositePlaybackEndSeconds = 0.0;
    double sampleRate = 0.0;
    int channelCount = 0;
    juce::int64 numSamples = 0;
    double durationSeconds = 0.0;
    juce::File exportedWav;
    juce::String exportStatus = "No ARA source loaded.";
    juce::Array<PlaybackMapping> playbackMappings;
};

struct QQDeBreathARAPersistentState
{
    QQDeBreathARASourceInfo sourceInfo;
    QQDeBreathBridgeAnalysisResult analysisResult;
};

struct QQDeBreathARAPlaybackParams
{
    bool monitorVoice = true;
    bool monitorBreath = true;
    bool monitorNoize = true;
    bool followPlayhead = false;
    bool enableFade = true;
    bool normalizeBreath = false;
    double fadeInMs = 10.0;
    double fadeOutMs = 10.0;
    double breathTargetDb = -6.0;
    double breathGainDb = 0.0;
    double waveformDisplayGain = 1.0;
    QQDeBreathEqState breathEqState;
};

class QQDeBreathAudioProcessor final : public juce::AudioProcessor,
                                       private juce::AudioProcessorARAExtension
{
public:
    struct RecordedBufferInfo
    {
        bool isRecordArmed = false;
        bool isRecording = false;
        bool hasRecording = false;
        double sampleRate = 0.0;
        int channelCount = 0;
        juce::int64 numSamples = 0;
        double durationSeconds = 0.0;
        double recordingStartTimelineSeconds = -1.0;
        juce::String sourceFingerprint;
        juce::String status;
    };

    QQDeBreathAudioProcessor();
    ~QQDeBreathAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "QQDeBreath"; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override;

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    bool hasRestoredStateInformation() const noexcept { return restoredStateInformation.load(std::memory_order_acquire); }

    void startRecording();
    void stopRecording();
    void clearRecording();
    RecordedBufferInfo getRecordedBufferInfo() const;
    bool copyRecordedBuffer(juce::AudioBuffer<float>& dest, double& sampleRate) const;
    bool tryCopyRecordedBuffer(juce::AudioBuffer<float>& dest, double& sampleRate) const;
    bool exportRecordedBufferToTempWav(juce::File& exportedFile, juce::String& status) const;
    bool exportRecordedBufferToWav(const juce::File& outputFile, juce::String& status) const;
    void setAnalysisResult(const QQDeBreathBridgeAnalysisResult& result);
    void updateAnalysisRegionsPreservingCaches(const juce::Array<QQDeBreathBridgeRegion>& regions);
    QQDeBreathBridgeAnalysisResult getAnalysisResult() const;
    void clearAnalysisResult();
    void setAraSourceInfo(const QQDeBreathARASourceInfo& info);
    QQDeBreathARASourceInfo getAraSourceInfo() const;
    void clearAraSourceInfo();
    QQDeBreathEqState getBreathEqState() const;
    void setBreathEqState(const QQDeBreathEqState& state);
    void applyBreathEqToBuffer(juce::AudioBuffer<float>& buffer, double sampleRate) const;
    void setInternalPreviewPosition(double localSeconds, double hostTimeSeconds);
    void setInternalPreviewLoopRange(double startSeconds, double endSeconds, double hostTimeSeconds);
    void clearInternalPreviewLoop();
    void clearInternalPreviewPosition();
    double getInternalPreviewPosition(double hostTimeSeconds) const;
    juce::AudioProcessorARAExtension* getARAClientExtensions() override { return this; }
    bool isBoundToAraHost() const noexcept { return isBoundToARA(); }
    bool hasAraPlaybackRendererRole() const noexcept { return isPlaybackRenderer(); }

    juce::AudioProcessorValueTreeState parameters;

private:
    void appendToRecordedBuffer(const juce::AudioBuffer<float>& buffer, double hostTimeSeconds);
    bool renderPreviewBlock(juce::AudioBuffer<float>& buffer, double hostTimeSeconds);
    double getInternalPreviewPositionUnlocked(double hostTimeSeconds) const;
    juce::Array<double> buildRegionPeakCacheForResult(const QQDeBreathBridgeAnalysisResult& result) const;
    bool exportRecordedBufferToWavUnchecked(const juce::File& outputFile, juce::String& status) const;
    static juce::File getRecordingExportDirectory();

    mutable juce::CriticalSection recordedBufferLock;
    juce::AudioBuffer<float> recordedBuffer;
    juce::int64 recordedLengthSamples = 0;
    double recordedSampleRate = 0.0;
    double recordingStartTimelineSeconds = -1.0;
    std::atomic<bool> recordArmed { false };
    std::atomic<bool> recording { false };
    std::atomic<bool> restoredStateInformation { false };
    std::atomic<int> droppedRecordBlocks { 0 };
    std::atomic<bool> internalPreviewActive { false };
    std::atomic<bool> internalPreviewLoopEnabled { false };
    std::atomic<double> internalPreviewAnchorLocalSeconds { 0.0 };
    std::atomic<double> internalPreviewAnchorHostSeconds { 0.0 };
    std::atomic<double> internalPreviewLoopStartSeconds { 0.0 };
    std::atomic<double> internalPreviewLoopEndSeconds { 0.0 };
    juce::AudioBuffer<float> araDryBuffer;
    juce::AudioBuffer<float> previewBreathBuffer;
    QQDeBreathEqProcessor vst3BreathEqProcessor;
    QQDeBreathEqProcessor vst3RegionEqProcessor;

    mutable juce::CriticalSection analysisLock;
    QQDeBreathBridgeAnalysisResult analysisResult;
    juce::Array<double> analysisRegionPeakCache;
    mutable juce::CriticalSection breathEqLock;
    QQDeBreathEqState breathEqState;
    mutable juce::CriticalSection araSourceInfoLock;
    QQDeBreathARASourceInfo araSourceInfo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(QQDeBreathAudioProcessor)
};

class QQDeBreathARADocumentController final : public juce::ARADocumentControllerSpecialisation
{
public:
    using juce::ARADocumentControllerSpecialisation::ARADocumentControllerSpecialisation;

    void upsertPersistentState(const QQDeBreathARASourceInfo& sourceInfo,
                               const QQDeBreathBridgeAnalysisResult& analysisResult);
    void updateRuntimePersistentState(const QQDeBreathARASourceInfo& sourceInfo,
                                      const QQDeBreathBridgeAnalysisResult& analysisResult);
    bool getPersistentStateForSource(const juce::String& sourceFingerprint,
                                     QQDeBreathARAPersistentState& state) const;
    bool getFirstPersistentState(QQDeBreathARAPersistentState& state) const;
    void setPlaybackParams(const QQDeBreathARAPlaybackParams& params);
    QQDeBreathARAPlaybackParams getPlaybackParams() const;
    bool hasRestoredPlaybackParams() const noexcept { return restoredPlaybackParams.load(std::memory_order_acquire); }

protected:
    juce::ARAPlaybackRenderer* doCreatePlaybackRenderer() override;
    bool doRestoreObjectsFromStream(juce::ARAInputStream& input, const juce::ARARestoreObjectsFilter* filter) noexcept override;
    bool doStoreObjectsToStream(juce::ARAOutputStream& output, const juce::ARAStoreObjectsFilter* filter) noexcept override;

private:
    mutable juce::CriticalSection persistentStateLock;
    juce::Array<QQDeBreathARAPersistentState> persistentStates;
    mutable juce::CriticalSection playbackParamsLock;
    QQDeBreathARAPlaybackParams playbackParams;
    std::atomic<bool> restoredPlaybackParams { false };
};
