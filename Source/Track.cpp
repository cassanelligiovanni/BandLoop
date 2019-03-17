/*
  ==============================================================================

    Track.cpp
    Created: 11 Mar 2019 12:56:15pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "Track.h"


Track::Track(String name,
             Slider* slider,
             TextButton* playStop,
             String inputs,
             MidiMessageCollector* midiCollector,
             const ValueTree& v,
             UndoManager& um)
:
tree (v), undoManager (um),
recorder(name, selectFolder(name)) //initialise the AudioRecorder with a name create a folder out of it.

{
    
   tree.addListener (this);
    DBG("fuck");
    
    
}

    Track::~Track(){
    }
    
    void Track::process(AudioBuffer<float> bufferIn, const AudioSourceChannelInfo& bufferOut, Slider* slider, int input){}
    
    void Track::setValue(float value){}

//=============================================================================
    void Track::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
        bufferToRecord.clear();
        bufferToRecord.setSize(1, samplesPerBlockExpected);
    }

//=============================================================================
    void Track::releaseResources() {}


//=============================================================================
    void Track::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {
        if(recording == true){
            //copy the desired content(channel) from bufferToFill and pass it to the recorder
        const auto numSamples = bufferToFill.buffer->getNumSamples();
        bufferToRecord.copyFrom(0, 0, *bufferToFill.buffer, 2, 0, numSamples);
        recorder.Record(&bufferToRecord);
        }
       

    
    }

//=============================================================================
void Track::startRecording(String name) {
    
    recording = true;
        //pass to the function that will create a writer (recorder.startRecording...) and
        //                          will also create the File (.getNonExis...) and pass it;
    recorder.startRecording(&bufferToRecord, this->selectFolder(name).getNonexistentChildFile(name, ".wav"));

}



//create a folder named as the Track;
//=============================================================================
File Track::selectFolder(String name) {
auto parentDir = File::getSpecialLocation (File::userDocumentsDirectory).getChildFile("BandLoop").getChildFile("Temporary").getChildFile(name);
    File myFolder(parentDir);
    bool b = myFolder.createDirectory();
    return myFolder;
}


void Track::valueTreeChildAdded (ValueTree& parentTree, ValueTree&) {};
void Track::valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) {};
void Track::valueTreeChildOrderChanged (ValueTree& parentTree, int, int) {};
void Track::valueTreeParentChanged (ValueTree&) {};

void Track::valueTreePropertyChanged (ValueTree&, const Identifier& property) {
    
    DBG(property);
    Identifier propertyName = property;
    String pair = tree.getProperty(propertyName);
    DBG(pair);
    
 

    
};
