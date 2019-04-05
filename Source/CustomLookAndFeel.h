/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 29 Mar 2019 5:24:38pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class CustomLookAndFeel    : public LookAndFeel_V4
{
    
    public :

    CustomLookAndFeel();

    
    void drawRoundThumb (Graphics& g, float x, float y, float diameter, Colour colour, float outlineThickness) ;
    
    
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override;
   
    Font getLabelFont (Label& label) override;
    
    void drawTickBox (Graphics& g, Component& component,
                      float x, float y, float w, float h,
                      bool ticked,
                      bool isEnabled,
                      bool isMouseOverButton,
                      bool isButtonDown) override;
    
    
//    void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
//                                float sliderPos, float minSliderPos, float maxSliderPos,
//                                const Slider::SliderStyle style, Slider& slider) override;
//
//    
//    void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
//                           float sliderPos, float minSliderPos, float maxSliderPos,
//                           const Slider::SliderStyle style, Slider& slider) override;
//    
//    
//    void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
//                                     float /*sliderPos*/,
//                                     float /*minSliderPos*/,
//                                     float /*maxSliderPos*/,
//                                     const Slider::SliderStyle /*style*/, Slider& slider) override;
//
//    
//    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
//                           float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;
//    
//    
    Typeface::Ptr getTypefaceForFont(const Font& f) override
    {
        static Typeface::Ptr myFont = Typeface::createSystemTypefaceFor(BinaryData::FallingSky_otf,
                                                                        BinaryData::FallingSky_otfSize);
        return myFont;
    }

};
