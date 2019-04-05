/*
 ==============================================================================
 
 SynthAudioSource.cpp
 Created: 2 Mar 2019 4:55:36pm
 Author:  Giovanni Cassanelli
 
 ==============================================================================
 */

#include "Playback.h"

Playback::Playback(MidiMessageCollector& fromPedal) : midiCollector(fromPedal)
{
    for (int i = 10; i < 120; i++) {
        
        noteAvailable.add(i);
    }
    
    DBG(noteAvailable.size());
    
    for (auto i = 0; i < 8; ++i)
    {
        synth.addVoice (new SamplerVoice());    // and these ones play the sampled sounds
    }
}

Playback::~Playback()
{
}
void Playback::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    playbackCollector.reset(sampleRate);
    synth.setCurrentPlaybackSampleRate (sampleRate);
}


void Playback::releaseResources()
{
    
  
}

void Playback::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {

    MidiBuffer incomingMidi;

    synth.renderNextBlock (*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
    
   
}

void Playback::createSound(File lastRecording, File actualFolder, float loopUnit) {
    
    WavAudioFormat wavFormat;
    
    Array<File> resultFiles;
    
    File parentDir = actualFolder;
    
    FileSearchPath path(parentDir.getFullPathName());
    
    path.findChildFiles(resultFiles, 2, false, lastRecording.getFileName());
    
    File toPlay = resultFiles.getLast();
    
//    std::cout<<"founds:";
//    std::cout<<resultFiles.size()<<std::endl;
//    std::cout<<"nameOfLast:";
//    std::cout<<resultFiles.getLast().getFileName()<<std::endl;
    
    std::unique_ptr<AudioFormatReader> audioReader2 (wavFormat.createReaderFor (toPlay.createInputStream(), true));
    
//    std::cout<<"length in samples"<<std::endl;
//    std::cout<<audioReader2->lengthInSamples/audioReader2->sampleRate<<std::endl;
    
    BigInteger b;
    b.setBit(noteAvailable.getFirst());
    
    synth.addSound (new SamplerSound ("bass2wav",
                                         *audioReader2,
                                         b,
                                         noteAvailable.getFirst(),   // root midi note
                                         0.2,  // attack time
                                         0.5,  // release time
                                         10.0  // maximum sample length
                                         ));
    
    
    int length = round((audioReader2->lengthInSamples/audioReader2->sampleRate)/loopUnit);
//
    Sound* newSound = new Sound(actualFolder, lastRecording, actualBar, length, noteAvailable.getFirst());
    sounds.add(newSound);
    
    noteAssigned.add(noteAvailable.getFirst());
    
//    DBG("noteAssigned :");
//    DBG(noteAssigned.getLast());
    
    noteAvailable.remove(0);
    
//    auto message = MidiMessage::noteOn (1, 74, 0.8f);
//    message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
//    midiCollector.addMessageToQueue(message);
    
    
}


void Playback::checkEvent(int bar){
    
    actualBar = bar;
    
//    for(int i = 0; i < sounds.size(); i++) {
//
//        if(((actualBar-(sounds[i]->start))%(sounds[i]->length)) == 0){
//
//            auto message = MidiMessage::noteOn (1, sounds[i]->note, 0.8f);
//            message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
//            playbackCollector.addMessageToQueue(message);
//
//        }
//    }
    
   
}


void Playback::triggerEvent(){
    
    
    for(int i = 0; i < sounds.size(); i++) {
//        DBG(actualBar);
//        DBG(sounds[i]->start);
//        DBG(sounds[i]->length);
        if(((actualBar-(sounds[i]->start))%(sounds[i]->length)) == 0){
         
//            auto message = MidiMessage::noteOn (1, sounds[i]->note, 0.8f);
//            message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
            synth.noteOn(1,sounds[i]->note, 0.8f);
//            playbackCollector.addMessageToQueue(message);
            
        }
    }
    
};
