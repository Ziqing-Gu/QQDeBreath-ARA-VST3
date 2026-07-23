#include "PluginProcessor.h"

#include <juce_gui_basics/juce_gui_basics.h>

#include <cmath>
#include <iostream>
#include <memory>

namespace
{
bool setParameter(juce::AudioProcessorValueTreeState& state, const char* id, float value)
{
    auto* parameter = state.getParameter(id);
    if (parameter == nullptr)
        return false;

    parameter->setValueNotifyingHost(parameter->convertTo0to1(value));
    return true;
}

bool near(double actual, double expected, double tolerance = 0.001)
{
    return std::abs(actual - expected) <= tolerance;
}
} // namespace

#if JUCE_WINDOWS
int wmain()
#else
int main()
#endif
{
    juce::ScopedJuceInitialiser_GUI juceInitialiser;
    QQDeBreathAudioProcessor processor;

    auto firstEditor = std::unique_ptr<juce::AudioProcessorEditor>(processor.createEditor());
    if (firstEditor == nullptr)
    {
        std::cerr << "FAIL: first editor was not created\n";
        return 1;
    }

    if (! setParameter(processor.parameters, QQDeBreath::ParamIDs::normalizeBreath, 1.0f)
        || ! setParameter(processor.parameters, QQDeBreath::ParamIDs::breathTargetDb, -37.3f)
        || ! setParameter(processor.parameters, QQDeBreath::ParamIDs::breathGainDb, 7.2f))
    {
        std::cerr << "FAIL: required parameter was not found\n";
        return 1;
    }

    QQDeBreathEqState expectedEq;
    expectedEq.enabled = true;
    expectedEq.highPassEnabled = true;
    expectedEq.highPassHz = 173.0;
    expectedEq.highPassSlopeDbPerOct = 24;
    expectedEq.bands[0].enabled = true;
    expectedEq.bands[0].frequencyHz = 1840.0;
    expectedEq.bands[0].gainDb = -4.7;
    expectedEq.bands[0].q = 1.8;
    processor.setBreathEqState(expectedEq);

    QQDeBreathEqState expectedSelectedEq;
    expectedSelectedEq.enabled = true;
    expectedSelectedEq.lowPassEnabled = true;
    expectedSelectedEq.lowPassHz = 9230.0;
    expectedSelectedEq.lowPassSlopeDbPerOct = 36;
    expectedSelectedEq.bands[1].enabled = true;
    expectedSelectedEq.bands[1].frequencyHz = 3270.0;
    expectedSelectedEq.bands[1].gainDb = 3.4;
    expectedSelectedEq.bands[1].q = 2.1;

    QQDeBreathBridgeAnalysisResult expectedAnalysis;
    expectedAnalysis.hasResult = true;
    expectedAnalysis.succeeded = true;
    expectedAnalysis.sampleRate = 48000;
    expectedAnalysis.channels = 1;
    expectedAnalysis.numSamples = 96000;
    expectedAnalysis.durationSeconds = 2.0;
    expectedAnalysis.breathCount = 1;
    QQDeBreathBridgeRegion expectedRegion;
    expectedRegion.type = "Breath";
    expectedRegion.startSample = 12000;
    expectedRegion.endSample = 24000;
    expectedRegion.startTime = 0.25;
    expectedRegion.endTime = 0.5;
    expectedRegion.gainDb = -8.6;
    expectedRegion.eqState = expectedSelectedEq;
    expectedAnalysis.regions.add(expectedRegion);
    processor.setAnalysisResult(expectedAnalysis);

    firstEditor.reset();
    auto reopenedEditor = std::unique_ptr<juce::AudioProcessorEditor>(processor.createEditor());
    if (reopenedEditor == nullptr)
    {
        std::cerr << "FAIL: reopened editor was not created\n";
        return 1;
    }

    const auto norm = processor.parameters.getRawParameterValue(QQDeBreath::ParamIDs::normalizeBreath)->load();
    const auto target = processor.parameters.getRawParameterValue(QQDeBreath::ParamIDs::breathTargetDb)->load();
    const auto gain = processor.parameters.getRawParameterValue(QQDeBreath::ParamIDs::breathGainDb)->load();
    const auto actualEq = processor.getBreathEqState();
    const auto actualAnalysis = processor.getAnalysisResult();
    const auto selectedRegionPreserved = actualAnalysis.regions.size() == 1
                                      && near(actualAnalysis.regions[0].gainDb, -8.6)
                                      && actualAnalysis.regions[0].eqState.enabled
                                      && actualAnalysis.regions[0].eqState.lowPassEnabled
                                      && near(actualAnalysis.regions[0].eqState.lowPassHz, 9230.0)
                                      && actualAnalysis.regions[0].eqState.lowPassSlopeDbPerOct == 36
                                      && actualAnalysis.regions[0].eqState.bands[1].enabled
                                      && near(actualAnalysis.regions[0].eqState.bands[1].frequencyHz, 3270.0)
                                      && near(actualAnalysis.regions[0].eqState.bands[1].gainDb, 3.4)
                                      && near(actualAnalysis.regions[0].eqState.bands[1].q, 2.1);

    const auto passed = norm >= 0.5f
                     && near(target, -37.3)
                     && near(gain, 7.2)
                     && actualEq.enabled
                     && actualEq.highPassEnabled
                     && near(actualEq.highPassHz, 173.0)
                     && actualEq.highPassSlopeDbPerOct == 24
                     && actualEq.bands[0].enabled
                     && near(actualEq.bands[0].frequencyHz, 1840.0)
                     && near(actualEq.bands[0].gainDb, -4.7)
                     && near(actualEq.bands[0].q, 1.8)
                     && selectedRegionPreserved;

    if (! passed)
    {
        std::cerr << "FAIL: reopening the editor changed live instance state\n"
                  << "  norm=" << norm << " target=" << target << " gain=" << gain << "\n"
                  << "  eqEnabled=" << actualEq.enabled << " hp=" << actualEq.highPassEnabled
                  << " hpHz=" << actualEq.highPassHz << " bandGain=" << actualEq.bands[0].gainDb << "\n";
        return 1;
    }

    std::cout << "PASS: Norm, Target, Global Gain, Global EQ, and selected Breath Gain/EQ survived editor recreation\n";
    return 0;
}
