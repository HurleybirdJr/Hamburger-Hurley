#include "Erosion.h"

#include <JuceHeader.h>

void Erosion::processBlock(dsp::AudioBlock<float>& block, double sampleRate)
{
    TRACE_EVENT("dsp", "Erosion::processBlock");

    erosionAmount.update();
    erosionFrequency.update();
    erosionQ.update();

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    for (int i = 0; i < block.getNumSamples(); i++) {
        delayLine.pushSample(0, leftBlock.getSample(0, i));
        delayLine.pushSample(1, rightBlock.getSample(0, i));
    }

    // get the current value of the saturation knob to figure out how much to multiply a random value by
    float saturationAmount = erosionAmount.getRaw() * 0.1f;

    // create buffer to store the random values
    randomBuffer.clear();
    for (int i = 0; i < block.getNumSamples(); i++) {
        // store the filtered random value in the buffer

        float randomValue = random.nextFloat() * 2.0f - 1.0f;
        randomBuffer.setSample(0, i, randomValue);

        randomValue = random.nextFloat() * 2.0f - 1.0f;
        randomBuffer.setSample(1, i, randomValue);
    }

    *iirFilter.state = *dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, erosionFrequency.getRaw(), erosionQ.getRaw());

    // now, filter the random values
    iirFilter.process(dsp::ProcessContextReplacing<float>(randomBlock));

    // now with the filtered values, multiply them by the saturation amount

    for (int i = 0; i < block.getNumSamples(); i++) {
        float leftEroded = delayLine.popSample(0, (randomBuffer.getSample(0, i) + 1.f) * saturationAmount);
        float rightEroded = delayLine.popSample(1, (randomBuffer.getSample(1, i) + 1.f) * saturationAmount);

        block.setSample(0, i, leftEroded);
        block.setSample(1, i, rightEroded);
    }
}

void Erosion::prepare(dsp::ProcessSpec& spec)
{
    // init delay
    erosionAmount.prepare(spec);
    erosionFrequency.prepare(spec);
    erosionQ.prepare(spec);

    delayLine.prepare(spec);

    delayLine.setMaximumDelayInSamples(0.1f * spec.sampleRate);

    // init iir filter
    *iirFilter.state = *dsp::IIR::Coefficients<float>::makeBandPass(spec.sampleRate, 880.0f, 3.0f);

    iirFilter.prepare(spec);

    // init random
    random.setSeedRandomly();

    // init random buffer
    randomBuffer.setSize(2, spec.maximumBlockSize);
    randomBuffer.clear();

    randomBlock = dsp::AudioBlock<float>(randomBuffer);

    oldSampleRate = spec.sampleRate;
    oldSamplesPerBlock = spec.maximumBlockSize;
}