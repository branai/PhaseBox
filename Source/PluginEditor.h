/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PhaseBoxAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer, public juce::Slider::Listener
{
public:
    PhaseBoxAudioProcessorEditor (PhaseBoxAudioProcessor&);
    ~PhaseBoxAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;

    // Something like this: https://forum.juce.com/t/get-project-root-path/37042/2
    juce::File PhaseBoxAudioProcessorEditor::getProjectDir()
    {

        auto start = juce::File::getSpecialLocation(juce::File::currentExecutableFile);
        while (start.exists() && !start.isRoot() && start.getFileName() != "Builds")
            start = start.getParentDirectory();

        if (start.getFileName() == "Builds")
        {
            auto resources = start.getSiblingFile("Resources");
            if (resources.isDirectory())
                return resources;

        }
    }
    void resized() override;
    juce::Point<float> mapToCoord(float x, float y);
    void timerCallback() override {
        repaint();
    }

    void sliderValueChanged(juce::Slider* slider) override
    {

        if (slider == &boxSizeSlider)
            audioProcessor.threshPercent = boxSizeSlider.getValue();// / 100.0;

    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PhaseBoxAudioProcessor& audioProcessor;

    juce::Rectangle<float> squareBounds;
    juce::Point<float> lineStart;
    float lineLength;

    juce::Slider boxSizeSlider;

    juce::File labelImageFile = getProjectDir().getChildFile("KnobLabel.png");
    juce::File titleImageFile = getProjectDir().getChildFile("Title.png");
    juce::Image labelImage = juce::ImageFileFormat::loadFrom(labelImageFile);
    juce::Image titleImage = juce::ImageFileFormat::loadFrom(titleImageFile);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaseBoxAudioProcessorEditor)
};
