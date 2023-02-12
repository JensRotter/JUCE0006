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
class JUCE0006AudioProcessorEditor : public juce::AudioProcessorEditor , private juce::Timer
{
public:
    JUCE0006AudioProcessorEditor (JUCE0006AudioProcessor&);
    ~JUCE0006AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void timerCallback();
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JUCE0006AudioProcessor& audioProcessor;
    juce::Label myBarLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JUCE0006AudioProcessorEditor)
};
