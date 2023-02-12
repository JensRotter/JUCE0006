/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
JUCE0006AudioProcessor::JUCE0006AudioProcessor()
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
    
    
}

JUCE0006AudioProcessor::~JUCE0006AudioProcessor()
{
}

//==============================================================================
const juce::String JUCE0006AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JUCE0006AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JUCE0006AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JUCE0006AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JUCE0006AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JUCE0006AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JUCE0006AudioProcessor::getCurrentProgram()
{
    return 0;
}

void JUCE0006AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JUCE0006AudioProcessor::getProgramName (int index)
{
    return {};
}

void JUCE0006AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JUCE0006AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  
    myMidiTool.prepareToPlay(sampleRate,samplesPerBlock);
    myMidiTool.Play();
}

void JUCE0006AudioProcessor::releaseResources()
{
    myMidiTool.Stop();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JUCE0006AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void JUCE0006AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    if (auto bpmFromHost = *getPlayHead()->getPosition()->getBpm())
        myMidiTool.setBpm(bpmFromHost);

    if (auto ppqFromHost = *getPlayHead()->getPosition()->getPpqPosition())
        DBG(ppqFromHost);

    if (getPlayHead()->getPosition()->getIsPlaying())
    {
        myMidiTool.processNextBlock(midiMessages, buffer.getNumSamples());
    }
}

//==============================================================================
bool JUCE0006AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JUCE0006AudioProcessor::createEditor()
{
    return new JUCE0006AudioProcessorEditor (*this);
}

//==============================================================================
void JUCE0006AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JUCE0006AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JUCE0006AudioProcessor();
}
