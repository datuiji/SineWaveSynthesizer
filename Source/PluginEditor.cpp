/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SineWaveSynthesizerAudioProcessorEditor::SineWaveSynthesizerAudioProcessorEditor (SineWaveSynthesizerAudioProcessor& p):
AudioProcessorEditor (&p),
audioProcessor (p),
waveform(p),
spectrum(p),
controller(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 800);
    for (auto& comp : subComponents)
        addAndMakeVisible(comp);
}

SineWaveSynthesizerAudioProcessorEditor::~SineWaveSynthesizerAudioProcessorEditor()
{
}

//==============================================================================
void SineWaveSynthesizerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(10,10,10));    
}

void SineWaveSynthesizerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    juce::FlexBox upFlexBox;
    upFlexBox.flexDirection = juce::FlexBox::Direction::row;
    upFlexBox.items.add(juce::FlexItem(waveform).withFlex(1.0f));
    upFlexBox.items.add(juce::FlexItem(spectrum).withFlex(1.0f));
    
    juce::FlexBox bottomFlexBox;
    bottomFlexBox.flexDirection = juce::FlexBox::Direction::row;
    bottomFlexBox.items.add(juce::FlexItem(controller).withFlex(1.0f));
    
    juce::FlexBox flexBox;
    flexBox.flexDirection = juce::FlexBox::Direction::column;
    flexBox.items.add(juce::FlexItem(upFlexBox).withFlex(3.0f));
    flexBox.items.add(juce::FlexItem(bottomFlexBox).withFlex(1.0f));
    flexBox.performLayout(area.reduced(10));
    
    for (auto& comp : subComponents)
    {
        comp->setBounds(comp->getBounds().reduced(3));
    }
}
