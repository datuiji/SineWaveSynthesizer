/*
  ==============================================================================

    FrequencySpectrum.h
    Created: 4 Oct 2021 1:32:23am
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
class FrequencySpectrum  : public juce::Component, public juce::Timer
{
public:
    FrequencySpectrum(SineWaveSynthesizerAudioProcessor&);
    ~FrequencySpectrum() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:
    SineWaveSynthesizerAudioProcessor& processor;
    SingleChannelSampleFifo<juce::AudioBuffer<float>>& spectrum;
    juce::AudioBuffer<float> pathBuffer;
    FFTDataGenerator<std::vector<float>> fftDataGenerator;
    FFTPathProducer<juce::Path> pathProducer;
    juce::Path spectrumPath;
    
    void generateSpectrum();
    
    void drawBackgroundGrid(juce::Graphics& g);
    void drawTextLabels(juce::Graphics& g);
    
    std::vector<float> getFrequencies()
    {
        return std::vector<float>
        {
            20, /*30, 40,*/ 50, 100,
            200, /*300, 400,*/ 500, 1000,
            2000, /*3000, 4000,*/ 5000, 10000,
            20000
        };
    };
    std::vector<float> getGains()
    {
        return std::vector<float>
        {
//            -24, -12, 0, 12, 24
            0, -12, -24, -36, -48
        };
    }
    std::vector<float> getXs(const std::vector<float>& freqs, float left, float width)
    {
        std::vector<float> xs;
        for( auto f : freqs )
        {
            auto normX = juce::mapFromLog10(f, 20.f, 20000.f);
            xs.push_back( left + width * normX );
        }
        
        return xs;
    }
    
    juce::Rectangle<int> getRenderArea()
    {
        auto bounds = getLocalBounds();
        
        bounds.removeFromTop(12);
        bounds.removeFromBottom(2);
        bounds.removeFromLeft(20);
        bounds.removeFromRight(20);
        
        return bounds;
    }


    juce::Rectangle<int> getAnalysisArea()
    {
        auto bounds = getRenderArea();
        bounds.removeFromTop(4);
        bounds.removeFromBottom(4);
        return bounds;
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencySpectrum)
};
