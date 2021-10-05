/*
  ==============================================================================

    SynthVoice.cpp
    Created: 24 Jul 2021 12:43:29pm
    Author:  tuiji da

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    adsr.noteOn();
    adsr.setSampleRate(getSampleRate());
    noteMidiNumber = midiNoteNumber;
    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    currentAngle = 0.f;
    angleIncrement = frequency / getSampleRate() * juce::MathConstants<float>::twoPi;
    tailOff = 0.0;
    isPlaying = true;
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    if (allowTailOff)
    {
        adsr.noteOff();
    }
    else
    {
        clearCurrentNote();
        level = 0;
        currentAngle = 0.f;
    }
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    // handle pitch wheel moved midi event
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    // handle midi control change
}

void SynthVoice::renderNextBlock (juce::AudioBuffer <float> &outputBuffer, int startSample, int numSamples)
{
    if (isPlaying)
    {
        adsr.setParameters(adsrParameters);
        for (int i = startSample; i < (startSample + numSamples); i++)
        {
            float adsrValue = adsr.getNextSample();
            if(adsrValue <= 0.005)
            {
                isPlaying = false;
            }
            float value = std::sin(currentAngle) * level * adsrValue;
            outputBuffer.addSample(0, i, value);
            outputBuffer.addSample(1, i, value);
            
            currentAngle += angleIncrement;
            if (currentAngle >= juce::MathConstants<float>::twoPi)
            {
                currentAngle -= juce::MathConstants<float>::twoPi;
            }
        }
    }
}

void SynthVoice::setLevel(float newLevel)
{
    level = newLevel;
}

void SynthVoice::setADSR (float attack, float decay, float sustain, float release)
{
    adsrParameters.attack = attack;
    adsrParameters.decay = decay;
    adsrParameters.sustain = sustain;
    adsrParameters.release = release;
}
