/*
  ==============================================================================

    SettingWindow.h
    Created: 19 Mar 2019 4:32:30pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/


#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


class SettingWindow   : public DocumentWindow,
public Thread,
public Label::Listener,
public ValueTree::Listener,
private ComboBox::Listener,
private Timer,
private ChangeListener
{
public:
    SettingWindow (const String& name,
                   Colour backgroundColour,
                   int buttonsNeeded,
                   const ValueTree& v,
                   UndoManager& um, AudioDeviceManager& deviceManager);
    
    ~SettingWindow() ;
    
    void run() override;
    
    void closeButtonPressed() override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override;
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override;
    void valueTreeParentChanged (ValueTree&) override;
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    
    void labelTextChanged (Label *labelThatHasChanged) override;
    
    void launchThread (int priority = 5);
    
    
private:
    //==============================================================================
    
    AudioDeviceSelectorComponent audioSetupComp;
    
    ValueTree tree;
    UndoManager& undoManager;
    
    void timerCallback() override;
    double progress;
    
    String message;
    CriticalSection messageLock;
    const int timeOutMsWhenCancelling = 1000;
    bool wasCancelledByUser;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingWindow)
};
