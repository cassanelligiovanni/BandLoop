/*
  ==============================================================================

    ADMinfo.h
    Created: 12 Apr 2019 11:14:46am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class ADMinfo : public ChangeBroadcaster,
                public ChangeListener
{

  public :  
    
ADMinfo (AudioDeviceManager& audioDeviceManager);
        
~ADMinfo();
    
    /**
     * Add an AudioSourcePlayer to AudioDeviceManager
     * the AudioSourcePlayer will be filled with an AudioSource which will be our Track
     */
    void addAudioCallback(AudioSourcePlayer* sourcePlayer);
    
    /**
     * Add an AudioSourcePlayer to AudioDeviceManager
     * the AudioSourcePlayer will be filled with an AudioSource which will be our Track
     */
    void removeAudioCallback(AudioSourcePlayer* sourcePlayer);
    
    
    
    /**
     * Called when the number of input is changed,
     * It will trigger the createInputSelections()
     */
    
    void changeListenerCallback (ChangeBroadcaster*) override;

    /**
     * Fill the array and ComboBox with the available inputs
     */
    void createInputSelections(Array<int> fromStereo);
    
    /**
     * Create stereo inputs(ex : "1+2", "3+4" ... )
     */
    void createStereoInput(int first, int next);
    
    
    /**
     * Fill the array and ComboBox with the available outputs
     */
    void createOutputSelections(Array<int> toStereo);
    
    /**
     * Create stereo outputs(ex : "1+2", "3+4" ... )
     */
    void createStereoOutput(int first, int next);

    bool isPairable(int first, int next);
    
    bool isOdd(int toStereo);
    
    
    StringArray inputsAvailable;
       StringArray outputsAvailable;

    
 private :
    
    AudioDeviceManager& deviceManager;

    BigInteger bigIntegerInputsAvailable ;
    
    Array<int> fromStereo; /**< enum value 1 */
    
    BigInteger bigIntegerOutputsAvailable ;
    
    Array<int> toStereo; /**< enum value 1 */
   
    /**
     * Get Inputs Array from BigInteger format
     */
     static Array<int> getListOfActiveBits (const BigInteger& b) //  Get Inputs from BigInteger
    {
        Array<int> inputsAvailable;
        for (auto i = 0; i <= b.getHighestBit(); ++i)
            if (b[i])
                inputsAvailable.add(int (i)+1);
        return inputsAvailable; };
    

};
