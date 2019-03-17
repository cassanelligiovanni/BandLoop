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
    
    float previousBar = 0;
    float startBar = 0;
    
    float loopUnit;
    int loopLenght;
    
    std::atomic<float> timeUIfloat;
    
    String timeUI { "" };
    int timeInMilliseconds = 0;
    
    float Bars = 0;
    int Bar;
    float Beats = 0;
    int Beat;
    
    String BarToText = "0";
    String BeatToText = "0";
    
    int BPMs = 120;
    float BPMratio = 0.;
    
    Label labelBPM;
    Label textBPM;
    
    PlayState playState { PlayState::Stop };
    TextButton playButton { "Stopped" };
    
//    AudioDeviceSelectorComponent audioSetupComp;
    
    
    //==============================================================================
    // Your private member variables go here...
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BPM)
};
