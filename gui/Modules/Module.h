#pragma once
 
#include "../../PluginProcessor.h"
#include "LightButton.h"
#include "../LookAndFeel/ComboBoxLookAndFeel.h"
#include "../Knob.h"
#include "Panel.h"

class Module : public juce::Component
{
public:
    Module(AudioPluginAudioProcessor &processor, const std::string &moduleName, const std::string buttonAttachmentId, std::string categoryAttachmentId, std::vector<std::unique_ptr<Panel>> panels)
        : modulePanels(std::move(panels)),
          powerOffImage(juce::ImageCache::getFromMemory(BinaryData::poweroff_png, BinaryData::poweroff_pngSize)), // yes it's using imageCache but stop constructing new instances! unify somehow
          powerOnImage(juce::ImageCache::getFromMemory(BinaryData::poweron_png, BinaryData::poweron_pngSize))
    {
        setupHeader();
        setupPanels(moduleName);
        setupTitleLabel(moduleName);
        setupCategorySelector(moduleName);

        setPaintingIsUnclipped(true);


        if (categoryAttachmentId != "")
        {
            categoryAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, categoryAttachmentId, categorySelector);
            setScreen(categorySelector.getSelectedItemIndex());
        }
        else
        {
            setScreen(0);
        }
        // categorySelector.setSelectedItemIndex(0);

        if (buttonAttachmentId != "")
        {
            enabledButton = std::make_unique<LightButton>(processor, powerOffImage, powerOnImage);
            attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.treeState, buttonAttachmentId, *enabledButton);
            addAndMakeVisible(enabledButton.get());
        }
        else
        {
            enabledButton = nullptr;
        }

        setCategoryText(moduleName);
    }

    ~Module() override = default;

    void paint(juce::Graphics &g) override
    {
        // g.fillAll(juce::Colour::fromRGB(0, 0, 0));
        Path p;
        p.addRoundedRectangle(getLocalBounds().reduced(4).toFloat(), 15.0f);
        g.setColour(juce::Colour::fromRGB(0, 0, 0));
        g.fillPath(p);
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced(8);
        auto titleBounds = bounds.removeFromTop(30);
        titleBounds.reduce(10, 0);

        auto extraSpace = 0.0f;

        header.items.clear();
        if (enabledButton != nullptr) {
            header.items.add(FlexItem(*enabledButton).withMinWidth(15.0f).withMargin(7.5f));
            extraSpace = 15.0f + 7.5f;
        }
        
        auto titleFont = titleLabel.getFont();

        if (modulePanels.size() == 1)
        {
            header.items.add(FlexItem(titleLabel).withMinWidth( titleFont.getStringWidth(titleLabel.getText()) * 1.1f));
        }
        else
        {
            header.items.add(FlexItem(categorySelector).withMinWidth( titleFont.getStringWidth(categorySelector.getText()) * 1.4f + extraSpace));
        }

        header.performLayout(titleBounds);

        for (auto &panel : modulePanels)
        {
            panel->setBounds(bounds);
        }
    }

    std::unique_ptr<LightButton> enabledButton = nullptr;
    juce::ComboBox categorySelector;

private:
    void setupHeader()
    {
        header.flexDirection = juce::FlexBox::Direction::row;
        header.justifyContent = juce::FlexBox::JustifyContent::center;
    }

    void setupPanels(const std::string &moduleName)
    {
        int i = 1;
        for (auto &panel : modulePanels)
        {
            categorySelector.addItem(panel->getName() + " " + moduleName, i++);
            addAndMakeVisible(panel.get());
            panel->setVisible(false);
        }
    }

    void setupTitleLabel(const std::string &moduleName)
    {
        titleLabel.setText(moduleName, juce::dontSendNotification);
        titleLabel.setFont(comboBoxLook.getComboBoxFont());
        titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        if (modulePanels.size() == 1)
        {
            addAndMakeVisible(titleLabel);
        }
    }

    void setCategoryText(juce::String moduleName)
    {
        if (categoryAttachment == nullptr) {
            categorySelector.setText(moduleName);
        } else {
            size_t index = categorySelector.getSelectedItemIndex();
            if (index != -1)
            {
                auto panelName = modulePanels[index]->getName();
                categorySelector.setText(panelName + " " + moduleName);
            }
        }
    }

    void setupCategorySelector(const std::string &moduleName)
    {
        categorySelector.setLookAndFeel(&comboBoxLook);
        categorySelector.onChange = [this, moduleName]
        {
            auto selection = categorySelector.getSelectedItemIndex();
            if (selection != -1)
            {
                setScreen(selection);
                this->resized();
            }
        };

        if (modulePanels.size() > 1)
        {
            setCategoryText(moduleName);
            addAndMakeVisible(categorySelector);
        }
    }

    void setScreen(int index)
    {
        if (index >= 0 && index < modulePanels.size())
        {
            for (auto &panel : modulePanels)
            {
                panel->setVisible(false);
            }
            modulePanels[index]->setVisible(true);
        }
    }

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> categoryAttachment = nullptr;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment = nullptr;
    // array of pointers of panel
    std::vector<std::unique_ptr<Panel>> modulePanels;

    juce::FlexBox header;
    ComboBoxLookAndFeel comboBoxLook;
    juce::Label titleLabel;

    juce::Image powerOffImage;
    juce::Image powerOnImage;
};