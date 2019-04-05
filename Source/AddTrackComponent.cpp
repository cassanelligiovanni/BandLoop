/*
  ==============================================================================

    AddTrackComponent.cpp
    Created: 3 Apr 2019 3:54:51pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "AddTrackComponent.h"


AddTrackComponent::AddTrackComponent (const String& name,
                                Colour backgroundColour,
                                int buttonsNeeded,
                                StringArray inputsAvailable,
                                const ValueTree& v,
                                UndoManager& um)
:
progress (0.0),
wasCancelledByUser (false),

tree (v), undoManager (um)
{
  
    Rectangle<int> bounds = getLocalBounds();
    
    tree.addListener(this);
    nameTrack.addListener(this);
    
    
    for (int i = 0; i < inputsAvailable.size(); i++){
        inputs.addItem (inputsAvailable[i], i+1);
        
    }
    
    nameTrackLabel.setText ("name", dontSendNotification);
    nameTrackLabel.setColour (Label::textColourId, Colours::BandLoopText);
    nameTrackLabel.setJustificationType (Justification::centred);
    nameTrackLabelProperties.set ("fontSize", 40);
    nameTrackLabelProperties.set ("fontStyleFlag", Font::plain);
    
    inputsLabel.setText ("inputs", dontSendNotification);
    inputsLabel.setColour (Label::textColourId, Colours::BandLoopText);
    inputsLabel.setJustificationType (Justification::centred);
    inputsLabelProperties.set ("fontSize", 40);
    inputsLabelProperties.set ("fontStyleFlag", Font::plain);
    
    nameTrack.setJustificationType (Justification::centred);
    nameTrackProperties.set ("fontSize", 30);
    nameTrackProperties.set ("fontStyleFlag", Font::plain);
    
    nameTrack.setLookAndFeel(&customLookAndFeel2);
    nameTrack.setEditable(true);
    
   
    inputs.setJustificationType (Justification::left);
    inputs.setLookAndFeel(&customLookAndFeel2);
    inputs.setTextWhenNothingSelected("select one...");
    inputs.addListener (this);

    
    addAndMakeVisible (nameTrackLabel);
    addAndMakeVisible (nameTrack);
    addAndMakeVisible (inputsLabel);
    addAndMakeVisible (inputs);
    
    //        inputTrackToPass = inputTrackToPass2;
    
    //        inputs.onChange = [this] { const_cast<String&>(inputTrackToPass) = "inputs.getText()"; };
}

AddTrackComponent::~AddTrackComponent()
{   inputs.setLookAndFeel(nullptr);
    nameTrack.setLookAndFeel(nullptr);
    inputsLabel.setLookAndFeel(nullptr);
}




void AddTrackComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    
}



void AddTrackComponent::labelTextChanged (Label* labelThatHasChanged) {
    
    if(labelThatHasChanged == &nameTrack){
        Identifier propertyName ("Name");
        tree.setProperty(propertyName, nameTrack.getText(), &undoManager);
        nameTrack.getText();
    }
    
};




void AddTrackComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged){
    
    
    if (comboBoxThatHasChanged == &inputs)
    {
        Identifier propertyName ("Inputs");
        tree.setProperty(propertyName, inputs.getText(), &undoManager);
        
    }
    
    
}



void AddTrackComponent::valueTreeChildAdded (ValueTree& parentTree, ValueTree&) {};
void AddTrackComponent::valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) {};
void AddTrackComponent::valueTreeChildOrderChanged (ValueTree& parentTree, int, int) {};
void AddTrackComponent::valueTreeParentChanged (ValueTree&) {};
void AddTrackComponent::valueTreePropertyChanged (ValueTree&, const Identifier&) {};

void AddTrackComponent::paint(Graphics& g) {}
void AddTrackComponent::resized() {
    
    nameTrackLabel.setBounds(0, 20, getWidth(), 30);
    nameTrack.setBounds(30, 70, getWidth() - 60, 30);
    inputsLabel.setBounds(0, 140, getWidth(), 30);
    inputs.setBounds(30, 190, getWidth()-60, 40);
}
