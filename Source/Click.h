/*
  ==============================================================================

    Click.h
    Created: 12 Apr 2019 9:04:53am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class Click : public AudioSource
{
    public:

    Click ();
    
    ~Click();
    
    
    //==============================================================================
    /**  This callback allows the source to initialise any resources it might need when playing.
     * It initialise the Synthesiser Click with the two different .wav sounds.
     @param samplesPerBlockExpected  (bufferSize)
     @param sampleRate (Hz)
     */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    
    //==============================================================================
    /** Allows the source to release anything it no longer needs after playback has stopped.
     */
    void releaseResources() override;
   
    
    //==============================================================================
    /** This callback will be made each
     time the audio playback hardware (or whatever other destination the audio
     data is going to) needs another block of data.
     \n In this case it will check the flags and play the Click sounds if needed.
     */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    
    void nextBar();
    
    bool flagOne = false;  //!<  True whan Beat 1 (4)
    bool flagTwoThreeFour = false; //!<  True whan Beat 2-3-4
    
    private :
    
    Synthesiser clickSounds ; //!<  Class that actually play the sound


    
};
