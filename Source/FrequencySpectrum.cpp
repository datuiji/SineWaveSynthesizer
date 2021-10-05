/*
  ==============================================================================

    FrequencySpectrum.cpp
    Created: 4 Oct 2021 1:32:23am
    Author:  tuiji da

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FrequencySpectrum.h"

//==============================================================================
FrequencySpectrum::FrequencySpectrum(SineWaveSynthesizerAudioProcessor& p):
processor(p),
spectrum(p.getSingleChannelSampleFifo())
{
    fftDataGenerator.changeOrder(10);
    pathBuffer.setSize(1, fftDataGenerator.getFFTSize());
    startTimerHz(30);
}

FrequencySpectrum::~FrequencySpectrum()
{
}

void FrequencySpectrum::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(50, 50, 50));
    g.setColour(juce::Colours::grey);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 3, 3);
    
    drawBackgroundGrid(g);
    
    g.setColour(juce::Colours::white);
    g.strokePath(spectrumPath, juce::PathStrokeType(1.f));
    
    drawTextLabels(g);
}

void FrequencySpectrum::resized()
{
    
}

void FrequencySpectrum::generateSpectrum()
{
    juce::AudioBuffer<float> tempIncomingBuffer;
    while(spectrum.getNumCompleteBuffersAvailable() > 0)
    {
        if(spectrum.getAudioBuffer(tempIncomingBuffer))
        {
            auto size = tempIncomingBuffer.getNumSamples();

            juce::FloatVectorOperations::copy(pathBuffer.getWritePointer(0, 0),
                                              pathBuffer.getReadPointer(0, size),
                                              pathBuffer.getNumSamples() - size);

            juce::FloatVectorOperations::copy(pathBuffer.getWritePointer(0, pathBuffer.getNumSamples() - size),
                                              tempIncomingBuffer.getReadPointer(0, 0),
                                              size);
            
            fftDataGenerator.produceFFTDataForRendering(pathBuffer, -48.f);
        }
    }
    
    const auto fftSize = fftDataGenerator.getFFTSize();
    const auto binWidth = processor.getSampleRate() / double(fftSize);

    while( fftDataGenerator.getNumAvailableFFTDataBlocks() > 0 )
    {
        std::vector<float> fftData;
        if(fftDataGenerator.getFFTData( fftData) )
        {
            pathProducer.generatePath(fftData, getLocalBounds().toFloat(), fftSize, binWidth, -48.f);
        }
    }
    
    while(pathProducer.getNumPathsAvailable() > 0)
    {
        pathProducer.getPath(spectrumPath);
    }

}

void FrequencySpectrum::timerCallback()
{
    generateSpectrum();
    repaint();
}

void FrequencySpectrum::drawBackgroundGrid(juce::Graphics& g)
{
    using namespace juce;
    auto freqs = getFrequencies();
    
    auto renderArea = getLocalBounds();
    auto left = renderArea.getX();
    auto right = renderArea.getRight();
    auto top = renderArea.getY();
    auto bottom = renderArea.getBottom();
    auto width = renderArea.getWidth();
    
    auto xs = getXs(freqs, left, width);
    
    g.setColour(Colours::dimgrey);
    for( auto x : xs )
    {
        g.drawVerticalLine(x, top, bottom);
    }
    
    auto gain = getGains();
    
    for( auto gDb : gain )
    {
        auto y = jmap(gDb, -48.f, 0.f, float(bottom), float(top));
        
        g.setColour(Colours::darkgrey);
        g.drawHorizontalLine(y, left, right);
    }
}

void FrequencySpectrum::drawTextLabels(juce::Graphics& g)
{
    using namespace juce;
    g.setColour(Colours::lightgrey);
    const int fontHeight = 10;
    g.setFont(fontHeight);
    
    auto renderArea = getAnalysisArea();
    auto left = renderArea.getX();
    
    auto top = renderArea.getY();
    auto bottom = renderArea.getBottom();
    auto width = renderArea.getWidth();
    
    auto freqs = getFrequencies();
    auto xs = getXs(freqs, left, width);
    
    for( int i = 0; i < freqs.size(); ++i )
    {
        auto f = freqs[i];
        auto x = xs[i];

        bool addK = false;
        String str;
        if( f > 999.f )
        {
            addK = true;
            f /= 1000.f;
        }

        str << f;
        if( addK )
            str << "k";
        str << "Hz";
        
        auto textWidth = g.getCurrentFont().getStringWidth(str);

        Rectangle<int> r;

        r.setSize(textWidth, fontHeight);
        r.setCentre(x, 0);
        r.setY(1);
        
        g.drawFittedText(str, r, juce::Justification::centred, 1);
    }
    
    auto gain = getGains();

    for( auto gDb : gain )
    {
        auto y = jmap(gDb, -48.f, 0.f, float(bottom), float(top));

        String str;
        if( gDb > 0 )
            str << "+";
        str << gDb << "dB";
        
        auto textWidth = g.getCurrentFont().getStringWidth(str);
        
        Rectangle<int> r;
        r.setSize(textWidth, fontHeight);
        r.setX(1);
        r.setCentre(r.getCentreX(), y);
        
        g.setColour(Colours::lightgrey);
        
        g.drawFittedText(str, r, juce::Justification::centredLeft, 1);
    }
}
