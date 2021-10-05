/*
  ==============================================================================

    Oscilloscope.h
    Created: 4 Oct 2021 1:31:15am
    Author:  tuiji da

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FIFO.h"
//==============================================================================
/*
*/
class Oscilloscope  : public juce::Component, public juce::Timer
{
public:
    Oscilloscope(SineWaveSynthesizerAudioProcessor&);
    ~Oscilloscope() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;
    
private:
    SineWaveSynthesizerAudioProcessor& processor;
    SingleChannelSampleFifo<juce::AudioBuffer<float>>& waveform;
    juce::AudioBuffer<float> pathBuffer;
    PathProducer<juce::Path> pathProducer;
    juce::Path waveformPath;
    
    void generateWaveform();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscilloscope)
};
