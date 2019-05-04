/*
  ==============================================================================

    Click.cpp
    Created: 12 Apr 2019 9:04:53am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "Click.h"

Click::Click () {
    
    for (auto i = 0; i < 8; ++i)
    {
        clickSounds.addVoice (new SamplerVoice());    // and these ones play the sampled sounds
    }
    
};

Click::~Click () {};



void Click::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
    
    {
        clickSounds.setCurrentPlaybackSampleRate (sampleRate);
        AudioFormatManager afm;
        afm.registerBasicFormats();
        ScopedPointer<AudioFormatReader> reader = afm.createReaderFor (new MemoryInputStream (BinaryData::one_wav, BinaryData::one_wavSize, false));
        BigInteger b;
        b.setBit(10);
        clickSounds.addSound (new SamplerSound ("1",
                                          *reader,
                                          b,
                                          10,   // root midi note
                                          0.2,  // attack time
                                          0.5,  // release time
                                          10.0  // maximum sample length
                                          ));
        
        ScopedPointer<AudioFormatReader> reader2 = afm.createReaderFor (new MemoryInputStream (BinaryData::two_wav, BinaryData::two_wavSize, false));
        BigInteger b2;
        b2.setBit(11);
        clickSounds.addSound (new SamplerSound ("2",
                                                *reader2,
                                                b2,
                                                11,   // root midi note
                                                0.2,  // attack time
                                                0.5,  // release time
                                                10.0  // maximum sample length
                                                ));

    }
    
    
    
};
void Click::releaseResources() {};

void Click::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {
   
    bufferToFill.clearActiveBufferRegion();
    int numSamples = bufferToFill.numSamples ;
    MidiBuffer incomingMidi;
    clickSounds.renderNextBlock (*bufferToFill.buffer, incomingMidi, 0, numSamples);

};


void Click::nextBar () {
    if (flagOne) {
        clickSounds.noteOn(1,10, 0.8f);
        flagOne =false;
   }
    if (flagTwoThreeFour) {
        clickSounds.noteOn(1,11, 0.8f);
        flagTwoThreeFour =false;
    }
};
