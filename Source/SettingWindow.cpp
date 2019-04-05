/*
  ==============================================================================

    SettingWindow.cpp
    Created: 19 Mar 2019 4:32:30pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "SettingWindow.h"


SettingWindow::SettingWindow (const String& name,
                                Colour backgroundColour,
                                int buttonsNeeded,
                                const ValueTree& v,
                                UndoManager& um, AudioDeviceManager& deviceManager)
:
DocumentWindow (name, backgroundColour, buttonsNeeded),
Thread ("ThreadWithProgressWindow"),
progress (0.0),
wasCancelledByUser (false), audioSetupComp (deviceManager,  0,  256,  0, 256,  true,  false,  false, false),

tree (v), undoManager (um)
{
    deviceManager.addChangeListener (this);
    tree.addListener(this);
    Component::addAndMakeVisible(audioSetupComp);
    launchThread(5);
    audioSetupComp.setBounds (0, 0, 400, 200);
    
}

SettingWindow::~SettingWindow()
{
    
    stopThread(200);
    
    
}

void SettingWindow::run(){
    
    while (! threadShouldExit())
    {
        wait (100);
        
        // because this is a background thread, we mustn't do any UI work without
        // first grabbing a MessageManagerLock..
        const MessageManagerLock mml (Thread::getCurrentThread());
        
        if (! mml.lockWasGained())  // if something is trying to kill this job, the lock
            return;                 // will fail, in which case we'd better return..
        
    }
    
    
}
void SettingWindow::closeButtonPressed()
{
    stopThread(500);
    stopTimer();
    wait(100);
    delete this;
    
}

void SettingWindow::changeListenerCallback (ChangeBroadcaster* source)
{
    
}

void SettingWindow::launchThread (int priority)
{
    
    startThread (priority);
    startTimer (100);
    
    
}


void SettingWindow::timerCallback()
{
    
    
}

void SettingWindow::labelTextChanged (Label* labelThatHasChanged) {

};




void SettingWindow::comboBoxChanged(ComboBox* comboBoxThatHasChanged){
    
}



void SettingWindow::valueTreeChildAdded (ValueTree& parentTree, ValueTree&) {};
void SettingWindow::valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) {};
void SettingWindow::valueTreeChildOrderChanged (ValueTree& parentTree, int, int) {};
void SettingWindow::valueTreeParentChanged (ValueTree&) {};
void SettingWindow::valueTreePropertyChanged (ValueTree&, const Identifier&) {};


