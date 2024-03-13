#pragma once
 
#include "Modules/Module.h"

#include "Modules/Panels/EQPanel.h"
#include "Modules/Panels/UtilityPanel.h"
#include "Modules/Panels/SettingsPanel.h"
#include "LookAndFeel/Palette.h"

class UtilColumn : public juce::Component
{
public:
    UtilColumn(AudioPluginAudioProcessor &p)
    {
        // panel with elements already inside
        std::vector<std::unique_ptr<Panel>> eqPanels;
        // ORDERING IS VERY IMPORTANT
        eqPanels.push_back(std::make_unique<EQPanel>(p));
        eq = std::make_unique<Module>(p, "EMPHASIS", "emphasisOn", "", std::move(eqPanels));
        eq->setLookAndFeel(&eqLookAndFeel);
        addAndMakeVisible(eq.get());

        std::vector<std::unique_ptr<Panel>> utilityPanels;
        // ORDERING IS VERY IMPORTANT
        utilityPanels.push_back(std::make_unique<UtilityPanel>(p));
        utility = std::make_unique<Module>(p, "UTILITY", "hamburgerEnabled", "", std::move(utilityPanels));
        utility->setLookAndFeel(&utilityLookAndFeel);
        addAndMakeVisible(utility.get());

        std::vector<std::unique_ptr<Panel>> settingsPanels;
        settingsPanels.push_back(std::make_unique<SettingsPanel>(p));
        settings = std::make_unique<Module>(p, "SETTINGS", "", "", std::move(settingsPanels), true);
        addAndMakeVisible(settings.get());
    }

    void resized() override{
        auto bounds = getLocalBounds();
        auto height = bounds.getHeight();

        eq->setBounds(bounds.removeFromTop(height * 0.5));
        utility->setBounds(bounds.removeFromBottom(height * 0.25));
        settings->setBounds(bounds);
    }

private:
    KnobLAF eqLookAndFeel = KnobLAF(Palette::colours[2]);
    KnobLAF utilityLookAndFeel = KnobLAF(juce::Colours::whitesmoke);

    std::unique_ptr<Module> eq = nullptr;
    std::unique_ptr<Module> utility = nullptr;
    std::unique_ptr<Module> settings = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UtilColumn)
};