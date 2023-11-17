#pragma once

#include <JuceHeader.h>
#include "../Panel.h"
#include "../../Knob.h"
#include "PanelNames.h"

class ClassicSatPanel : public Panel
{
public:
    ClassicSatPanel(AudioPluginAudioProcessor &p) : Panel(p, "GRILL"), 
        satKnob(p, "SATURATION", "saturationAmount", 0.0f, 100.0f),
        biasKnob(p, "BIAS", "bias", 0.0f, 100.0f),
        fuzzKnob(p, "FUZZ", "fuzz", 0.0f, 100.0f),
        grungeAmountKnob(p, "GRUNGE", "grungeAmt", 0.0f, 1.0f),
        grungeToneKnob(p, "TONE", "grungeTone", 0.0f, 1.0f)
        {
        
        addAndMakeVisible(satKnob);
        addAndMakeVisible(biasKnob);
        addAndMakeVisible(fuzzKnob);
        addAndMakeVisible(grungeToneKnob);
        addAndMakeVisible(grungeAmountKnob);

        auto laf = new KnobLAF(juce::Colours::orangered);
        setLookAndFeel(laf); // will cascade to all children knobs
    }

    void paint(juce::Graphics &g) override
    {
        // g.fillAll(juce::Colours::green);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        satKnob.setBounds(bounds.removeFromTop(bounds.getHeight() / 1.5f).reduced(10));

        auto width = bounds.getWidth() / 4;
        grungeAmountKnob.setBounds(bounds.removeFromLeft(width));
        grungeToneKnob.setBounds(bounds.removeFromLeft(width));
        biasKnob.setBounds(bounds.removeFromLeft(width));
        fuzzKnob.setBounds(bounds);
    }

    ParamKnob satKnob;
    ParamKnob biasKnob;
    ParamKnob fuzzKnob;
    ParamKnob grungeAmountKnob;
    ParamKnob grungeToneKnob;
};