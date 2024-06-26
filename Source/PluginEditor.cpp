/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PhaseBoxAudioProcessorEditor::PhaseBoxAudioProcessorEditor (PhaseBoxAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(500, 500);
    startTimer(1.0 / 44100.0);


    addAndMakeVisible(boxSizeSlider);
    boxSizeSlider.setRange(0.01, 1.0);
    boxSizeSlider.addListener(this);
    boxSizeSlider.setValue(1.0);

    boxSizeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Set the slider style to vertical
    boxSizeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);


    // WHY DO THESE LINES CAUSE ERRORS
    /*boxSizeSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::black);
    boxSizeSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::black);
    boxSizeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::black);
    boxSizeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
    boxSizeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);*/


    // Initialize the square's position and size
    squareBounds = juce::Rectangle<float>(100.0f, 100.0f, 50.0f, 50.0f);

    audioProcessor.coords = std::deque<juce::Point<float>>();
    audioProcessor.movingPoints = std::deque<PointData>();
}

PhaseBoxAudioProcessorEditor::~PhaseBoxAudioProcessorEditor()
{
}

//==============================================================================
void PhaseBoxAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    float screenWidth = static_cast<float>(getWidth());
    float screenHeight = static_cast<float>(getHeight());
    g.fillAll(juce::Colours::white);


    g.setColour(juce::Colours::grey); //::white
    g.setFont(0.5f);

    int sideLen = 150 * audioProcessor.threshPercent;

    float squareX = (250 - sideLen);
    float squareY = (250 - sideLen);
    g.setColour(juce::Colours::grey);
    g.drawRect(juce::Rectangle<float>(squareX, squareY, sideLen * 2, sideLen * 2), 3.0f);



    

    while (audioProcessor.coords.size() > 100)
        audioProcessor.coords.pop_front();


    std::deque<juce::Point<float>> coordsCopy(audioProcessor.coords);

    for (const juce::Point<float>& curr : coordsCopy)
    {

        juce::Point<float> nc = mapToCoord(curr.x, curr.y);
        float dotSize = 5.0f;

        if (std::abs(curr.x) < audioProcessor.threshPercent && std::abs(curr.y) < audioProcessor.threshPercent) {
            g.setColour(juce::Colours::black);
            g.fillRect(nc.x - dotSize / 2.0f, nc.y - dotSize / 2.0f, dotSize, dotSize);
        }


    }

    
    g.setFont(juce::Font(20.0f, juce::Font::bold));
    //juce::Rectangle<int> textArea(200, 25, 100, 50);

    // Draw the text centered within the defined rectangle
    //g.drawFittedText("PhaseBox", textArea, juce::Justification::centred, 1);
    g.drawImage(titleImage, 150, 10, titleImage.getWidth(), titleImage.getHeight(), 0, 0, titleImage.getWidth(), titleImage.getHeight());
    g.drawImage(labelImage, 0, 400, labelImage.getWidth(), labelImage.getHeight(), 0, 0, labelImage.getWidth(), labelImage.getHeight());



}




juce::Point<float> PhaseBoxAudioProcessorEditor::mapToCoord(float x, float y) {
    int sideLen = 150 * audioProcessor.threshPercent;
    float mappedX = (x * sideLen / audioProcessor.threshPercent) + 250;
    float mappedY = (y * sideLen / audioProcessor.threshPercent) + 250;
    return juce::Point<float>(mappedX, mappedY);
}

void PhaseBoxAudioProcessorEditor::resized()
{
    // Update the square's position when the editor is resized
    squareBounds.setX(250);
    squareBounds.setY(250);
    boxSizeSlider.setBounds (200,412 ,100,75);
    //boxSizeSlider.setBounds(450, 100, 20, 300);
}
