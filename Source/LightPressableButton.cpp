/*
  ==============================================================================

    LightPressableButton.cpp
    Created: 21 Apr 2019 10:48:34am
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "LightPressableButton.h"
#include "/Users/giovanni/BandLoop/Source/RecordState.h"


LightPressableButton::LightPressableButton(RecordState& recordStateR, bool& isStartingRecordingR, bool& isStoppingRecordingR,int& pedalNumber, RecordState& recordState) :
recordState(recordStateR)
, isStartingRecording(isStartingRecordingR)
, isStoppingRecording(isStoppingRecordingR)
, nPedal(pedalNumber)
, recState(recordState)

{
    DrawablePlayingBarOn.setImages(barPlayingOnImage);
    DrawablePlayingBarOff.setImages(barPlayingOffImage);
    DrawableRecordingBarOn.setImages(barRecordingOnImage);
    DrawableRecordingBarOff.setImages(barRecordingOffImage);
    
//    recordImage.setImages(barRecordingOffImage);
    
//    addAndMakeVisible(frame);
    
    addAndMakeVisible(DrawablePlayingBarOn);
    addAndMakeVisible(DrawablePlayingBarOff);
    addAndMakeVisible(DrawableRecordingBarOn);
    addAndMakeVisible(DrawableRecordingBarOff);
    addAndMakeVisible(recordImage);
//    addAndMakeVisible(recordButton);

//    frame.setColour(Colours::BandLoopBackground);
//    recordButton.setColour(TextButton::buttonColourId,  Colours::transparentWhite);
//    recordButton.setColour(TextButton::textColourOffId,  Colours::transparentWhite);
    
    startTimer(100);
    
}

LightPressableButton::~LightPressableButton(){}

void LightPressableButton::paint(Graphics& g) {
    
};

void LightPressableButton::resized() {
    
    Rectangle<int> bounds= getLocalBounds();
    
//    frame.setBounds(bounds);
    labelBounds = bounds;
    
    DrawablePlayingBarOn.setBounds( 0, 0, bounds.getWidth(), bounds.getHeight());
    DrawablePlayingBarOff.setBounds( 0, 0, bounds.getWidth(), bounds.getHeight());
    DrawableRecordingBarOn.setBounds( 0, 0, bounds.getWidth(), bounds.getHeight());
    DrawableRecordingBarOff.setBounds( 0, 0, bounds.getWidth(), bounds.getHeight());
    
//    recordImage. setBounds(0, 0, 10, 10);
//    recordButton.setBounds(bounds);
};

void LightPressableButton::timerCallback() {
   
    alternateFlag ++ ;

    if (recState == RecordState::isStartingRecording) {
        barRecordingBlinking();
    }

    if (recState == RecordState::isStartingPlaying || recState == RecordState::isStartingStopping) {
        barPlayingBlinking();
    }

    if(recState == RecordState::Recording ) {
        barRecordingOn();
    }
    
    if(recState == RecordState::Playing) {
        barPlayingOn();
    }
    
    if(recState == RecordState::Stopped) {
        barPlayingOn();
    }
    
    if (updateGui == true) {
        messageToPedal();
        updateGui = false;
    }
}

void LightPressableButton::barRecordingOn() {
    
    type  = 2;
    
    DrawablePlayingBarOn.setVisible(true);
    DrawablePlayingBarOff.setVisible(false);
    DrawableRecordingBarOn.setVisible(true);
    DrawableRecordingBarOff.setVisible(false);
    
}

void LightPressableButton::barRecordingBlinking() {
    
    type = 1 ;
    
    DrawableRecordingBarOn.setVisible(true);
    DrawableRecordingBarOff.setVisible(true);
    
    if(alternateFlag > 3) {
        
        flagRecOn = !flagRecOn ;
        flagRecOff = !flagRecOff;
        
        if (!flagRecOn)
            DrawableRecordingBarOn.setVisible(false);
        if (!flagRecOff)
            DrawableRecordingBarOff.setVisible(false);
        alternateFlag = 0;
    }
    
    DrawablePlayingBarOn.setVisible(false);
    DrawablePlayingBarOff.setVisible(false);
    
}

void LightPressableButton::barPlayingOn() {
    
    type = 4;
    
    DrawablePlayingBarOn.setVisible(true);
    DrawablePlayingBarOff.setVisible(false);
    DrawableRecordingBarOn.setVisible(false);
    DrawableRecordingBarOff.setVisible(false);
    
}

void LightPressableButton::barPlayingBlinking() {
    
    type = 3;
    
    DrawablePlayingBarOn.setVisible(true);
    DrawablePlayingBarOff.setVisible(true);
    
    if(alternateFlag > 3) {
        
        flagPlayOn = !flagPlayOn ;
        flagPlayOff = !flagPlayOff;
        
        if (!flagPlayOn)
            DrawablePlayingBarOn.setVisible(false);
        
        if (!flagPlayOff)
            DrawablePlayingBarOff.setVisible(false);
        alternateFlag = 0;
    }
    DrawableRecordingBarOn.setVisible(false);
    DrawableRecordingBarOff.setVisible(false);
    
}


void LightPressableButton::updateTrue() {
    
    updateGui = true;
    
}

void LightPressableButton::messageToPedal() {
  
    if(nPedal) {
    

    int midiToSend = ((nPedal)*10) + type ;

    auto message = MidiMessage::noteOn (2, midiToSend, 0.8f);
    message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
    //                midiCollector.addMessageToQueue(message);
    
    for (int i  = 0; i < MidiOutput::getDevices().size(); i++) {
        MidiOutput* midiOutput;
        midiOutput = MidiOutput::openDevice(i);
        midiOutput->sendMessageNow (message);
        midiOutput = nullptr;
    }
    
    }
    

}
