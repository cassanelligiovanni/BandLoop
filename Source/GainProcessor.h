#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "/Users/giovanni/BandLoop/Source/PassthroughProcessor.h"


class GainProcessor : public PassthroughProcessor
{
public:
   /**
    * Create the gain processor, which just multiplies all the samples 
    * that pass through it by a floating point gain value. 
    */
    
   GainProcessor(std::atomic<float>& gain, int channelCount);

   ~GainProcessor();


    
   void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;

   float GetGain() const;

    void SetGain(float gain);

private:

    std::atomic<float>& masterGain;
  
    protected :
    /**
    * 0.0 .. 1.0. Defaults to 1.0/unity gain.
    */
   float fGain;


};

