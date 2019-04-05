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
    
        
        setContentOwned (new AddTrackComponent(name, backgroundColour, buttonsNeeded, inputsAvailable, tree, undoManager), false);
//        addAndMakeVisible(&nameTrack);
//        addAndMakeVisible(&inputsLabel);
//        addAndMakeVisible(&inputs);
//
        Rectangle<int> bounds = getLocalBounds();
        
        tree.addListener(this);
        nameTrack.addListener(this);
        
        
        for (int i = 0; i < inputsAvailable.size(); i++){
            inputs.addItem (inputsAvailable[i], i+1);
 
        }
        
        nameTrackLabel.setText ("BPM", dontSendNotification);
        nameTrackLabel.setColour (Label::textColourId, Colours::BandLoopBackground);
        nameTrackLabel.setJustificationType (Justification::left);
        
        
        nameTrackLabelProperties.set ("fontSize", 30);
        nameTrackLabelProperties.set ("fontStyleFlag", Font::italic);
        
        
//        addAndMakeVisible (&nameTrackLabel);
        
        inputsLabelProperties.set ("fontSize", 30);
        inputsLabelProperties.set ("fontStyleFlag", Font::plain);
        
    
        
        nameTrack.setEditable(true);
        
       
        nameTrack.setBounds(100, 200, 300, 25);
        inputs.setBounds(100, 250, 300, 25);
        
        
        
        launchThread(5);
        
        inputs.addListener (this);
     
        
//        inputTrackToPass = inputTrackToPass2;
        
//        inputs.onChange = [this] { const_cast<String&>(inputTrackToPass) = "inputs.getText()"; };
    }
    
  addTrackWindow::~addTrackWindow()
    {
        nameTrackLabel.setLookAndFeel(nullptr);
        inputsLabel.setLookAndFeel(nullptr);
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


}

void addTrackWindow::labelTextChanged (Label* labelThatHasChanged) {
  
    if(labelThatHasChanged == &nameTrack){
        Identifier propertyName ("Name");
        tree.setProperty(propertyName, nameTrack.getText(), &undoManager);
        nameTrack.getText();
    }
    
};




void addTrackWindow::comboBoxChanged(ComboBox* comboBoxThatHasChanged){
    
    
    if (comboBoxThatHasChanged == &inputs)
    {
        Identifier propertyName ("Inputs");
    tree.setProperty(propertyName, inputs.getText(), &undoManager);
        
    }
        
    
}



void addTrackWindow::valueTreeChildAdded (ValueTree& parentTree, ValueTree&) {};
void addTrackWindow::valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) {};
void addTrackWindow::valueTreeChildOrderChanged (ValueTree& parentTree, int, int) {};
void addTrackWindow::valueTreeParentChanged (ValueTree&) {};
void addTrackWindow::valueTreePropertyChanged (ValueTree&, const Identifier&) {};
