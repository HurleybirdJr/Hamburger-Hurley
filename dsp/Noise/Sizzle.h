/*
  ==============================================================================

	Sizzle.h
	Created: 13 Jun 2021 6:32:32pm
	Author:  DavZ

  ==============================================================================
*/

#pragma once

 

#include "../SmoothParam.h"
#include "../EnvelopeFollower.h"

//==============================================================================
/*
 */
class Sizzle
{
public:
	Sizzle(juce::AudioProcessorValueTreeState& treeState);
	~Sizzle();

	void processBlock(dsp::AudioBlock<float>& block);
	void prepare(dsp::ProcessSpec& spec);

	void setSampleRate(float sampleRate) { envelopeDetector.setSampleRate(sampleRate);}

	static float whiteNoise()
	{
		return ((float)rand() / (RAND_MAX));
	}
	
	float pinkNoise()
	{
		return 0.f; // idk yet
	}

	float oldSizzleFunction(float inputSample, float nextSizzle)
	{
		return inputSample * (1.0f - whiteNoise() * (nextSizzle * 0.5f)); // nextSizzle;
	}

	float newSizzleFunction(float inputSample, float nextSizzle, float envelope, float nextRand)
	{
		float audioClipped = std::fmin(1.f, std::fmax(0.f, inputSample));
		float amt = powf(audioClipped, 2.0f) - 2.0f * audioClipped + 1.0f;

		return inputSample + (nextRand * ((nextSizzle * envelope * amt) * 0.5f)); // nextSizzle * (nextSizzle / 2)
	}

	float newSizzleV3(float inputSample, float nextSizzle, float envelope, float nextRand)
	{
		float sign = inputSample < 0 ? -1.0f : 1.0f;
		float rectifiedSample = fabs(inputSample);
		float sizzleLevel = nextSizzle * 4.0f * pow(fmax(envelope - rectifiedSample, 0.0f), 1.6f);

		return inputSample + (sizzleLevel * nextRand - (sizzleLevel * 0.5f)) * sign;
	}

private:
	unsigned long xrng = 123456789;
	unsigned long yrng = 362436069;
	unsigned long zrng = 521288629;

	float fastNoise(void)
	{ // period 2^96-1
		unsigned long t;
		xrng ^= xrng << 16;
		xrng ^= xrng >> 5;
		xrng ^= xrng << 1;

		t = xrng;
		xrng = yrng;
		yrng = zrng;
		zrng = t ^ xrng ^ yrng;

		return (float)zrng * 2.3283064365386963e-10 * 2;
	}

	EnvelopeFollower envelopeDetector;
	juce::Random random;
	
	SmoothParam noiseAmount;
	SmoothParam filterTone;
	SmoothParam filterQ;
	
	// filter to filter out sizzle tone
	dsp::IIR::Filter<float> filter;

	double sampleRate;
};
