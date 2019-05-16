/*
 ==============================================================================
 ADMinfo.cpp
 Created: 12 Apr 2019 11:14:46am
 Author:  Giovanni Cassanelli
 ==============================================================================
 */

#include "ADMinfo.h"

ADMinfo::ADMinfo (AudioDeviceManager& audioDeviceManager) :
deviceManager(audioDeviceManager)
{
    deviceManager.addChangeListener(this);
    
    
}


ADMinfo::~ADMinfo () {
    
    
    
}


void ADMinfo::addAudioCallback(AudioSourcePlayer* sourcePlayer) {
    
    
    deviceManager.addAudioCallback(sourcePlayer);
    
    
}

void ADMinfo::removeAudioCallback(AudioSourcePlayer* sourcePlayer) {
    
    
    deviceManager.removeAudioCallback(sourcePlayer);
    
    
}

void ADMinfo::changeListenerCallback (ChangeBroadcaster*)
{
    
    bigIntegerInputsAvailable = deviceManager.getCurrentAudioDevice()->getActiveInputChannels();
    fromStereo = getListOfActiveBits(bigIntegerInputsAvailable);
    createInputSelections(fromStereo);
    
    bigIntegerOutputsAvailable = deviceManager.getCurrentAudioDevice()->getActiveOutputChannels();
    toStereo = getListOfActiveBits(bigIntegerOutputsAvailable);
    createOutputSelections(toStereo);
    
    
    this->sendChangeMessage();
    
}



//=========================================================================================
//  Retrieve INPUTS AVAILABLE
//=========================================================================================

void ADMinfo::createInputSelections(Array<int> fromStereo) {
    
    inputsAvailable.clear();
    
    for (int i = 0; i < fromStereo.size(); i++) {
        
        inputsAvailable.add(static_cast<String>(fromStereo[i]));
    }
    
    for (int i = 0; i < fromStereo.size(); i++) {
        
        if ( isOdd(fromStereo[i])  && isPairable(fromStereo[i+1], fromStereo[i]+1)) {
            
            createStereoInput(fromStereo[i], fromStereo[i]+1);
            
        }
    }
}





// createStereoInput
void ADMinfo::createStereoInput(int first, int next) {
    
    String odd = static_cast<String>(first);
    odd += " + " ;
    odd += static_cast<String>(next) ;
    inputsAvailable.add(odd);
    
}


//=========================================================================================
//  Retrieve OUTPUTS AVAILABLE
//=========================================================================================

void ADMinfo::createOutputSelections(Array<int> toStereo) {
    
    outputsAvailable.clear();
    
    for (int i = 0; i < toStereo.size(); i++) {
        
        outputsAvailable.add(static_cast<String>(toStereo[i]));
    }
    
    for (int i = 0; i < toStereo.size(); i++) {
        
        if ( isOdd(toStereo[i])  && isPairable(toStereo[i+1], toStereo[i]+1)) {
            
            createStereoOutput(toStereo[i], toStereo[i]+1);
            
        }
    }
}


// createStereoOutput
void ADMinfo::createStereoOutput(int first, int next) {
    
    String odd = static_cast<String>(first);
    odd += " + " ;
    odd += static_cast<String>(next) ;
    outputsAvailable.add(odd);
    
}

// isPairable?
bool ADMinfo::isPairable(int first, int next) {
    
    return first == next;
    
}




// isOdd ?
bool ADMinfo::isOdd(int toStereo) {
    
    return toStereo % 2 != 0;
    
}

