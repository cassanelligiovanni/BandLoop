/*
  ==============================================================================

    CustomLookAndFeel2.cpp
    Created: 3 Apr 2019 12:44:54pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "CustomLookAndFeel2.h"

CustomLookAndFeel2::CustomLookAndFeel2(){
    setColour(TextButton::buttonColourId, Colours::BandLoopText);
    setColour(TextButton::textColourOffId, Colours::BandLoopBackground);
    setColour(ListBox::backgroundColourId,                Colours::BandLoopBackground);
              setColour(ListBox::outlineColourId,                   Colours::BandLoopText);
              setColour(ListBox::textColourId,                       Colours::BandLoopText);
}

  void CustomLookAndFeel2::drawLabel(Graphics& g, Label& label)
    {
        
        g.fillAll (label.findColour (Label::backgroundColourId));
        
        if (! label.isBeingEdited())
        {
            auto alpha = label.isEnabled() ? 1.0f : 0.5f;
            const Font font (getLabelFont (label));
            
            g.setColour (label.findColour (Label::textColourId).withMultipliedAlpha (alpha));
            g.setFont (font);
            
            auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());
            
            g.drawFittedText (label.getText(), textArea, label.getJustificationType(),
                              jmax (1, (int) (textArea.getHeight() / font.getHeight())),
                              label.getMinimumHorizontalScale());
            
            g.setColour (label.findColour (Label::outlineColourId).withMultipliedAlpha (alpha));
        }
        else if (label.isEnabled())
        {
            g.setColour (label.findColour (Label::outlineColourId));
        }
        
        g.drawRect (label.getLocalBounds());
        
        
//        g.setColour(Colours::BandLoopText);
//        g.fillRoundedRectangle(
//            0,              // x
//            0,              // y
//            label.getWidth(),   // width
//            label.getHeight(),  // height
//            8             // cornerSize
//        );
//        g.fillAll (label.findColour (Label::backgroundColourId));
        
//        if (! label.isBeingEdited())
//        {
//            auto alpha = label.isEnabled() ? 1.0f : 0.5f;
//            const Font font (getLabelFont (label));
//
//            g.setColour (Colours::BandLoopText.withMultipliedAlpha (alpha));
//
//            g.setFont (Font ("Falling Sky", "Regular", 12.f));
//
//            auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());
//
//            g.drawFittedText (label.getText(), textArea, label.getJustificationType(),
//                              jmax (1, (int) (textArea.getHeight() / font.getHeight())),
//                              label.getMinimumHorizontalScale());
//
//            g.setColour (Colours::BandLoopBackground.withMultipliedAlpha (alpha));
//        }
//        else if (label.isEnabled())
//        {
//
//
//        }
        
    }


Font CustomLookAndFeel2::getLabelFont (Label& label)
{
    const NamedValueSet& properties = label.getProperties();
    String typefaceName = properties.getWithDefault ("fontTypefaceName", "Falling Sky");
    int fontSize = properties.getWithDefault ("fontSize", 12);
    int styleFlag = properties.getWithDefault ("fontStyleFlag", Font::plain);
    return Font (typefaceName, fontSize, styleFlag);
}



void CustomLookAndFeel2::drawComboBox (Graphics& g, int width, int height, bool,
                                   int, int, int, int, ComboBox& box)
{
    auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
    Rectangle<int> boxBounds (0, 0, width, height);
    
    g.setColour (Colours::BandLoopText);
    g.fillRoundedRectangle (boxBounds.toFloat(), 8);
    
    g.setColour (Colours::BandLoopBackground);
    g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), 8, 1.0f);
    
    Rectangle<int> arrowZone (width - 30, 0, 20, height);
    Path path;
    path.startNewSubPath (arrowZone.getX() + 3.0f, arrowZone.getCentreY() - 2.0f);
    path.lineTo (static_cast<float> (arrowZone.getCentreX()), arrowZone.getCentreY() + 3.0f);
    path.lineTo (arrowZone.getRight() - 3.0f, arrowZone.getCentreY() - 2.0f);
    
    g.setColour (Colours::BandLoopBackground.withAlpha ((box.isEnabled() ? 0.9f : 0.2f)));
    g.strokePath (path, PathStrokeType (2.0f));
}

void CustomLookAndFeel2::drawPopupMenuBackground (Graphics& g, int width, int height){
    auto background = Colours::BandLoopText;
    
    g.fillAll (background);
    g.setColour (background.overlaidWith (Colours::BandLoopText));
    
    for (int i = 0; i < height; i += 3)
        g.fillRect (0, i, width, 1);
    
#if ! JUCE_MAC
    g.setColour (Colours::BandLoopText.withAlpha (0.6f));
    g.drawRect (0, 0, width, height);
#endif
}

void CustomLookAndFeel2::drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                                        const bool isSeparator, const bool isActive,
                                        const bool isHighlighted, const bool isTicked,
                                        const bool hasSubMenu, const String& text,
                                        const String& shortcutKeyText,
                                        const Drawable* icon, const Colour* const textColourToUse)
{
    if (isSeparator)
    {
        auto r = area.reduced (5, 0);
        r.removeFromTop (r.getHeight() / 2 - 1);
        
        g.setColour (Colours::BandLoopBackground);
        g.fillRect (r.removeFromTop (1));
        
        g.setColour (Colours::BandLoopBackground);
        g.fillRect (r.removeFromTop (1));
    }
    else
    {
        auto textColour = Colours::BandLoopBackground;
        
//        if (textColourToUse != nullptr)
//            textColour = *textColourToUse;
        
        auto r = area.reduced (1);
        
        if (isHighlighted)
        {
            g.setColour (Colours::BandLoopText.darker(0.1));
            g.fillRect (r);
            
            g.setColour (Colours::BandLoopBackground);
        }
        else
        {
            g.setColour (Colours::BandLoopBackground);
        }
        
        if (! isActive)
            g.setOpacity (0.3f);
        
        Font font (getPopupMenuFont());
        
        auto maxFontHeight = area.getHeight() / 1.3f;
        
        if (font.getHeight() > maxFontHeight)
            font.setHeight (maxFontHeight);
        
        g.setFont (font);
        
        auto iconArea = r.removeFromLeft ((r.getHeight() * 5) / 4).reduced (3).toFloat();
        
        if (icon != nullptr)
        {
            icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
        }
        else if (isTicked)
        {
            auto tick = getTickShape (1.0f);
            g.fillPath (tick, tick.getTransformToScaleToFit (iconArea, true));
        }
        
        if (hasSubMenu)
        {
            auto arrowH = 0.6f * getPopupMenuFont().getAscent();
            
            auto x = (float) r.removeFromRight ((int) arrowH).getX();
            auto halfH = (float) r.getCentreY();
            
            Path p;
            p.addTriangle (x, halfH - arrowH * 0.5f,
                           x, halfH + arrowH * 0.5f,
                           x + arrowH * 0.6f, halfH);
            
            g.fillPath (p);
        }
        
        r.removeFromRight (3);
        g.drawFittedText (text, r, Justification::centredLeft, 1);
        
        if (shortcutKeyText.isNotEmpty())
        {
            Font f2 (font);
            f2.setHeight (f2.getHeight() * 0.75f);
            f2.setHorizontalScale (0.95f);
            g.setFont (f2);
            
            g.drawText (shortcutKeyText, r, Justification::centredRight, true);
        }
    }
}

void CustomLookAndFeel2::drawComboBoxTextWhenNothingSelected (Graphics& g, ComboBox& box, Label& label)
{
    g.setColour (Colours::BandLoopBackground.withMultipliedAlpha (0.5f));
    
    auto font = label.getLookAndFeel().getLabelFont (label);
    
    g.setFont (font);
    
    auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());
    
    g.drawFittedText (box.getTextWhenNothingSelected(), textArea, label.getJustificationType(),
                      jmax (1, (int) (textArea.getHeight() / font.getHeight())),
                      label.getMinimumHorizontalScale());
}



Label *     CustomLookAndFeel2::createComboBoxTextBox (ComboBox &box) {return new Label (String(), String());};

void    CustomLookAndFeel2::positionComboBoxText (ComboBox &box, Label &label) {
    label.setBounds (1, 1,box.getWidth() + 3 - box.getHeight(),box.getHeight() - 2);
    label.setFont (Font ("Falling Sky", "Regular", 12.f));
    label.setColour(Label::textColourId, Colours::BandLoopBackground);
};


PopupMenu::Options     CustomLookAndFeel2::getOptionsForComboBoxPopupMenu (ComboBox &box, Label &label) {
    return PopupMenu::Options().withTargetComponent (&box)
    .withItemThatMustBeVisible (box.getSelectedId())
    .withMinimumWidth (box.getWidth())
    .withMaximumNumColumns (1)
    .withStandardItemHeight (label.getHeight());
};


Font CustomLookAndFeel2::getComboBoxFont (ComboBox& box)
{
    return getCommonMenuFont();
}

Font CustomLookAndFeel2::getPopupMenuFont()
{
    return getCommonMenuFont();
}



Font CustomLookAndFeel2::getCommonMenuFont()
{
    return Font ("Falling Sky", "Regular", 12.f);
}

Font CustomLookAndFeel2::getTextButtonFont (TextButton&, int buttonHeight)
{
    return Font ("Falling Sky", "Regular", 12.f);
}


void CustomLookAndFeel2::drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                                              bool isMouseOverButton, bool isButtonDown)
{
    auto baseColour = backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
    .withMultipliedAlpha      (button.isEnabled() ? 0.9f : 0.5f);
    
    if (isButtonDown || isMouseOverButton)
        baseColour = baseColour.contrasting (isButtonDown ? 0.2f : 0.1f);
    
    auto flatOnLeft   = button.isConnectedOnLeft();
    auto flatOnRight  = button.isConnectedOnRight();
    auto flatOnTop    = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();
    
    auto width  = button.getWidth()  - 1.0f;
    auto height = button.getHeight() - 1.0f;
    
    if (width > 0 && height > 0)
    {
        auto cornerSize = jmin (15.0f, jmin (width, height) * 0.45f);
        auto lineThickness = cornerSize    * 0.1f;
        auto halfThickness = lineThickness * 0.5f;
        
        Path outline;
        outline.addRoundedRectangle (0.5f + halfThickness, 0.5f + halfThickness, width - lineThickness, height - lineThickness,
                                     cornerSize, cornerSize,
                                     ! (flatOnLeft  || flatOnTop),
                                     ! (flatOnRight || flatOnTop),
                                     ! (flatOnLeft  || flatOnBottom),
                                     ! (flatOnRight || flatOnBottom));
        
        auto outlineColour = button.findColour (button.getToggleState() ? TextButton::textColourOnId
                                                : TextButton::textColourOffId);
        
        g.setColour (baseColour);
        g.fillPath (outline);
        
        if (! button.getToggleState())
        {
            g.setColour (outlineColour);
            g.strokePath (outline, PathStrokeType (lineThickness));
        }
    }
}




void CustomLookAndFeel2::drawToggleButton (Graphics& g, ToggleButton& button,
                                       bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto fontSize = jmin (15.0f, button.getHeight() * 0.75f);
    auto tickWidth = fontSize * 1.1f;
    
    drawTickBox (g, button, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
                 tickWidth, tickWidth,
                 button.getToggleState(),
                 button.isEnabled(),
                 shouldDrawButtonAsHighlighted,
                 shouldDrawButtonAsDown);
    
    g.setColour (button.findColour (ToggleButton::textColourId));
    g.setFont (fontSize);
    
    if (! button.isEnabled())
        g.setOpacity (0.5f);
    
    g.drawFittedText (button.getButtonText(),
                      button.getLocalBounds().withTrimmedLeft (roundToInt (tickWidth) + 10)
                      .withTrimmedRight (2),
                      Justification::centredLeft, 10);
}

void CustomLookAndFeel2::drawTickBox (Graphics& g, Component& component,
                                  float x, float y, float w, float h,
                                  const bool ticked,
                                  const bool isEnabled,
                                  const bool shouldDrawButtonAsHighlighted,
                                  const bool shouldDrawButtonAsDown)
{
    ignoreUnused (isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
    
    Rectangle<float> tickBounds (x, y, w, h);
    
    g.setColour (Colours::BandLoopText);
    g.drawRoundedRectangle (tickBounds, 4.0f, 1.0f);
    
    if (ticked)
    {
        g.setColour (Colours::BandLoopText);
        auto tick = getTickShape (0.75f);
        g.fillPath (tick, tick.getTransformToScaleToFit (tickBounds.reduced (4, 5).toFloat(), false));
    }
}
