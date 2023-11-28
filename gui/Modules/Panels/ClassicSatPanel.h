#pragma once

 
#include "../Panel.h"
#include "../../Knob.h"
#include "PanelNames.h"

class ClassicSatPanel : public Panel
{
public:
    ClassicSatPanel(AudioPluginAudioProcessor &p) : Panel(p, "GRILL"), 
        satKnob(p, "SATURATION", "saturationAmount", ParamUnits::percent),
        biasKnob(p, "BIAS", "bias"),
        fuzzKnob(p, "FUZZ", "fuzz")
    {
        addAndMakeVisible(satKnob);
        addAndMakeVisible(biasKnob);
        addAndMakeVisible(fuzzKnob);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        satKnob.setBounds(bounds.removeFromTop(bounds.getHeight() / 1.5f).reduced(10));

        auto width = bounds.getWidth() / 2;
        biasKnob.setBounds(bounds.removeFromLeft(width));
        fuzzKnob.setBounds(bounds);
    }

private:
    ParamKnob satKnob;
    ParamKnob biasKnob;
    ParamKnob fuzzKnob;
};