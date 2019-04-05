/*
  ==============================================================================

    DecibelSlider.h
    Created: 1 Apr 2019 10:48:06am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DecibelSlider : public Slider
{
public:
    DecibelSlider();

    double getValueFromText (const String& text);

    String getTextFromValue (double value);


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecibelSlider)
};
