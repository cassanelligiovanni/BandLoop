/*
 ==============================================================================
 
 SynthAudioSource.h
 Created: 2 Mar 2019 4:55:36pm
 Author:  Giovanni Cassanelli
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "/Users/giovanni/BandLoop/Source/Sound.h"
#pragma once

class Playback
{
    
public :
    Playback(MidiMessageCollector& fromPedal);
    ~Playback();
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    void releaseResources();
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);
    void createSound(File lastRecording, File actualFolder, float loopUnit);
    
    void checkEvent(int bar);
    void triggerEvent();
    
private:
    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    Array< File> founds;
    File toPlay;
    OwnedArray<Sound> sounds;
    bool newBar = false;
    int actualBar;
    
    Array<int> noteAvailable;
    Array<int> noteAssigned;

    
public :
    MidiMessageCollector& midiCollector;
    MidiMessageCollector playbackCollector;
    Synthesiser synth;
    
    MidiBuffer midiQueue;
    float loopUnit;
    
};
