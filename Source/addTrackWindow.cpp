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
        tree.addListener(this);
        nameTrack.addListener(this);
        
        juce::Component::addAndMakeVisible(inputs);
        juce::Component::addAndMakeVisible(nameTrack);
        juce::Component::addAndMakeVisible(inputsID);
        juce::Component::addAndMakeVisible(nameTrackID);
        
        for (int i = 0; i < inputsAvailable.size(); i++){
            inputs.addItem (inputsAvailable[i], i+1);
 
        }
        
        nameTrack.setEditable(true);
//        nameTrack.setJustificationType(Justification::centred);
        nameTrack.setBounds(100, 25, 300, 25);
        
//        inputs.setJustificationType(Justification::centred);
        inputs.setBounds(100, 50, 300, 25);
        
        nameTrackID.setText("Name : ", dontSendNotification);
        nameTrackID.attachToComponent(&nameTrack, true);
        
        inputsID.setText("Inputs : ", dontSendNotification);
        inputsID.attachToComponent(&inputs, true);
        
        launchThread(5);
        
        inputs.addListener (this);
        tree.addListener (this);
        
//        inputTrackToPass = inputTrackToPass2;
        
//        inputs.onChange = [this] { const_cast<String&>(inputTrackToPass) = "inputs.getText()"; };
    }
    
  addTrackWindow::~addTrackWindow()
    {
        
        

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


}

void addTrackWindow::labelTextChanged (Label* labelThatHasChanged) {
  
    if(labelThatHasChanged == &nameTrack){
        Identifier propertyName ("NameTrack");
        tree.setProperty(propertyName, nameTrack.getText(), nullptr);
        nameTrack.getText();
    }
    
};




void addTrackWindow::comboBoxChanged(ComboBox* comboBoxThatHasChanged){
    
    
    if (comboBoxThatHasChanged == &inputs)
    {
        Identifier propertyName ("Inputs");
    tree.setProperty(propertyName, inputs.getText(), nullptr);
        
    }
        
    
}



void addTrackWindow::valueTreeChildAdded (ValueTree& parentTree, ValueTree&) {};
void addTrackWindow::valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) {};
void addTrackWindow::valueTreeChildOrderChanged (ValueTree& parentTree, int, int) {};
void addTrackWindow::valueTreeParentChanged (ValueTree&) {};
void addTrackWindow::valueTreePropertyChanged (ValueTree&, const Identifier&) {};


