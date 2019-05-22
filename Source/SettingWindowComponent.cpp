/*
  ==============================================================================

    SettingWindowComponent.cpp
    Created: 29 Apr 2019 7:31:56pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "SettingWindowComponent.h"
/*
  ==============================================================================

    SettingWindow.cpp
    Created: 19 Mar 2019 4:32:30pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/



SettingWindowComponent::SettingWindowComponent (const String& name,
                                Colour backgroundColour,
                                int buttonsNeeded,
                                const ValueTree& v,
                                UndoManager& um, AudioDeviceManager& deviceManager,Array<int>& pedalsAvailable, MidiMessageCollector& midiMessageCollector)
:
//Thread ("ThreadWithProgressWindow"),
progress (0.0),
wasCancelledByUser (false), audioSetupComp (deviceManager,  0,  256,  0, 256,  true,  false,  false, false),
tree (v), undoManager (um),
midiCollector(midiMessageCollector),
pedalsAvailables(pedalsAvailable)

{
    
    bool result = pedalImage->replaceColour (Colours::black, Colours::white);
    bool result2 = pedalImagePressed->replaceColour (Colours::black, Colours::red);
    
    setLookAndFeel(&customLookAndFeel2);
    deviceManager.addChangeListener (this);
    tree.addListener(this);
    addAndMakeVisible(audioSetupComp);
    addAndMakeVisible(pairingButton);
    
//    launchThread(5);
    
    audioSetupComp.setBounds (0, 0, 350, 200);
    pairingButton.setBounds(20, 400, 360, 30);
    for (int i = 0 ; i < 5 ; i++  ) {
        
        auto* newPedal = new DrawableButton("newButton", DrawableButton::ImageFitted );
        pedalButtons.add(newPedal);
        pedalButtons.getLast()->setImages(pedalImage, pedalImage, pedalImage , pedalImage, pedalImagePressed);
        
        pedalButtons.getLast()->setBounds ( (i*60) + 20, 470, 50, 50);
        
        

        
        addAndMakeVisible(pedalButtons[i]);
        pedalButtons[i]->setBounds ( (i*60) + 20, 470, 50, 50);
        
        
    }

    
    pairingButton.addListener(this);

    pairing();
    
}

SettingWindowComponent::~SettingWindowComponent()
{
    stopTimer();
    setLookAndFeel(nullptr);
//    stopThread(200);
    
    
}

void SettingWindowComponent::resized() {
    
    
};
    
void SettingWindowComponent::paint(Graphics& g) {};

void SettingWindowComponent::run(){
    
//    while (! threadShouldExit())
//    {
//            wait (100);
    
        // because this is a background thread, we mustn't do any UI work without
        // first grabbing a MessageManagerLock..
        
        const MessageManagerLock mml (Thread::getCurrentThread());
    
        if(pedalJustTriggered>-1) {
         pedalButtons[pedalJustTriggered]->setToggleState(!(pedalButtons[pedalJustTriggered]->getToggleState()), dontSendNotification);
        pedalJustTriggered = -1;
        }
        numberOfPedals();
        

//        if (! mml.lockWasGained())  // if something is trying to kill this job, the lock
//            return;                 // will fail, in which case we'd better return..
//
//    }
    
    
}


void SettingWindowComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    
}

void SettingWindowComponent::launchThread (int priority)
{
    
//    startThread (priority);
    startTimer (100);
    
    
}


void SettingWindowComponent::timerCallback()
{
   
    
}

void SettingWindowComponent::labelTextChanged (Label* labelThatHasChanged) {

};




void SettingWindowComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged){
    
}

void SettingWindowComponent::buttonClicked(Button* button) {
    
    if (button == &pairingButton) {
        
        pairing();
        
    }
    
}



void SettingWindowComponent::valueTreeChildAdded (ValueTree& parentTree, ValueTree&) {};
void SettingWindowComponent::valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) {};
void SettingWindowComponent::valueTreeChildOrderChanged (ValueTree& parentTree, int, int) {};
void SettingWindowComponent::valueTreeParentChanged (ValueTree&) {};
void SettingWindowComponent::valueTreePropertyChanged (ValueTree&, const Identifier&) {};


void SettingWindowComponent::pairing() {
    
                auto message = MidiMessage::noteOn (2, 5, 0.8f);
                message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
//                midiCollector.addMessageToQueue(message);
    
    for (int i  = 0; i < MidiOutput::getDevices().size(); i++) {
        midiOutput.reset(MidiOutput::openDevice(i));
//        MidiOutput* midiOutput;
//        MidiOutput* midiOutput = MidiOutput::openDevice(i);
        midiOutput->sendMessageNow (message);
        midiOutput = nullptr;

    }
    
    
    pedalsAvailables.clear();
    
    for (int i = 0 ; i < pedalButtons.size(); i ++) {
        
        pedalButtons[i]->setVisible(false);
        
        
    }
    
}


void SettingWindowComponent::numberOfPedals() {
    


    for (int i = 0 ; i < pedalsAvailables.size(); i ++) {
        
        pedalButtons[i]->setVisible(true);
        
    }
    

    

}


void SettingWindowComponent::pedalClicked(int pedalNumber) {

    pedalJustTriggered = pedalNumber;
    
}



