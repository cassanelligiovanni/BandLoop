/*
  ==============================================================================

    DecibelSlider.h
    Created: 1 Apr 2019 10:48:06am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

//==============================================================================
/**
 A derived class of juce::Slider made to adapt to DSP need.
 
 A vertical slider with a range from -INF db to +6 db
 
It will control the gain of each track, its recording amplitude, and its playback output
 */


#include "../JuceLibraryCode/JuceHeader.h"

class DecibelSlider : public Slider
{
public:
    
    /**
     *Creates a slider with Decibel range.
     */
    DecibelSlider();

    /**
     * @return Return the value when the slider text is changed
     */
    double getValueFromText (const String& text);

    /**
     * @return Return String when the slider value is changed
     */
    String getTextFromValue (double value);


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecibelSlider)
};
