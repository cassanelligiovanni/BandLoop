/*
  ==============================================================================

    addTrackWindow.h
    Created: 13 Mar 2019 11:35:30am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


class addTrackWindow   : public DocumentWindow,
                         public Thread,
                         public Label::Listener,
                         public ValueTree::Listener,
                         private ComboBox::Listener,
                         private Timer,
                         private ChangeListener
{
public:
    addTrackWindow (const String& name, Colour backgroundColour, int buttonsNeeded, StringArray inputsAvailable, const ValueTree& v, UndoManager& um);
    
    ~addTrackWindow() ;
    
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
    
//        ThreadWithProgressWindow (const String& windowTitle,
//                                  bool hasProgressBar,
//                                  bool hasCancelButton,
//                                  int timeOutMsWhenCancelling = 10000,
//                                  const String& cancelButtonText = String(),
//                                  Component* componentToCentreAround = nullptr);
    
    
        
        //==============================================================================

    
        void launchThread (int priority = 5);
    
        
    private:
        //==============================================================================
    
    ValueTree tree;
    UndoManager& undoManager;
    
    void timerCallback() override;
        
        double progress;
 
        String message;
        CriticalSection messageLock;
        const int timeOutMsWhenCancelling = 1000;
        bool wasCancelledByUser;
    
       Label inputsID;
        ComboBox inputs;
         Label nameTrack;
        Label nameTrackID;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (addTrackWindow)
};
