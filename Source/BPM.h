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
#include "/Users/giovanni/BandLoop/Source/Click.h"
#include "/Users/giovanni/BandLoop/Source/ADMinfo.h"

//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class BPM :
public Component,
public HighResolutionTimer,
public Timer,
public Button::Listener,
public ComboBox::Listener,
public ChangeListener
{
public:
    //==============================================================================
    BPM(const ValueTree& v,
        UndoManager& um, ADMinfo& ADM);
    ~BPM();
    
    //==============================================================================

    //==============================================================================
    /**  The paint() Method gets called when a region of a component needs redrawing,
     Never need to call this method directly
     
     @param g    the graphics context that must be used to do the drawing operations.
     */
    void paint (Graphics& g) override;
   
    /** Called when this component's size has been changed.
     */
    void resized() override;
    
    /** Start Metronome
     */
    void play();
    
    /** Stop and reset Metronome
     */
    void stop();
    
    /**
     *  As suggested by the JUCE API, I created just one instance of it
     *  \n It is used for creating flags that are send to other part of the code
        \n
     @see HighResolutionTimer
     */
    void hiResTimerCallback() override;
   
    /**
     *  A less precise time(5ms), to regularly check changes
     */
    void timerCallback() override;
    
    /** Called when a Button is Pressed.
     @param button
     *  Use this for filtering the button that is actually pressed
     * \n ex :     if (button == &recordButton) then...
     */
    void buttonClicked (Button* button) override;
    
    /** Called the inputs available from the AudioDeviceManager changes,
     \n the callback is made listening to the AMDinfo Wrapper
     @see retrieveInputs
     */
    void changeListenerCallback (ChangeBroadcaster*)override;
    
    
    /** Called when a ComboBox option is choosen (drop Down menu) .
     @param comboBoxThatHasChanged, use this for filtering the button that is actually pressed if (comboBoxThatHasChanged == selectInput) then...
     */
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    /**
     *  Load images and draw the display which keep Timing Informations
     */
    void drawBPMdisplay();
    
    /**
    *  Load images and draw Play and Stop Button
    */
    void drawButton();
    
    /**
     *  Load images and draw the array of Light-Counters
     */
    void drawCounter();
    
    /**
     *  BPM-tempo has been manually changed
     */
    void updateTempo();
   
    /** Technically check if the modulo of base 1 of the actual Bar is > 0.9990 and < 1
      * \n Pratically  check if we are close to a new Bar
     */
    bool isRightBeforeNewBar(float Bars);
    
    /** Check if we are in a a new Bar
     */
    bool isNewBar(float Bars);
    
    /** Let  know to the rest of the Code that we are close to a new Bar
     */
    void rightBeforeNewBar();
   
    /** Let it know to the rest of the Code that we are in a new Bar
     */
    void newBar();
    
    
    /** update Array of Led
     */
    void hasToBeRepainted();

    
    /** Play the Metronome.
     @param Beat
     *  check the Beat and play the metronome
     *
     */
    void playClick(int Beat);
    
    /** Metronome
     */
    Click click;
    
    /** It translates the inputs Strings from the ComboBox (ex : "1 + 2" or "1"), into
     channel references (outputL or outputR).
     */
    void retrieveOutputs(String fromComboBox);
    
    /** When the Outputs available are changed, it updates the channel references (outputL or outputR).
     * \n This is because sometimes if only output 1 and 3 are available, then output 3 is the channel (2) and not (3)
     */
    void updateOutputs();
    
    void initialiseOutputSelector();
    
    
private:
    
    ADMinfo& admInfo;
    
    Label labelHeadphonesOutputSelector;
    ComboBox headphonesOutputSelector;
    
    bool outIsStereo = true;
    int outputL = 1;
    int outputR =2;
        StringArray outputsAvailables ;
    
    ValueTree tree;
    UndoManager& undoManager;

    
    enum class PlayState
    {
        Play,
        Stop
    };
    

    std::atomic<float> timeUIfloat; //!< Time in s
    bool justTriggered = false;
    int timeInMilliseconds = 0; //!< Time in ms
    
    float BPMs = 120.;  //!< Beat per Minute
    float BPMratio = 2.; //!< Ration between actual n-bpm and 60bpm
    
    float previousBPMratio;
    float loopUnit = 2.; //!< Length in second of a Bar at the actual BPM

    float Bars = 0;
    float previousBar = 0;
    float Bar;
    float Beats = 0;
    int BarToDisplay;
    int Beat;
    int previousBeat;

    
    String BarToText = "0";
    String BeatToText = "0";
    
    Label labelBeat;
    Label labelBar;
    Label labelBPM;
    Label textBPM;
    NamedValueSet& labelBeatProperties = labelBeat.getProperties();
    NamedValueSet& labelBarProperties = labelBar.getProperties();
    NamedValueSet& labelBPMProperties = labelBPM.getProperties();
    NamedValueSet& textBPMProperties = textBPM.getProperties();
    
    PlayState playState { PlayState::Stop };
    DrawableButton DrawablePlayButton {"playButton", DrawableButton::ImageFitted };
    DrawableButton bpmDisplayButton {"bpmDisplayButton", DrawableButton::ImageStretched };
    
    
    ScopedPointer<Drawable> bpmDisplay;
    ScopedPointer<Drawable> playButtonImage;
    ScopedPointer<Drawable> stopButtonImage;
    ScopedPointer<Drawable> ON;
    ScopedPointer<Drawable> OFF;
    
    OwnedArray<DrawableButton> BpmCounters; //!<  Array of Light - Counters (4)
    

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BPM)
};
