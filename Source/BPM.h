/*
  ==============================================================================

    BPM.h
    Created: 17 Mar 2019 3:41:01pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class BPM :
public Component,
public HighResolutionTimer,
public Timer,
public Button::Listener
{
public:
    //==============================================================================
    BPM(const ValueTree& v,
        UndoManager& um);
    ~BPM();
    
    //==============================================================================

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    void play();
    void stop();
    void hiResTimerCallback() override;
    void timerCallback() override;
    void buttonClicked (Button* button) override;
    
private:
    
    ValueTree tree;
    UndoManager& undoManager;

    
    enum class PlayState
    {
        Play,
        Stop
    };
    
    int previousBeat;
    float previousBar = 0;
    float startBar = 0;
    
    float loopUnit;
    int loopLenght;
    
    std::atomic<float> timeUIfloat;
    
    String timeUI { "" };
    int timeInMilliseconds = 0;
    
    float Bars = 0;
    int BarToDisplay;
    float Bar;
    float Beats = 0;
    int Beat;
    
    String BarToText = "0";
    String BeatToText = "0";
    
    Label labelBeat;
     NamedValueSet& labelBeatProperties = labelBeat.getProperties();
    Label labelBar;
      NamedValueSet& labelBarProperties = labelBar.getProperties();
    
    int BPMs = 120;
    float BPMratio = 0.;
    float previousBPMratio;
    
    Label labelBPM;
    NamedValueSet& labelBPMProperties = labelBPM.getProperties();
    Label textBPM;
    NamedValueSet& textBPMProperties = textBPM.getProperties();
    
    PlayState playState { PlayState::Stop };
    DrawableButton DrawablePlayButton {"playButton", DrawableButton::ImageFitted };
    DrawableButton bpmDisplayButton {"bpmDisplayButton", DrawableButton::ImageStretched };
    
    FlexBox counterFlexbox { FlexBox::Direction::row, FlexBox::Wrap::noWrap, FlexBox::AlignContent::center, FlexBox::AlignItems::center, FlexBox::JustifyContent::center };
    
    ScopedPointer<Drawable> bpmDisplay = Drawable::createFromImageData (BinaryData::BPMDisplay_svg, BinaryData::BPMDisplay_svgSize);
    
   ScopedPointer<Drawable> playButtonImage = Drawable::createFromImageData (BinaryData::PlayButton_svg, BinaryData::PlayButton_svgSize);
    
   ScopedPointer<Drawable> stopButtonImage = Drawable::createFromImageData (BinaryData::StopButton_svg, BinaryData::StopButton_svgSize);
   
    ScopedPointer<Drawable> ON = Drawable::createFromImageData (BinaryData::BPMCounterON_svg, BinaryData::BPMCounterON_svgSize);
    
    ScopedPointer<Drawable> OFF = Drawable::createFromImageData (BinaryData::BPMCounterOFF_svg, BinaryData::BPMCounterOFF_svgSize);
    
    OwnedArray<DrawableButton> BpmCounters;
    

    
    
//    play = Drawable::createFromImageData (BinaryData::PedalShape3_svg, BinaryData::PedalShape3_svgSize);
//    stop = Drawable::createFromImageData (BinaryData::PedalShape3_svg, BinaryData::PedalShape3_svgSize);

    TextButton playButton { "Stopped" };
    
    bool justTriggered = false;

//    std::atomic<int> transportBar ;
//    std::atomic<int> transportBeat ;
//    std::atomic<bool> transportUpToDate ;
    
//    AudioDeviceSelectorComponent audioSetupComp;
    
    
    //==============================================================================
    // Your private member variables go here...
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BPM)
};
