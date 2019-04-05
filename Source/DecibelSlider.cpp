/*
  ==============================================================================

    DecibelSlider.cpp
    Created: 1 Apr 2019 10:48:06am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "DecibelSlider.h"

DecibelSlider::DecibelSlider() {}

double    DecibelSlider::getValueFromText (const String& text)
    {
        auto minusInfinitydB = -100.0;

        auto decibelText = text.upToFirstOccurrenceOf ("dB", false, false).trim();    // [1]

        return decibelText.equalsIgnoreCase ("-INF") ? minusInfinitydB
                                                     : decibelText.getDoubleValue();  // [2]
    }

String    DecibelSlider::getTextFromValue (double value)
    {
        return Decibels::toString (value);
    }

