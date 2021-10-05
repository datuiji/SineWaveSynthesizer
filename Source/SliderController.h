/*
  ==============================================================================

    SliderController.h
    Created: 4 Oct 2021 1:33:08am
    Author:  tuiji da

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyLookAndFeel.h"
//==============================================================================
/*
*/
class RotarySliderWithLabel : public juce::Component
{
public:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    RotarySliderWithLabel(SineWaveSynthesizerAudioProcessor& audioProcessor, juce::String titleName);
    ~RotarySliderWithLabel();
    
    void paint (juce::Graphics&) override;
    void resized() override;

    
private:
    SineWaveSynthesizerAudioProcessor& processor;
    juce::Label title;
    juce::Slider slider;
    std::unique_ptr<SliderAttachment> attachment;
    
    MyLookAndFeel lnf;
};


class SliderController  : public juce::Component
{
public:
    SliderController(SineWaveSynthesizerAudioProcessor&);
    ~SliderController() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    SineWaveSynthesizerAudioProcessor& processor;
    RotarySliderWithLabel levelSlider;
    RotarySliderWithLabel attackSlider;
    RotarySliderWithLabel decaySlider;
    RotarySliderWithLabel sustainSlider;
    RotarySliderWithLabel releaseSlider;
    
    std::vector<RotarySliderWithLabel*> sliders {&levelSlider, &attackSlider, &decaySlider, &sustainSlider, &releaseSlider};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderController)
};
