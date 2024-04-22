#pragma once

#include "Cooked.h"

//==============================================================================
Cooked::Cooked(juce::AudioProcessorValueTreeState& treeState) : amount(treeState, ParamIDs::fold) {}

Cooked::~Cooked() {}

void Cooked::prepare(juce::dsp::ProcessSpec& spec) noexcept {
	amount.prepare(spec);
}

void Cooked::processBlock(juce::dsp::AudioBlock<float>& block) noexcept {
	#ifdef DEBUG
	TRACE_EVENT("dsp", "Cooked::processBlock");
	#endif

	amount.update();

	if (amount.getRaw() == 0.f) {
		return;
	}

	for (int sample = 0; sample < block.getNumSamples(); sample++) {
		float nextCooked = amount.getNextValue() * 0.01f;

		for (int channel = 0; channel < block.getNumChannels(); channel++) {
			auto dryData = block.getChannelPointer(channel);

			if (nextCooked != 0.f) {
				auto x = dryData[sample] * (nextCooked * 20.0f + 1.0f);
				dryData[sample] = (4.0f * (abs(0.25*x + 0.25 - round(0.25*x + 0.25)) - 0.25));
			}
		}
	}
};