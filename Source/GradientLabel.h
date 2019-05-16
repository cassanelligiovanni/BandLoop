/*
  ==============================================================================

    GradientLabel.h
    Created: 22 Apr 2019 11:01:17am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/CustomLookAndFeel2.h"


class GradientLabel : public Component

{
    
  public:
    
    GradientLabel();
    
    ~GradientLabel();
    
    void paint(Graphics& g) override;
    
    void resized() override;
    
    void setColour(Colour colours);
    void setName(String nameOfTrack, int size) ;
    
    Label text;
       
private:
    
    Rectangle<int> bound;
    Colour colour;
    
    NamedValueSet& textProperties = text.getProperties();
    
};
    
