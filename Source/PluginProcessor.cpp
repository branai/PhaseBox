/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PhaseBoxAudioProcessor::PhaseBoxAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    for (juce::Point<float>& curr : coords) // Assuming left channel
    {
        curr = juce::Point<float>(0.5f, 0.5f);
    }
}

PhaseBoxAudioProcessor::~PhaseBoxAudioProcessor()
{
}

//==============================================================================
const juce::String PhaseBoxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PhaseBoxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PhaseBoxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PhaseBoxAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PhaseBoxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PhaseBoxAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PhaseBoxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PhaseBoxAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PhaseBoxAudioProcessor::getProgramName (int index)
{
    return {};
}

void PhaseBoxAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PhaseBoxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PhaseBoxAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PhaseBoxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PhaseBoxAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    float xFloat = 0;
    float yFloat = 0;

    auto* channelDataL = buffer.getReadPointer(0);
    auto* outputChannelDataL = buffer.getWritePointer(0);

    auto* channelDataR = buffer.getReadPointer(1);
    auto* outputChannelDataR = buffer.getWritePointer(1);

    bool hit = false;


    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float inputSampleL = channelDataL[sample];
        float inputSampleR = channelDataR[sample];




        // Apply threshold clipping
        if (std::abs(inputSampleL) > threshPercent || std::abs(inputSampleR) > threshPercent) {
            hit = true;
        }



        xFloat = inputSampleL;// * outputChannelDataL[sample] / std::abs(outputChannelDataL[sample]);
        yFloat = inputSampleR;


        outputChannelDataL[sample] = softClip(inputSampleL, threshPercent);
        outputChannelDataR[sample] = softClip(inputSampleR, threshPercent);




        if (sample % 16 == 0) { //arbitrary num, biggger means less connected
            coords.push_back(juce::Point<float>(xFloat, yFloat));
        }

    }
}


double PhaseBoxAudioProcessor::softClip(float inputValue, float
    thresh)
{
    inputValue = inputValue / thresh;

    float smooth = 0.9; //smoothness (start of turn)
    float transform = std::max(-smooth, std::min(smooth, inputValue));//smooth - (1 - thresh);
    // Apply soft clipping
    return thresh * ((
        (1 - smooth) * std::tanh((inputValue - transform) / (1 - smooth))
        ) + transform);
}


//==============================================================================
bool PhaseBoxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PhaseBoxAudioProcessor::createEditor()
{
    return new PhaseBoxAudioProcessorEditor (*this);
}

//==============================================================================
void PhaseBoxAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PhaseBoxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PhaseBoxAudioProcessor();
}

