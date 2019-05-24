/*
 ==============================================================================
 
 SynthAudioSource.cpp
 Created: 2 Mar 2019 4:55:36pm
 Author:  Giovanni Cassanelli
 
 ==============================================================================
 */

#include "Playback.h"

Playback::Playback(RecordState& recordState) :
recState(recordState)
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
        
        if (recState == RecordState::Playing ) {
            recState = RecordState::isStartingStopping;
            DrawablePlayButton.setToggleState(false, dontSendNotification);
        }
        
        if (recState == RecordState::Recording ) {
            recState = RecordState::isStartingStopping;
            DrawablePlayButton.setToggleState(false, dontSendNotification);
        }
            
        if (recState == RecordState::Stopped) {
            recState = RecordState::isStartingPlaying;
            DrawablePlayButton.setToggleState(true, dontSendNotification);
        }
        
        
//        if (isPlaying)
//        {
//            isStoppingPlaying = !isStoppingPlaying ;
//            DrawablePlayButton.setToggleState(!(DrawablePlayButton.getToggleState()), dontSendNotification);
//            }
//
//
//        else if (!isPlaying)
//        {
//            isStartingPlaying  = !isStartingPlaying ;
//            DrawablePlayButton.setToggleState(!(DrawablePlayButton.getToggleState()), dontSendNotification);
//        };
    
     
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


//    std::cout<< " loopUnit : " <<  loopUnit  << std::endl;
//    std::cout<< " length : " << length << std::endl;
//    std::cout<< " actualBar : " <<  actualBar << std::endl;

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
    std::cout<<"Has Triggered the Event with bar : "   << actualBar <<std::endl;
    
    if (recState == RecordState::Stopped) {
        
        for(int i = 0; i < sounds.size(); i++) {
            
            synth.noteOff(1,sounds[i]->getNote(), 0.8f, false);
            
        }
        std::cout<< " stopped " <<std::endl;

    }
    
    if (!(recState == RecordState::Stopped)) {
    
       
        
    for(int i = 0; i < sounds.size(); i++) {
//        std::cout<< " start : " <<  sounds[i]->getStart()  << std::endl;
//        std::cout<< " length : " << sounds[i]->getLength() << std::endl;
//        std::cout<< " actualBar : " <<  actualBar << std::endl;
//        std::cout<< actualBar << "-" <<  sounds[i]->getStart()  << "%" << sounds[i]->getLength() << "=" << (actualBar-(sounds[i]->getStart()))%(sounds[i]->getLength())<< std::endl;
        
         if(((actualBar-(sounds[i]->getStart()))%(sounds[i]->getLength())) == 0){
            synth.noteOff(1,sounds[i]->getNote(), 0.8f, false);
            synth.noteOn(1,sounds[i]->getNote(), 0.8f);

             
        
        }
     }
}
   
    
    
};

void Playback::stopImmediatelly() {
    
    if(!sounds.isEmpty()) {
    
     for(int i = 0; i < sounds.size(); i++) {
         
          synth.noteOff(1,sounds[i]->getNote(), 0.8f, false);
         
     }
    }

}


void Playback::startOrStopAtNextBar(){
    
//    if (isPlaying)
//           isStoppingPlaying = true;
//    
//    if (!isPlaying)
//        isStartingPlaying = true;
};

void Playback::startRecording() {
    
//    isStartingPlaying = true;
}

void Playback::updateGui() {
    
//    if (isPlaying)
//        DrawablePlayButton.setToggleState(true
//                                          , dontSendNotification);
//
//    if (!isPlaying)
//        DrawablePlayButton.setToggleState(false
//                                          , dontSendNotification);

}


void Playback::newSong(File oldFolder, File actualFolder) {
    
    
    
    for (int i = 10; i < 120; i++) {
        
        notesAvailable.add(i);
    }
    
    notesAssigned.clear();
    
    
    Array<File> filesToDelete;
    FileSearchPath path(actualFolder.getFullPathName());
    path.findChildFiles(filesToDelete, 2, false, "*.wav");
    for (int i = 0; i <  actualFolder.getNumberOfChildFiles(2); i++ ) {
        
        filesToDelete[i].deleteFile() ;
        
        
    };
    
    
    Array<File> filesToDelete2;
    FileSearchPath path2(oldFolder.getFullPathName());
    path.findChildFiles(filesToDelete2, 2, false, "*.wav");
    for (int i = 0; i <  oldFolder.getNumberOfChildFiles(2)+1; i++ ) {
        
        filesToDelete2[i].deleteRecursively() ;
        
    };
    
    sounds.clear();
    
};

void Playback::save(File savedFolder, File actualFolder) {
    
    
    File parentDir = actualFolder;
    
    actualFolder.copyDirectoryTo(savedFolder);
    

    
};
