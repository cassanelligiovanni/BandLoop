/*
 ==============================================================================
 
 SynthAudioSource.h
 Created: 2 Mar 2019 4:55:36pm
 Author:  Giovanni Cassanelli
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"

#pragma once

class SynthAudioSource
{
    public :
    SynthAudioSource();
    
    void playback (const AudioSourceChannelInfo& bufferToFill);
    
    
    
private:
    
    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    
    Array< File> founds;
    File toPlay;
    
    public :

    MidiMessageCollector midiCollector;
//    MidiKeyboardState& keyboardState;
    Synthesiser synth;
};
