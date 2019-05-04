/*
  ==============================================================================

    LightPressableButton.h
    Created: 21 Apr 2019 10:48:34am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

//==============================================================================
/**
 A derived class of juce::Button.
 
 \n It is both a Button and LED Indicator
 
 \n It allow the user to start and stop the recordings
 
 \n and gives feedback on the  process being made by the Track
 
 */


#include "../JuceLibraryCode/JuceHeader.h"
#include "/Users/giovanni/BandLoop/Source/RecordState.h"
#include "/Users/giovanni/BandLoop/Source/GradientLabel.h"
#include "/Users/giovanni/BandLoop/Source/CustomLookAndFeel2.h"


class LightPressableButton : public Component,
                             public Timer
{
public:
    
    /**
     *Creates a slider with Decibel range.
     */
    LightPressableButton(RecordState& recordStateR, bool& isStartingRecordingR, bool& isStoppingRecordingR, int& pedalNumber);
    
    ~LightPressableButton();
    
    void timerCallback() override;
    
    void paint(Graphics& g) override;
    
    void resized() override;
    
    void barRecordingOn();
    
    void barRecordingBlinking() ;
    
    void barPlayingOn();
    
    void barPlayingBlinking();
    
    void messageToPedal();
//    TextButton recordButton {"AllButton"};
    
    void updateTrue();
    
    
private:
    
    int& nPedal;
    
    Rectangle<int> labelBounds ;
//    GradientLabel frame;
    
    DrawableButton recordImage {"RecordButton", DrawableButton::ImageStretched };
    
    RecordState& recordState ;
    bool& isStartingRecording;
    bool& isStoppingRecording;
    
    //flags for the Bar blinking
    int alternateFlag = 0;
    int flagRecOn = true ;
    int flagRecOff = false;
    int flagPlayOn = true;
    int flagPlayOff = false;
    
    ScopedPointer<Drawable> barPlayingOnImage = Drawable::createFromImageData (BinaryData::BarPlayingOn_svg, BinaryData::BarPlayingOn_svgSize);
    
    ScopedPointer<Drawable> barPlayingOffImage = Drawable::createFromImageData (BinaryData::BarPlayingOff_svg, BinaryData::BarPlayingOff_svgSize);
    
    ScopedPointer<Drawable> barRecordingOnImage = Drawable::createFromImageData (BinaryData::BarRecordingOn_svg, BinaryData::BarRecordingOn_svgSize);
    
    ScopedPointer<Drawable> barRecordingOffImage = Drawable::createFromImageData (BinaryData::BarRecordingOff_svg, BinaryData::BarRecordingOff_svgSize);
    
    
    ScopedPointer<Drawable> deleteTrackImage = Drawable::createFromImageData (BinaryData::DeleteTrack_svg, BinaryData::DeleteTrack_svgSize);
    
    DrawableButton deleteTrackButton{"deleteTrackButton", DrawableButton::ImageFitted };
    
    DrawableButton DrawablePlayingBarOn {"BarPlayingLightOn", DrawableButton::ImageFitted };
    DrawableButton DrawablePlayingBarOff {"BarPlayingLightOff", DrawableButton::ImageFitted };
    DrawableButton DrawableRecordingBarOn {"BarRecordingLightOn", DrawableButton::ImageFitted };
    DrawableButton DrawableRecordingBarOff {"BarRecordingLightOff", DrawableButton::ImageFitted };
    
    int type;
    bool updateGui = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LightPressableButton)
};
