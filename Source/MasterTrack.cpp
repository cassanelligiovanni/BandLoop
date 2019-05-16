/*
  ==============================================================================

    MasterTrack.cpp
    Created: 12 May 2019 10:58:13am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "MasterTrack.h"




MasterTrack::MasterTrack() :
masterGainProcessor(masterGain, 2)
{
    masterGain.store(0.);
}

MasterTrack::~MasterTrack()
{
    
    
}

// ===============   COMPONENT Methods  ==================================

/**  The paint() Method gets called when a region of a component needs redrawing,
 Never need to call this method directly
 
 @param g    the graphics context that must be used to do the drawing operations.
 */
void MasterTrack::paint (Graphics& g) {};

/** Called when this component's size has been changed.
 */
void MasterTrack::resized() {};


void MasterTrack::addInputSource (AudioSource* input, const bool deleteWhenRemoved)
{
    if (input != nullptr && ! inputs.contains (input))
    {
        double localRate;
        int localBufferSize;
        
        {
            const ScopedLock sl (lock);
            localRate = currentSampleRate;
            localBufferSize = bufferSizeExpected;
        }
        
        if (localRate > 0.0)
            input->prepareToPlay (localBufferSize, localRate);
        
        const ScopedLock sl (lock);
        
        inputsToDelete.setBit (inputs.size(), deleteWhenRemoved);
        inputs.add (input);
    }
    
    auto* newBuffer = new AudioBuffer<float> ;
    tempBuffers.add(newBuffer);
    
}

void MasterTrack::removeInputSource (AudioSource* const input)
{
    if (input != nullptr)
    {
        std::unique_ptr<AudioSource> toDelete;
        
        {
            const ScopedLock sl (lock);
            const int index = inputs.indexOf (input);
            
            if (index < 0)
                return;
            
            if (inputsToDelete [index])
                toDelete.reset (input);
            
            inputsToDelete.shiftBits (-1, index);
            inputs.remove (index);
        }
        
        input->releaseResources();
    }
}

void MasterTrack::removeAllInputs()
{
    OwnedArray<AudioSource> toDelete;
    
    {
        const ScopedLock sl (lock);
        
        for (int i = inputs.size(); --i >= 0;)
            if (inputsToDelete[i])
                toDelete.add (inputs.getUnchecked(i));
        
        inputs.clear();
    }
    
    for (int i = toDelete.size(); --i >= 0;)
        toDelete.getUnchecked(i)->releaseResources();
}

void MasterTrack::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    tempBuffer.setSize (2, samplesPerBlockExpected);
    
    const ScopedLock sl (lock);
    
    currentSampleRate = sampleRate;
    bufferSizeExpected = samplesPerBlockExpected;
    
    for (int i = inputs.size(); --i >= 0;)
        inputs.getUnchecked(i)->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MasterTrack::releaseResources()
{
    const ScopedLock sl (lock);
    
    for (int i = inputs.size(); --i >= 0;)
        inputs.getUnchecked(i)->releaseResources();
    
    tempBuffer.setSize (2, 0);
    
    currentSampleRate = 0;
    bufferSizeExpected = 0;
}

void MasterTrack::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
    const ScopedLock sl (lock);
    
    
        if (inputs.size() > 0)
        {
            for (int i = 0; i < inputs.size(); i++) {
    
                
                tempBuffers[i]->setSize (jmax (1, info.buffer->getNumChannels()),
                                                info.buffer->getNumSamples());
                
                AudioSourceChannelInfo newTempInfo (tempBuffers[i], 0, info.numSamples);

                        for (int chan = 0; chan < info.buffer->getNumChannels(); ++chan){

                            newTempInfo.buffer->copyFrom (chan, info.startSample, *info.buffer, chan, 0, info.numSamples);

                        }
               
                
                
                tempInfos.add(&newTempInfo);
                
        
            }
        }
    
    
    for (int i = 0; i < inputs.size(); i++) {
        
        inputs.getUnchecked(i)->getNextAudioBlock(*tempInfos[i]);
        
    }
    
    info.clearActiveBufferRegion();
    
    for (int i = 0; i < inputs.size(); i++) {
        
        for (int chan = 0; chan < info.buffer->getNumChannels(); ++chan){
            
            info.buffer->addFrom (chan, info.startSample, *tempBuffers[i], chan, 0, info.numSamples);
        
        }
        
   
        
    }
    
    
    DBG("tempBuffersSize:");
    DBG(tempBuffers.size());
    
    DBG("tempInfosSize:");
    DBG(tempInfos.size());

    tempInfos.clear(false);
    
//    if (inputs.size() > 0)
//    {
//        inputs.getUnchecked(0)->getNextAudioBlock (info);
//
//        if (inputs.size() > 1)
//        {
//            tempBuffer.setSize (jmax (1, info.buffer->getNumChannels()),
//                                info.buffer->getNumSamples());
//
//            AudioSourceChannelInfo info2 (&tempBuffer, 0, info.numSamples);
//
//            for (int i = 1; i < inputs.size(); ++i)
//            {
//                inputs.getUnchecked(i)->getNextAudioBlock (info2);
//
//                for (int chan = 0; chan < info.buffer->getNumChannels(); ++chan)
//                    info.buffer->addFrom (chan, info.startSample, tempBuffer, chan, 0, info.numSamples);
//            }
//        }
//    }
//    else
//    {
//        info.clearActiveBufferRegion();
//    }
}
