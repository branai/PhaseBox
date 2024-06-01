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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaseBoxAudioProcessorEditor)
};
