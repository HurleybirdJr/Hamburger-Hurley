#include "Compressor.h"
#include "Curves.h"
 

void Compressor::prepare(juce::dsp::ProcessSpec& spec)
{
    envelope.prepare(spec);
}

void Compressor::processBlock(juce::dsp::AudioBlock<float>& dryBuffer)
{
    #ifdef DEBUG
    TRACE_DSP();
    #endif

    float makeupGain = juce::Decibels::decibelsToGain(makeup_dB);

    for (size_t sample = 0; sample < dryBuffer.getNumSamples(); sample++)
    {
        float leftSample = dryBuffer.getSample(0, sample);
        float rightSample = dryBuffer.getSample(1, sample);

        // #ifdef DEBUG
        // TRACE_EVENT_BEGIN("dsp", "envelope.processSampleStereo");
        // #endif

        float envelopeVal = envelope.processSampleStereo(leftSample, rightSample);

        // #ifdef DEBUG
        // TRACE_EVENT_END("dsp");
        // #endif

        float gain = 0.0f;

        // #ifdef DEBUG
        // TRACE_EVENT_BEGIN("dsp", "compute different gain types");
        // #endif

        switch (type)
        {
        case EXPANDER:
            gain = Curves::computeExpanderGain(envelopeVal, ratioLower, kneeWidth);
            break;
        case COMPRESSOR:
            gain = Curves::computeCompressorGain(envelopeVal, thresholdLower, ratioLower, kneeWidth);
            break;
        case UPWARDS_DOWNWARDS:
            gain = Curves::computeUpwardsDownwardsGain(envelopeVal, thresholdUpper, thresholdLower, ratioUpper, ratioLower, kneeWidth);
            break;
        }

        // #ifdef DEBUG
        // TRACE_EVENT_END("dsp");
        // #endif

        // #ifdef DEBUG
        // TRACE_EVENT_BEGIN("dsp", "apply gain");
        // #endif

        dryBuffer.setSample(0, sample, leftSample * gain);
        dryBuffer.setSample(1, sample, rightSample * gain);

        // #ifdef DEBUG
        // TRACE_EVENT_END("dsp");
        // #endif
    }

    dryBuffer.multiplyBy(makeupGain);
}

float Compressor::processOneSampleGainStereo(float sampleL, float sampleR)
{
    float makeupGain = juce::Decibels::decibelsToGain(makeup_dB);
    float leftSample = sampleL;
    float rightSample = sampleR;

    // #ifdef DEBUG
    // TRACE_EVENT_BEGIN("dsp", "envelope.processSampleStereo");
    // #endif

    float envelopeVal = envelope.processSampleStereo(leftSample, rightSample);

    // #ifdef DEBUG
    // TRACE_EVENT_END("dsp");
    // #endif

    float gain = 0.0f;

    // #ifdef DEBUG
    // TRACE_EVENT_BEGIN("dsp", "compute different gain types");
    // #endif

    switch (type)
    {
    case EXPANDER:
        gain = Curves::computeExpanderGain(envelopeVal, ratioLower, kneeWidth);
        break;
    case COMPRESSOR:
        gain = Curves::computeCompressorGain(envelopeVal, thresholdLower, ratioLower, kneeWidth);
        break;
    case UPWARDS_DOWNWARDS:
        gain = Curves::computeUpwardsDownwardsGain(envelopeVal, thresholdUpper, thresholdLower, ratioUpper, ratioLower, kneeWidth);
        break;
    }
    
    return gain * makeupGain;
}

float Compressor::processOneSampleGainMono(float sample)
{
    float makeupGain = juce::Decibels::decibelsToGain(makeup_dB);
    float envelopeVal = envelope.processSample(sample);
    float gain = 0.0f;

    switch (type)
    {
    case EXPANDER:
        gain = Curves::computeExpanderGain(envelopeVal, ratioLower, kneeWidth);
        break;
    case COMPRESSOR:
        gain = Curves::computeCompressorGain(envelopeVal, thresholdLower, ratioLower, kneeWidth);
        break;
    case UPWARDS_DOWNWARDS:
        gain = Curves::computeUpwardsDownwardsGain(envelopeVal, thresholdUpper, thresholdLower, ratioUpper, ratioLower, kneeWidth);
        break;
    }
    
    return gain * makeupGain;
}

void Compressor::updateParameters(float atk, float rel, float mkp, float rat, float thres, float knee, float mkpDB)
{
    // #ifdef DEBUG
    // TRACE_EVENT_BEGIN("dsp", "Compressor::updateParameters");
    // #endif

    envelope.setAttackTime(atk);
    envelope.setReleaseTime(rel);
    this->attack = atk;
    this->release = rel;
    this->makeup = mkp;
    this->ratioLower = rat;
    this->ratioUpper = rat;
    this->thresholdLower = thres;
    this->thresholdUpper = thres + 12.0f + knee * 2.0f;
    this->kneeWidth = knee;
    this->makeup_dB = mkp;

    // #ifdef DEBUG
    // TRACE_EVENT_END("dsp");
    // #endif
}

void Compressor::updateUpDown(float atk, float rel, float mkp, float ratioLow, float ratioUp, float thresholdLow, float thresholdUp, float kneeW, float mkpDB)
{
    // #ifdef DEBUG
    // TRACE_EVENT_BEGIN("dsp", "Compressor::updateParameters");
    // #endif

    envelope.setAttackTime(atk);
    envelope.setReleaseTime(rel);
    this->attack = atk;
    this->release = rel;
    this->makeup = mkp;
    this->ratioLower = ratioLow;
    this->ratioUpper = ratioUp;
    this->thresholdLower = thresholdLow;
    this->thresholdUpper = thresholdUp;
    this->kneeWidth = kneeW;
    this->makeup_dB = mkp;

    // #ifdef DEBUG
    // TRACE_EVENT_END("dsp");
    // #endif
}