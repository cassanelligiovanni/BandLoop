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
                                UndoManager& um, AudioDeviceManager& deviceManager,Array<int>& pedalsAvailable, MidiMessageCollector& midiMessageCollector)
:
DocumentWindow (name, backgroundColour, buttonsNeeded),
Thread ("ThreadWithProgressWindow"),
progress (0.0),
wasCancelledByUser (false), audioSetupComp (deviceManager,  0,  256,  0, 256,  true,  false,  false, false),
tree (v), undoManager (um),
midiCollector(midiMessageCollector),
pedalsAvailables(pedalsAvailable),
settingWindow( "Settings", Colours::BandLoopBackground, DocumentWindow::allButtons, tree,
                       undoManager, deviceManager, pedalsAvailables, midiCollector)
{
    bool result = pedalImage->replaceColour (Colours::black, Colours::white);
    bool result2 = pedalImagePressed->replaceColour (Colours::black, Colours::red);
    
    setLookAndFeel(&customLookAndFeel2);

    launchThread(5);
    
//    audioSetupComp.setBounds (0, 0, 350, 200);
//    pairingButton.setBounds(20, 330, 360, 30);
    
    for (int i = 0 ; i < 5 ; i++  ) {
        
    auto* newPedal = new DrawableButton("newButton", DrawableButton::ImageFitted );
    pedalButtons.add(newPedal);
    pedalButtons.getLast()->setImages(pedalImage, pedalImage, pedalImage , pedalImage, pedalImagePressed);
        
    pedalButtons.getLast()->setBounds ( (i*60) + 20, 400, 50, 50);
        
      
    auto* newLabel = new Label("newLabel",String(pedalButtons.size()));
    newLabel->attachToComponent(pedalButtons.getLast(), false);
        
        
    }
    
    setSize(600, 600);
    
 
    
    setContentOwned(&settingWindow, false);
    
    
    
    
}

SettingWindow::~SettingWindow()
{
    setLookAndFeel(nullptr);
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
    wait(100);
    delete this;
    
}



void SettingWindow::launchThread (int priority)
{
    
    startThread (priority);
    
    
}


void SettingWindow::pairing() {
    
                auto message = MidiMessage::noteOn (2, 5, 0.8f);
                message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
//                midiCollector.addMessageToQueue(message);
    
    for (int i  = 0; i < MidiOutput::getDevices().size(); i++) {
        MidiOutput* midiOutput;
        midiOutput = MidiOutput::openDevice(i);
        midiOutput->sendMessageNow (message);
        midiOutput = nullptr;
    }
    
    
    pedalsAvailables.clear();
    
    for (int i = 0 ; i < pedalButtons.size(); i ++) {
        
        pedalButtons[i]->setVisible(false);
        
        
    }
    
}


void SettingWindow::numberOfPedals() {
    


    for (int i = 0 ; i < pedalsAvailables.size(); i ++) {
        
        pedalButtons[i]->setVisible(true);
        
    }

}

void SettingWindow::pedalClicked(int pedalNumber) {
    
    settingWindow.pedalClicked(pedalNumber);
    
    
};
