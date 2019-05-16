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
    toStereo = getListOfActiveBits(bigIntegerInputsAvailable);
    createInputSelections(toStereo);

    this->sendChangeMessage();
    
}



//=========================================================================================
//  Retrieve INPUTS AVAILABLE
//=========================================================================================

void ADMinfo::createInputSelections(Array<int> toStereo) {
    
    inputsAvailable.clear();
    
    for (int i = 0; i < toStereo.size(); i++) {
        
        inputsAvailable.add(static_cast<String>(toStereo[i]));
    }
    
    for (int i = 0; i < toStereo.size(); i++) {
        
        if ( isOdd(toStereo[i])  && isPairable(toStereo[i+1], toStereo[i]+1)) {
    
             createStereoInput(toStereo[i], toStereo[i]+1);
            
        }
    }
}


// isPairable?
bool ADMinfo::isPairable(int first, int next) {
    
    return first == next;
    
}

// isOdd ?
bool ADMinfo::isOdd(int toStereo) {
    
    return toStereo % 2 != 0;
    
}

// createStereoInput
void ADMinfo::createStereoInput(int first, int next) {

    String odd = static_cast<String>(first);
    odd += " + " ;
    odd += static_cast<String>(next) ;
    inputsAvailable.add(odd);
    
}

