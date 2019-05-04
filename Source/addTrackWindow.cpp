/*
  ==============================================================================

    addTrackWindow.cpp
    Created: 13 Mar 2019 11:35:30am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "addTrackWindow.h"

addTrackWindow::addTrackWindow (const String& name,
                                Colour backgroundColour,
                                int buttonsNeeded,
                                StringArray inputsAvailable,
                                const ValueTree& v,
                                UndoManager& um)
:
DocumentWindow (name, backgroundColour, buttonsNeeded),
Thread ("ThreadWithProgressWindow"),
progress (0.0),
wasCancelledByUser (false),

tree (v), undoManager (um)
    {
    
        setContentOwned (new AddTrackComponent(name, backgroundColour, buttonsNeeded, inputsAvailable, tree, undoManager, toDestroy), false);

        launchThread(5);
        
     
    }
    
  addTrackWindow::~addTrackWindow()
    {

        stopThread(200);
    }

void addTrackWindow::run(){

    
    

    
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
    void addTrackWindow::closeButtonPressed()
    {
        stopThread(500);
        stopTimer();
        wait(100);
        delete this;
        
    }

    void addTrackWindow::changeListenerCallback (ChangeBroadcaster* source)
    {
        
    }

void addTrackWindow::launchThread (int priority)
{
    
    startThread (priority);
    startTimer (100);
   

}


void addTrackWindow::timerCallback()
{
    if (toDestroy == true)
        closeButtonPressed();

}

void addTrackWindow::labelTextChanged (Label* labelThatHasChanged) {
  

};




void addTrackWindow::comboBoxChanged(ComboBox* comboBoxThatHasChanged){
    
    
}



void addTrackWindow::valueTreeChildAdded (ValueTree& parentTree, ValueTree&) {};
void addTrackWindow::valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) {};
void addTrackWindow::valueTreeChildOrderChanged (ValueTree& parentTree, int, int) {};
void addTrackWindow::valueTreeParentChanged (ValueTree&) {};
void addTrackWindow::valueTreePropertyChanged (ValueTree&, const Identifier&) {};
