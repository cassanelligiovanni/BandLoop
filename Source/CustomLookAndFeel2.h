/*
  ==============================================================================

    CustomLookAndFeel2.h
    Created: 3 Apr 2019 12:44:54pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class CustomLookAndFeel2 : public LookAndFeel_V4
{
    
public:
    CustomLookAndFeel2();
    void drawLabel(Graphics& g, Label& l)override ;
    Font getLabelFont (Label& label) override;
public:
    Font getComboBoxFont (ComboBox& ) override;
    Font getPopupMenuFont() override;
private:
    Font getCommonMenuFont();
public:
    void drawComboBox (Graphics &, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, ComboBox & box) override;
    

    
     void drawPopupMenuBackground (Graphics&, int width, int height) override;
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                                                const bool isSeparator, const bool isActive,
                                                const bool isHighlighted, const bool isTicked,
                                                const bool hasSubMenu, const String& text,
                                                const String& shortcutKeyText,
                                                const Drawable* icon, const Colour* const textColourToUse)override;
    Font getTextButtonFont (TextButton&, int buttonHeight)override;
    
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override;
    
    
     void drawComboBoxTextWhenNothingSelected (Graphics& g, ComboBox& box, Label& label) override;

     Label *     createComboBoxTextBox (ComboBox &box) override;
    
    void     positionComboBoxText (ComboBox &box, Label &label) override;
    
     PopupMenu::Options     getOptionsForComboBoxPopupMenu (ComboBox &, Label &) override;
    
    
    
    void drawToggleButton (Graphics& g, ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    void drawTickBox (Graphics& g, Component& component,
                                      float x, float y, float w, float h,
                                      const bool ticked,
                                      const bool isEnabled,
                                      const bool shouldDrawButtonAsHighlighted,
                                      const bool shouldDrawButtonAsDown) override;
    
};
