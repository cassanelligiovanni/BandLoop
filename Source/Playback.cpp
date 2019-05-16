/*
 ==============================================================================
 
 SynthAudioSource.cpp
 Created: 2 Mar 2019 4:55:36pm
 Author:  Giovanni Cassanelli
 
 ==============================================================================
 */

#include "Playback.h"

Playback::Playback()
{
    
    playButtonImage = Drawable::createFromImageData (BinaryData::StartPlayingButton_svg, BinaryData::StartPlayingButton_svgSize);
    stopButtonImage = Drawable::createFromImageData (BinaryData::StopPlayingButton_svg, BinaryData::StopPlayingButton_svgSize);
    DrawablePlayButton.setImages ( playButtonImage,
                                    playButtonImage,
                                    playButtonImage,
                                    playButtonImage,
                                    stopButtonImage,
                                    stopButtonImage,
                                    stopButtonImage, stopButtonImage);
    addAndMakeVisible(DrawablePlayButton);
    
    DrawablePlayButton.addListener(this);
    DrawablePlayButton.setToggleState(true, dontSendNotification);
//    DrawablePlayButton.onClick = [this]() { isStoppingPlaying = true ;};
    
    for (int i = 10; i < 120; i++) {
        
        notesAvailable.add(i);
    }
    
    for (auto i = 0; i < 16; ++i)
    {
        synth.addVoice (new SamplerVoice());    // and these ones play the sampled sounds
    }
    
    DrawablePlayButton.setBounds(getLocalBounds());
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

void Playback::getNextAudioBlock(AudioBuffer<float>& outputBuffer, int numSamples) {

    MidiBuffer incomingMidi;

    synth.renderNextBlock (outputBuffer, incomingMidi, 0, numSamples);
   
}



void Playback::paint (Graphics& g) {};


void Playback::resized() {
    
    DrawablePlayButton.setBounds(getLocalBounds());
};


void Playback::buttonClicked (Button* button) {
    
    if (button == &DrawablePlayButton)
    {
        if (isPlaying)
        {
            isStoppingPlaying = !isStoppingPlaying ;
            DrawablePlayButton.setToggleState(!(DrawablePlayButton.getToggleState()), dontSendNotification);
            }
        
        
        else if (!isPlaying)
        {
            isStartingPlaying  = !isStartingPlaying ;
            DrawablePlayButton.setToggleState(!(DrawablePlayButton.getToggleState()), dontSendNotification);
        };
    
     
 }

}

void Playback::createSound(File lastRecording, File actualFolder, float loopUnit) {
    
    WavAudioFormat wavFormat;
    
    Array<File> resultFiles;
    
    File parentDir = actualFolder;
    
    FileSearchPath path(parentDir.getFullPathName());
    
    path.findChildFiles(resultFiles, 2, false, lastRecording.getFileName());
    
    File toPlay = resultFiles.getLast();

    
    std::unique_ptr<AudioFormatReader> audioReader2 (wavFormat.createReaderFor (toPlay.createInputStream(), true));
    
    
    BigInteger b;
    b.setBit(notesAvailable.getFirst());
    
    synth.addSound (new SamplerSound ("bass2wav",
                                         *audioReader2,
                                         b,
                                         notesAvailable.getFirst(),   // root midi note
                                         0.2,  // attack time
                                         0.5,  // release time
                                         10.0  // maximum sample length
                                         ));
    
    
    int length = round((audioReader2->lengthInSamples/audioReader2->sampleRate)/loopUnit);

    Sound* newSound = new Sound(actualFolder, lastRecording, actualBar, length, notesAvailable.getFirst());
    sounds.add(newSound);
    
    notesAssigned.add(notesAvailable.getFirst());
    
    notesAvailable.remove(0);

    
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
   
    
    if (isStartingPlaying == true) {
        isPlaying = true ;
        isStartingPlaying = false;
    }
    
    if (isStoppingPlaying == true) {
        isPlaying = false ;
        isStoppingPlaying = false;
    }
    
    if (isPlaying) {
    
    for(int i = 0; i < sounds.size(); i++) {
        
         if(((actualBar-(sounds[i]->getStart()))%(sounds[i]->getLength())) == 0){
         
            synth.noteOn(1,sounds[i]->getNote(), 0.8f);
           
        }
     }
}
    updateGui();
    
};

void Playback::stopImmediatelly() {
    
     for(int i = 0; i < sounds.size(); i++) {
         
          synth.noteOff(1,sounds[i]->getNote(), 0.8f, false);
         
     }
    
    isPlaying = false;
     isStartingPlaying = false;
    
    updateGui();
}


void Playback::startOrStopAtNextBar(){
    
    if (isPlaying)
           isStoppingPlaying = true;
    
    if (!isPlaying)
        isStartingPlaying = true;
};

void Playback::startRecording() {
    
    isStartingPlaying = true;
}

void Playback::updateGui() {
    
    if (isPlaying)
        DrawablePlayButton.setToggleState(true
                                          , dontSendNotification);
        
    if (!isPlaying)
        DrawablePlayButton.setToggleState(false
                                          , dontSendNotification);

}


void Playback::newSong() {};

void Playback::save() {};
