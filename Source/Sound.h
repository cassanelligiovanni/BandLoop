/*
  ==============================================================================

    Sound.h
    Created: 26 Mar 2019 5:56:09pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

/**
 A simple class to keep track of all the .wav loops,
 
 It is initilised right after the loop is recorded.
 
 all its parameters are useful in order to keep track of wheter it has to be triggered at next bar
 */

#include "../JuceLibraryCode/JuceHeader.h"

class Sound
{
public:
    
    /**
     * Creates a Sound right after recording is completed and initialise with the .wav properties.
     */
    Sound(File fFolder, File fFile, int fstart, int flength, int fnote);
    
    ~Sound();
    /**
     * @return Return  the Bar when it started to playback
     */
    int getStart();
    /**
     * @return Return  the Length in bar of the loop
     */
    int getLength();
    /**
     * @return Return  the Note assigned to this track
     */
    int getNote();
    
   
    //==============================================================================
    /** Called when Global BPM/Tempo is resetted to 0, this starting point will be 0  too.
     @param newStart         "usually 0"
     */
    void setStart(int newStart);

    /** set a New Length for the Sound.
     @param newLength
     */
    void setLength(int newLength);

    /** set a New Note for the Sound.
     @param newNote
     */
    void setNote(int newNote);
    
private :
    
    File folder;
    File file ;
    int start ;
    int length ;
//    bool readyToPlay ;
    int note ;
    
};
