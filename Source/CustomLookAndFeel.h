/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 29 Mar 2019 5:24:38pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Fader.h"

//#include "/Users/giovanni/Downloads/ff_meters-master 2/ff_meters.h"


class CustomLookAndFeel    : public LookAndFeel_V4
//,public FFAU::LevelMeter::LookAndFeelMethods
{
    
//#include "/Users/giovanni/Downloads/ff_meters-master 2/ff_meters_LookAndFeelMethods.h"

    
    public :
    
    CustomLookAndFeel();

    
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override;
   
    Font getLabelFont (Label& label) override;
    
 
    
    void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
                                float sliderPos, float minSliderPos, float maxSliderPos,
                                const Slider::SliderStyle style, Slider& slider) override;

//    
    void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const Slider::SliderStyle style, Slider& slider) override;
    
//    
    void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
                                     float /*sliderPos*/,
                                     float /*minSliderPos*/,
                                     float /*maxSliderPos*/,
                                     const Slider::SliderStyle /*style*/, Slider& slider) override;
    
    
    void drawPopupMenuBackground (Graphics& g, int width, int height) override;
   void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                       const bool isSeparator, const bool isActive,
                       const bool isHighlighted, const bool isTicked,
                       const bool hasSubMenu, const String& text,
                       const String& shortcutKeyText,
                           const Drawable* icon, const Colour* const textColourToUse) override;
    
    Font getTextButtonFont (TextButton& box, int buttonHeight) override;
    
    void drawComboBoxTextWhenNothingSelected (Graphics& g, ComboBox& box, Label& label) override;
    
    
    Label *     createComboBoxTextBox (ComboBox &box)override;
    
    void    positionComboBoxText (ComboBox &box, Label &label) override;
    
    
    PopupMenu::Options     getOptionsForComboBoxPopupMenu (ComboBox &box, Label &label) override;
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
    
    
    
    
    
    
    
    
    
    
    private:
    Fader fader;
    
    ScopedPointer<Drawable> faderImage = Drawable::createFromImageData (Fader::fader_png2, Fader::fader_png2Size);
    
     Image verticalSliderKnob;
};
