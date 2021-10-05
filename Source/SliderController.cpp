/*
  ==============================================================================

    SliderController.cpp
    Created: 4 Oct 2021 1:33:08am
    Author:  tuiji da

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SliderController.h"

RotarySliderWithLabel::RotarySliderWithLabel(SineWaveSynthesizerAudioProcessor& audioProcessor,
                                             juce::String titleName):
processor(audioProcessor)
{
    setLookAndFeel(&lnf);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, slider.getTextBoxWidth(), slider.getTextBoxHeight());
    title.setText(titleName, juce::dontSendNotification);
    title.setJustificationType(juce::Justification::centred);
    attachment = std::make_unique<SliderAttachment>(processor.tree, juce::String{titleName}.toLowerCase(), slider);
    addAndMakeVisible(title);
    addAndMakeVisible(slider);
}

RotarySliderWithLabel::~RotarySliderWithLabel()
{
    setLookAndFeel(nullptr);
}

void RotarySliderWithLabel::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(50, 50, 50));
}

void RotarySliderWithLabel::resized()
{
    auto area = getLocalBounds();
    title.setBounds(area.removeFromTop(15));
    slider.setBounds(area.reduced(4));
}



//==============================================================================
SliderController::SliderController(SineWaveSynthesizerAudioProcessor& p):
processor(p),
levelSlider(p, "Level"),
attackSlider(p, "Attack"),
decaySlider(p, "Decay"),
sustainSlider(p, "Sustain"),
releaseSlider(p, "Release")
{
    for(auto& slider : sliders)
    {
        addAndMakeVisible(slider);
    }
}

SliderController::~SliderController()
{
}

void SliderController::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(50, 50, 50));
}

void SliderController::resized()
{
    juce::FlexBox flexBox;
    for(auto& slider : sliders)
    {
        juce::FlexItem item {*slider};
        flexBox.items.add(item.withFlex(1.0));
    }
    flexBox.performLayout(getLocalBounds());
    
    for(auto& slider : sliders)
    {
        slider->setBounds(slider->getBounds().reduced(5));
    }
}
