/*
  ==============================================================================

    SqStep.cpp
    Created: 13 Feb 2023 4:18:58pm
    Author:  lil_jer

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SqStep.h"

//==============================================================================
SqStep::SqStep()
{
    pitchSlider.setMinAndMaxValues(1, 12, juce::sendNotificationAsync);
    pitchSlider.setRange(juce::Range<double>(1, 12),1.0);
    addAndMakeVisible(pitchSlider);
    pitchSlider.addListener(this);
}

SqStep::~SqStep()
{
}

void SqStep::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void SqStep::resized()
{
    pitchSlider.setBounds(getLocalBounds());
}

void SqStep::sliderValueChanged(juce::Slider* slider) 
{
    //DBG(slider->getValue());
}
