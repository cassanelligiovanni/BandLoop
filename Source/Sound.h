/*
  ==============================================================================

    cancelTrack.h
    Created: 26 Mar 2019 5:56:09pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Sound
{
public:
    
    Sound(File fFolder, File fFile, int fstart, int flength, int fnote);
    
    /**
     * dtor.
     */
    
    ~Sound();
    
    /**
     * dtor.
     */
    File folder;
    File file ;
    int start ;
    int length ;
    bool readyToPlay ;
    int note ;
    
private:
    
    
};
