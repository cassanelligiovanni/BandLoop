/*
  ==============================================================================

    Gain.cpp
    Created: 11 May 2019 10:05:36pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/



#include "GainProcessor.h"

GainProcessor::GainProcessor(std::atomic<float>& gain, int channelCount) :
PassthroughProcessor(channelCount, channelCount),
masterGain(gain),
fGain(1.0)

{

}

GainProcessor::~GainProcessor()
{

}

void GainProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
   float gain = this->GetGain();


   buffer.applyGain(0, buffer.getNumSamples(), this->GetGain());

}

void GainProcessor::SetGain(float gain)
{
    masterGain.store(gain);


}

float GainProcessor::GetGain() const
{

    return masterGain.load();
}


