/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Oscilloscope.h"
#include "FrequencySpectrum.h"
#include "SliderController.h"

//==============================================================================
/**
*/
class SineWaveSynthesizerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SineWaveSynthesizerAudioProcessorEditor (SineWaveSynthesizerAudioProcessor&);
    ~SineWaveSynthesizerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    SineWaveSynthesizerAudioProcessor& audioProcessor;
    Oscilloscope waveform;
    FrequencySpectrum spectrum;
    SliderController controller;
    
    std::vector<juce::Component*> subComponents {&waveform, &spectrum, &controller};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineWaveSynthesizerAudioProcessorEditor)
};
