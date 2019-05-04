/*
  ==============================================================================

    GradientLabel.cpp
    Created: 22 Apr 2019 11:01:17am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "GradientLabel.h"

 
GradientLabel::GradientLabel() 

{
    
    addAndMakeVisible(text);
    
}
    
GradientLabel::~GradientLabel(){
        
        
}
    

void GradientLabel::paint(Graphics& g) {
    
        Rectangle<int> internalBounds = bound.reduced(2);
    
    Path external;
    external.addRoundedRectangle(bound, 5, 5);

    float x1 = bound.getX();
    float y1 = bound.getY()+bound.getHeight()/2;
    float x4 = bound.getWidth();
    
    ColourGradient gradientExternal (colour.darker(0.15), x1, y1,
                                      colour.darker(0.15), x4, y1, false);
    
    gradientExternal.addColour(0.9, colour.brighter(0.3));
    gradientExternal.addColour(0.1, colour.brighter(0.3));
    
    g.setGradientFill (gradientExternal);
    g.fillPath (external);
    
    
    Path internal;
    internal.addRoundedRectangle(internalBounds, 5, 5);
    
     x1 = internalBounds.getX();
     y1 = internalBounds.getY()+internalBounds.getHeight()/2;
     x4 = internalBounds.getWidth();
    
    ColourGradient gradientInternal (colour.brighter(0.1), x1, y1,
                                     colour.brighter(0.1), x4, y1, false);
    
    gradientInternal.addColour(0.9, colour);
    gradientInternal.addColour(0.1, colour);
    
    g.setGradientFill (gradientInternal);
    g.fillPath (internal);
    
}


void GradientLabel::resized(){
    
    bound = getLocalBounds();
    text.setBounds(bound);
    
}

void GradientLabel::setName(String nameOfTrack, int size) {
    
    text.setColour (Label::textColourId, Colours::BandLoopText);
    text.setJustificationType (Justification::centred);
    textProperties.set ("fontSize", size);
    textProperties.set ("fontStyleFlag", Font::plain);
    text.setText("choose a name...", dontSendNotification);
    text.setEditable(true);
    text.setColour(Label::backgroundColourId, Colours::transparentWhite);
    text.setColour(Label::outlineColourId, Colours::transparentWhite);

    
}

void GradientLabel::setColour(Colour colours) {
    
    colour = colours;
    
}
