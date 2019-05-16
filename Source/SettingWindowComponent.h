/*
  ==============================================================================

    SettingWindowComponent.h
    Created: 29 Apr 2019 7:31:56pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once
/*
  ==============================================================================

    SettingWindow.h
    Created: 19 Mar 2019 4:32:30pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/


#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "/Users/giovanni/BandLoop/Source/CustomLookAndFeel2.h"


class SettingWindowComponent   : public Component,
public Thread,
public Label::Listener,
public ValueTree::Listener,
public Button::Listener,
private ComboBox::Listener,
private Timer,
private ChangeListener

{
public:
    SettingWindowComponent (const String& name,
                   Colour backgroundColour,
                   int buttonsNeeded,
                   const ValueTree& v,
                   UndoManager& um, AudioDeviceManager& deviceManager, Array<int>& pedalsAvailable, MidiMessageCollector& midiMessageCollector);
    
    ~SettingWindowComponent() ;
    
    void resized() override;
    
    void paint(Graphics& g) override;
    
    void run() override;
        
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    void buttonClicked(Button* button) override;
    
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override;
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override;
    void valueTreeParentChanged (ValueTree&) override;
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    
    void labelTextChanged (Label *labelThatHasChanged) override;
    
    
    void launchThread (int priority = 5);
    
    void pairing();
    
    void numberOfPedals ();
    
    void pedalClicked(int pedalNumber) ;
    
    MidiMessageCollector& midiCollector ;
    Array<int>& pedalsAvailables ;
    
private:
    //==============================================================================
    
    AudioDeviceSelectorComponent audioSetupComp;
    
    TextButton pairingButton {"Pairing Button"};
//    MidiOutput midiOutput;
    
    ValueTree tree;
    UndoManager& undoManager;
    
    void timerCallback() override;
    double progress;
    
    String message;
    CriticalSection messageLock;
    const int timeOutMsWhenCancelling = 1000;
    bool wasCancelledByUser;

    CustomLookAndFeel2  customLookAndFeel2;
    
    ScopedPointer<Drawable> pedalImage = Drawable::createFromImageData (BinaryData::pedal_svg, BinaryData::pedal_svgSize);
    ScopedPointer<Drawable> pedalImagePressed = Drawable::createFromImageData (BinaryData::pedal_svg, BinaryData::pedal_svgSize);

    
    OwnedArray<DrawableButton> pedalButtons;
   
    int pedalJustTriggered = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingWindowComponent)
};
