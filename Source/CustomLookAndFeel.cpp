/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 29 Mar 2019 5:24:38pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "CustomLookAndFeel.h"


CustomLookAndFeel::CustomLookAndFeel(){
    
    
    Image verticalSliderKnob = ImageCache::getFromMemory (Fader::fader_png2,
                                                          Fader::fader_png2Size);
    
    setColour(TextButton::buttonColourId, Colours::BandLoopBackground);
    setColour(TextButton::textColourOffId, Colours::BandLoopText);
    setColour(ListBox::backgroundColourId,                Colours::BandLoopBackground);
    setColour(ListBox::outlineColourId,                   Colours::BandLoopBackground);
    setColour(ListBox::textColourId,                       Colours::BandLoopText);

}



void CustomLookAndFeel::drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                           bool isMouseOverButton, bool isButtonDown)
{
    auto baseColour =Colours::BandLoopBackground;
    
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



void CustomLookAndFeel::drawLinearSliderThumb(Graphics& g,
                           int x, int y,
                           int width, int height,
                           float sliderPos,
                           float minSliderPos,
                           float maxSliderPos,
                           const Slider::SliderStyle style,
                           Slider& slider)
{
    
//        g.setOpacity(1.0);
//        int centerX = x + width /2;
//        int centerY = sliderPos;
//
//        g.drawImageAt(verticalSliderKnob, centerX - verticalSliderKnob.getWidth()/2,
//                      centerY - verticalSliderKnob.getHeight()/2, false);
    }
//    else
//    {
//        // Just call the base class for the demo
//        LookAndFeel_V2::drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
//    }
//}
//
void CustomLookAndFeel::drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                       float sliderPos, float minSliderPos, float maxSliderPos,
                       const Slider::SliderStyle style, Slider& slider)
{
     drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
   
    if (slider.isBar())
    {
        g.setColour (slider.findColour (Slider::trackColourId));
        g.fillRect (slider.isHorizontal() ? Rectangle<float> (static_cast<float> (x), y + 0.5f, sliderPos - x, height - 1.0f)
                    : Rectangle<float> (x + 0.5f, sliderPos, width - 1.0f, y + (height - sliderPos)));
    }
    else
    {
        auto isTwoVal   = (style == Slider::SliderStyle::TwoValueVertical   || style == Slider::SliderStyle::TwoValueHorizontal);
        auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);
        
        auto trackWidth = jmin (6.0f, slider.isHorizontal() ? height * 0.25f : width * 0.25f);
        
        Point<float> startPoint (slider.isHorizontal() ? x : x + width * 0.5f,
                                 slider.isHorizontal() ? y + height * 0.5f : height + y);
        
        Point<float> endPoint (slider.isHorizontal() ? width + x : startPoint.x,
                               slider.isHorizontal() ? startPoint.y : y);
        
        Path backgroundTrack;
        backgroundTrack.startNewSubPath (startPoint);
        backgroundTrack.lineTo (endPoint);
        g.setColour (slider.findColour (Slider::backgroundColourId));
        g.strokePath (backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });
        
        Path valueTrack;
        Point<float> minPoint, maxPoint, thumbPoint;
        
        if (isTwoVal || isThreeVal)
        {
            minPoint = { slider.isHorizontal() ? minSliderPos : width * 0.5f,
                slider.isHorizontal() ? height * 0.5f : minSliderPos };
            
            if (isThreeVal)
                thumbPoint = { slider.isHorizontal() ? sliderPos : width * 0.5f,
                    slider.isHorizontal() ? height * 0.5f : sliderPos };
            
            maxPoint = { slider.isHorizontal() ? maxSliderPos : width * 0.5f,
                slider.isHorizontal() ? height * 0.5f : maxSliderPos };
        }
        else
        {
            auto kx = slider.isHorizontal() ? sliderPos : (x + width * 0.5f);
            auto ky = slider.isHorizontal() ? (y + height * 0.5f) : sliderPos;
            
            if (ky > y + height - 30)
                ky = y + height - 30;
            
            if (ky < y + 2)
                ky = y + 2;
            
            minPoint = startPoint;
            maxPoint = { kx, ky };
        }
        
        auto thumbWidth = getSliderThumbRadius (slider);
        
        valueTrack.startNewSubPath (minPoint);
        valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
        g.setColour (slider.findColour (Slider::trackColourId));
        g.strokePath (valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });
        
        if (! isTwoVal)
        {
            
            Rectangle<float> faderBounds (Rectangle<float> (static_cast<float> (20), static_cast<float> (60)).withCentre (maxPoint.translated(0, 20)));
            
            faderImage->drawWithin(g, faderBounds, 1, 1);
    
        }
        
        if (isTwoVal || isThreeVal)
        {
            auto sr = jmin (trackWidth, (slider.isHorizontal() ? height : width) * 0.4f);
            auto pointerColour = slider.findColour (Slider::thumbColourId);
            
            if (slider.isHorizontal())
            {
                drawPointer (g, minSliderPos - sr,
                             jmax (0.0f, y + height * 0.5f - trackWidth * 2.0f),
                             trackWidth * 2.0f, pointerColour, 2);
                
                drawPointer (g, maxSliderPos - trackWidth,
                             jmin (y + height - trackWidth * 2.0f, y + height * 0.5f),
                             trackWidth * 2.0f, pointerColour, 4);
            }
            else
            {
                drawPointer (g, jmax (0.0f, x + width * 0.5f - trackWidth * 2.0f),
                             minSliderPos - trackWidth,
                             trackWidth * 2.0f, pointerColour, 1);
                
                drawPointer (g, jmin (x + width - trackWidth * 2.0f, x + width * 0.5f), maxSliderPos - sr,
                             trackWidth * 2.0f, pointerColour, 3);
            
            }
        }
    }
    
   

}
//
void CustomLookAndFeel::drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
                                 float /*sliderPos*/,
                                 float /*minSliderPos*/,
                                 float /*maxSliderPos*/,
                                 const Slider::SliderStyle /*style*/, Slider& slider)
{
    
    const float infinity = -30.f;
    
        Rectangle<int> bounds(x, y, width, height);
    
        const float h = (bounds.getHeight() - 2.0) * 0.05;
        g.setFont (h * 0.8f);
        for (int i=2; i<20; ++i) {
            const float y = bounds.getY() + i * h;
            if (i % 2 == 0) {
                g.drawHorizontalLine (y + 1,
                                      bounds.getX() + 4,
                                      bounds.getRight());
                if (i < 20) {
//                    g.drawFittedText (juce::String (i * 0.05 * infinity),
//                                      bounds.getX(), y + 4, bounds.getWidth(), h * 0.6,
//                                      juce::Justification::topRight, 1);
                }
            }
            else {
                g.drawHorizontalLine (y + 2,
                                      bounds.getWidth()/4 ,
                                      bounds.getWidth()/4*3);
            }
        }
    
}
//
//void CustomLookAndFeel::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
//                       float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
//{
//    auto radius = jmin (width / 2, height / 2) - 2.0f;
//    auto centreX = x + width  * 0.5f;
//    auto centreY = y + height * 0.5f;
//    auto rx = centreX - radius;
//    auto ry = centreY - radius;
//    auto rw = radius * 2.0f;
//    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
//    auto isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
//    
//    if (slider.isEnabled())
//        g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
//    else
//        g.setColour (Colour (0x80808080));
//    
//    {
//        Path filledArc;
//        filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, angle, 0.0);
//        g.fillPath (filledArc);
//    }
//    
//    {
//        auto lineThickness = jmin (15.0f, jmin (width, height) * 0.45f) * 0.1f;
//        Path outlineArc;
//        outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 0.0);
//        g.strokePath (outlineArc, PathStrokeType (lineThickness));
//    }
//    
//}


Font CustomLookAndFeel::getLabelFont (Label& label)
{
    const NamedValueSet& properties = label.getProperties();
    String typefaceName = properties.getWithDefault ("fontTypefaceName", "Falling Sky");
    int fontSize = properties.getWithDefault ("fontSize", 16);
    int styleFlag = properties.getWithDefault ("fontStyleFlag", Font::plain);
    return Font (typefaceName, fontSize, styleFlag);
}



void CustomLookAndFeel::drawPopupMenuBackground (Graphics& g, int width, int height){
    auto background = Colours::BandLoopBackground;
    
    g.fillAll (background);
    g.setColour (background.overlaidWith (Colours::BandLoopBackground));
    
    for (int i = 0; i < height; i += 3)
        g.fillRect (0, i, width, 1);
    
#if ! JUCE_MAC
    g.setColour (Colours::BandLoopBackground.withAlpha (0.6f));
    g.drawRect (0, 0, width, height);
#endif
}

void CustomLookAndFeel::drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
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
        
        g.setColour (Colours::BandLoopText);
        g.fillRect (r.removeFromTop (1));
        
        g.setColour (Colours::BandLoopText);
        g.fillRect (r.removeFromTop (1));
    }
    else
    {
        auto textColour = Colours::BandLoopText;
        
        //        if (textColourToUse != nullptr)
        //            textColour = *textColourToUse;
        
        auto r = area.reduced (1);
        
        if (isHighlighted)
        {
            g.setColour (Colours::BandLoopBackground.darker(0.1));
            g.fillRect (r);
            
            g.setColour (Colours::BandLoopText);
        }
        else
        {
            g.setColour (Colours::BandLoopText);
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

void CustomLookAndFeel::drawComboBoxTextWhenNothingSelected (Graphics& g, ComboBox& box, Label& label)
{
    g.setColour (Colours::BandLoopText.withMultipliedAlpha (0.5f));
    
    auto font = label.getLookAndFeel().getLabelFont (label);
    
    g.setFont (font);
    
    auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());
    
    g.drawFittedText (box.getTextWhenNothingSelected(), textArea, label.getJustificationType(),
                      jmax (1, (int) (textArea.getHeight() / font.getHeight())),
                      label.getMinimumHorizontalScale());
}



Label *     CustomLookAndFeel::createComboBoxTextBox (ComboBox &box) {return new Label (String(), String());};

void    CustomLookAndFeel::positionComboBoxText (ComboBox &box, Label &label) {
    label.setBounds (1, 1,box.getWidth() + 3 - box.getHeight(),box.getHeight() - 2);
    label.setFont (Font ("Falling Sky", "Regular", 12.f));
    label.setColour(Label::textColourId, Colours::BandLoopText);
};


PopupMenu::Options     CustomLookAndFeel::getOptionsForComboBoxPopupMenu (ComboBox &box, Label &label) {
    return PopupMenu::Options().withTargetComponent (&box)
    .withItemThatMustBeVisible (box.getSelectedId())
    .withMinimumWidth (box.getWidth())
    .withMaximumNumColumns (1)
    .withStandardItemHeight (label.getHeight());
};


Font CustomLookAndFeel::getTextButtonFont (TextButton& box, int buttonHeight)
{
    const NamedValueSet& properties = box.getProperties();
    String typefaceName = properties.getWithDefault ("fontTypefaceName", "Falling Sky");
    int fontSize = properties.getWithDefault ("fontSize", 12);
    int styleFlag = properties.getWithDefault ("fontStyleFlag", Font::plain);
    
    return Font (typefaceName, fontSize, styleFlag);
}
