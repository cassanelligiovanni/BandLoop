/*
  ==============================================================================

    MasterTrack.h
    Created: 12 May 2019 10:58:13am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once
/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/GainProcessor.h"

class MasterTrack  : public MixerAudioSource,
                    public Component
{
public:
    //==============================================================================
    /** Creates an empty AudioSourcePlayer. */
    MasterTrack();

    /** Destructor.

        Make sure this object isn't still being used by an AudioIODevice before
        deleting it!
    */
    ~MasterTrack() override;
    
    // ===============   COMPONENT Methods  ==================================
    
    /**  The paint() Method gets called when a region of a component needs redrawing,
     Never need to call this method directly
     
     @param g    the graphics context that must be used to do the drawing operations.
     */
    void paint (Graphics& g) override;
    
    /** Called when this component's size has been changed.
     */
    void resized() override;

    
    //==============================================================================
    /** Adds an input source to the mixer.
     
     If the mixer is running you'll need to make sure that the input source
     is ready to play by calling its prepareToPlay() method before adding it.
     If the mixer is stopped, then its input sources will be automatically
     prepared when the mixer's prepareToPlay() method is called.
     
     @param newInput             the source to add to the mixer
     @param deleteWhenRemoved    if true, then this source will be deleted when
     no longer needed by the mixer.
     */
    void addInputSource (AudioSource* newInput, bool deleteWhenRemoved);
    
    /** Removes an input source.
     If the source was added by calling addInputSource() with the deleteWhenRemoved
     flag set, it will be deleted by this method.
     */
    void removeInputSource (AudioSource* input);
    
    /** Removes all the input sources.
     Any sources which were added by calling addInputSource() with the deleteWhenRemoved
     flag set will be deleted by this method.
     */
    void removeAllInputs();
    
    //==============================================================================
    /** Implementation of the AudioSource method.
     This will call prepareToPlay() on all its input sources.
     */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    /** Implementation of the AudioSource method.
     This will call releaseResources() on all its input sources.
     */
    void releaseResources() override;
    
    /** Implementation of the AudioSource method. */
    void getNextAudioBlock (const AudioSourceChannelInfo&) override;
    
    
private:
    //==============================================================================
   
    OwnedArray<AudioBuffer<float>> tempBuffers ;
    OwnedArray<AudioSourceChannelInfo> tempInfos ;
    
    Array<AudioSource*> inputs;
    BigInteger inputsToDelete;
    CriticalSection lock;
    AudioBuffer<float> tempBuffer;
    double currentSampleRate;
    int bufferSizeExpected;

    int outputL = 0 ;
    int outputR = 1 ;
    
    std::atomic<float> masterGain;
    
    GainProcessor masterGainProcessor ;
    
//    CriticalSection readLock;
    AudioSource* source = nullptr;
    double sampleRate = 0;
    int bufferSize = 0;
    float* channels[128];
    float* outputChans[128];
    const float* inputChans[128];
//    AudioBuffer<float> tempBuffer;
    float lastGain = 1.0f, gain = 1.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterTrack)
};


