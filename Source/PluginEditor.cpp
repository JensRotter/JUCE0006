/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JUCE0006AudioProcessorEditor::JUCE0006AudioProcessorEditor (JUCE0006AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);
    startTimer(50);
    addAndMakeVisible(myBarLabel);
}

JUCE0006AudioProcessorEditor::~JUCE0006AudioProcessorEditor()
{
}

//==============================================================================
void JUCE0006AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    
    
}

void JUCE0006AudioProcessorEditor::timerCallback()
{
    int myBarCt = audioProcessor.myMidiTool.midiTransport.trQuarter;
    myBarLabel.setText(std::to_string(myBarCt), juce::dontSendNotification);
    repaint();

}

void JUCE0006AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    myBarLabel.setBounds(10, 10, 200, 100);
}
