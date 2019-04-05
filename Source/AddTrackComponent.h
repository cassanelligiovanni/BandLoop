/*
  ==============================================================================

    AddTrackComponent.h
    Created: 3 Apr 2019 3:54:51pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

/*
 ==============================================================================
 
 addTrackWindow.h
 Created: 13 Mar 2019 11:35:30am
 Author:  Giovanni Cassanelli
 
 ==============================================================================
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "/Users/giovanni/BandLoop/Source/CustomLookAndFeel.h"
#include "/Users/giovanni/BandLoop/Source/CustomLookAndFeel2.h"

class AddTrackComponent   : public Component,
public Label::Listener,
public ValueTree::Listener,
private ComboBox::Listener,
private ChangeListener
{
public:
    AddTrackComponent (const String& name, Colour backgroundColour, int buttonsNeeded, StringArray inputsAvailable, const ValueTree& v, UndoManager& um);
    
    ~AddTrackComponent() ;

    void paint(Graphics& g)override;
    void resized()override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override;
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override;
    void valueTreeParentChanged (ValueTree&) override;
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    
    void labelTextChanged (Label *labelThatHasChanged) override;

    
    
private:
    //==============================================================================
    
    ValueTree tree;
    UndoManager& undoManager;
    
    double progress;
    
    String message;
    CriticalSection messageLock;
    const int timeOutMsWhenCancelling = 1000;
    bool wasCancelledByUser;
    
    Label inputsLabel;
    NamedValueSet& inputsLabelProperties = inputsLabel.getProperties();
    ComboBox inputs;
    NamedValueSet& inputsProperties = inputs.getProperties();
    Label nameTrack;
    NamedValueSet& nameTrackProperties = nameTrack.getProperties();
    Label nameTrackLabel;
    NamedValueSet& nameTrackLabelProperties = nameTrackLabel.getProperties();
    
    
    CustomLookAndFeel2 customLookAndFeel2;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AddTrackComponent)
};
