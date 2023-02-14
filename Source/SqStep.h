/*
  ==============================================================================

    SqStep.h
    Created: 13 Feb 2023 4:18:58pm
    Author:  lil_jer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SqStep  : public juce::Component,
                       juce::Slider::Listener
{
public:
    SqStep();
    ~SqStep() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    juce::Slider pitchSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SqStep)
};
