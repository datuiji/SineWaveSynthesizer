/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 5 Oct 2021 4:37:51am
    Author:  tuiji da

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
        auto fill    = juce::Colours::orange; //slider.findColour (juce::Slider::rotarySliderFillColourId);

        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10);

        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = juce::jmin (8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;

        juce::Path backgroundArc;
        backgroundArc.addCentredArc (bounds.getCentreX(),
                                     bounds.getCentreY(),
                                     arcRadius,
                                     arcRadius,
                                     0.0f,
                                     rotaryStartAngle,
                                     rotaryEndAngle,
                                     true);

        g.setColour (outline);
        g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::square));
        
        juce::Point<float> thumbPoint (bounds.getCentreX() + (arcRadius - 10.0f) * std::cos (toAngle - juce::MathConstants<float>::halfPi),
                                 bounds.getCentreY() + (arcRadius - 10.0f) * std::sin (toAngle - juce::MathConstants<float>::halfPi));
        
        g.setColour (fill);
        g.drawLine (backgroundArc.getBounds().getCentreX(), backgroundArc.getBounds().getCentreY(), thumbPoint.getX(), thumbPoint.getY(), lineW);

        if (slider.isEnabled())
        {
            juce::Path valueArc;
            valueArc.addCentredArc (bounds.getCentreX(),
                                    bounds.getCentreY(),
                                    arcRadius,
                                    arcRadius,
                                    0.0f,
                                    rotaryStartAngle,
                                    toAngle,
                                    true);

            g.setColour (fill);
            g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::square));
        }
    }
};
