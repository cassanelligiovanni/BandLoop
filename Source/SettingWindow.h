/*
  ==============================================================================

    SettingWindow.h
    Created: 19 Mar 2019 4:32:30pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/


#pragma once
#include "../Source/SettingWindowComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/CustomLookAndFeel2.h"


class SettingWindow   : public DocumentWindow,
public Thread


{
public:
    SettingWindow (const String& name,
                   Colour backgroundColour,
                   int buttonsNeeded,
                   const ValueTree& v,
                   UndoManager& um, AudioDeviceManager& deviceManager, Array<int>& pedalsAvailable, MidiMessageCollector& midiMessageCollector);
    
    ~SettingWindow() ;
    
    void run() override;
    
    void closeButtonPressed() override;

    void launchThread (int priority = 5);
    
    void pairing();
    
    void numberOfPedals ();
    
    void pedalClicked(int pedalNumber);
    
    MidiMessageCollector& midiCollector ;
    Array<int>& pedalsAvailables ;
    
private:
    //==============================================================================
    
    AudioDeviceSelectorComponent audioSetupComp;
    
    TextButton pairingButton {"Pairing Button"};
//    MidiOutput midiOutput;
    
    ValueTree tree;
    UndoManager& undoManager;
    
    double progress;
    
    String message;
    CriticalSection messageLock;
    const int timeOutMsWhenCancelling = 1000;
    bool wasCancelledByUser;

    CustomLookAndFeel2  customLookAndFeel2;
    
    ScopedPointer<Drawable> pedalImage = Drawable::createFromImageData (BinaryData::pedal_svg, BinaryData::pedal_svgSize);
    ScopedPointer<Drawable> pedalImagePressed = Drawable::createFromImageData (BinaryData::pedal_svg, BinaryData::pedal_svgSize);

    
    OwnedArray<DrawableButton> pedalButtons;
    
    SettingWindowComponent settingWindow;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingWindow)
};
