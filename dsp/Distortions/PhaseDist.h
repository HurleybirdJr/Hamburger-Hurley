#pragma once

#include "../SmoothParam.h"
#include "WaveShapers.h"

#ifdef DEBUG
#include <melatonin_perfetto/melatonin_perfetto.h>
#endif

//==============================================================================
/*
 */
class PhaseDist
{
public:
    PhaseDist(juce::AudioProcessorValueTreeState& treeState);
    ~PhaseDist() {}

    void processBlock(juce::dsp::AudioBlock<float> &block);
    void prepare(juce::dsp::ProcessSpec& spec);

private:
    SmoothParam amount;
    SmoothParam tone;
    SmoothParam normalise;

    float sampleRate;

    // delay line for phase distortion effect
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;

    // filter to remove harsh freqs from phase distorted signal
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhaseDist)
};
