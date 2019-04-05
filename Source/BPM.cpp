/*
  ==============================================================================

    BPM.cpp
    Created: 17 Mar 2019 3:41:01pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "BPM.h"
#include "CustomLookAndFeel.h"
#include <functional>



//==============================================================================
BPM::BPM(const ValueTree& v,
         UndoManager& um) :
tree (v), undoManager (um)
{
    
    
    DrawablePlayButton.setImages ( playButtonImage,
                                  playButtonImage,
                                  playButtonImage,
                                  playButtonImage,
                                  stopButtonImage,
                                  stopButtonImage,
                                  stopButtonImage, stopButtonImage);
    
    bpmDisplayButton.setImages(bpmDisplay);
    
    for(int i  = 0; i< 4; i ++) {
    
        
    DrawableButton* newBpmCounter = new DrawableButton {"newButton", DrawableButton::ImageFitted };
        BpmCounters.add(newBpmCounter);
    }

    
    for (int i = 0; i< BpmCounters.size(); i ++) {
    addAndMakeVisible(BpmCounters[i]);
        BpmCounters[i]->setImages ( OFF,nullptr,nullptr,nullptr, ON,nullptr,nullptr, nullptr);
    }

    addAndMakeVisible(bpmDisplayButton);
    addAndMakeVisible(DrawablePlayButton);
    DrawablePlayButton.onClick = [this](){play();};
    DrawablePlayButton.addListener(this);
    
    BPMratio = BPMs/60.;
    loopUnit = 4/BPMratio;
    Identifier  BPMratio ("BPMratio");
    tree.setProperty(BPMratio, loopUnit, &undoManager);

    playButton.setToggleState(false, NotificationType::dontSendNotification);
    playButton.setColour(TextButton::ColourIds::buttonColourId, Colours::red);
    playButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::limegreen);
    playButton.onClick = [this]() { play(); };
    playButton.addListener(this);
    addAndMakeVisible(playButton);
    

    labelBPM.setText ("BPM", dontSendNotification);
    labelBPM.attachToComponent (&textBPM, false);
    labelBPM.setColour (Label::textColourId, Colours::BandLoopBackground);
    labelBPM.setJustificationType (Justification::left);
    
    
    labelBPMProperties.set ("fontSize", 30);
    labelBPMProperties.set ("fontStyleFlag", Font::italic);
    addAndMakeVisible (labelBPM);
    
    textBPM.setText ("127", dontSendNotification);
    textBPM.setEditable (true);
    textBPM.setColour (Label::backgroundColourId, Colours::transparentWhite);
    textBPM.setColour (Label::textColourId, Colours::BandLoopBackground);
    textBPMProperties.set ("fontSize", 50);
    textBPMProperties.set ("fontStyleFlag", Font::italic);
    addAndMakeVisible (textBPM);
    
    textBPM.onTextChange = [this] { BPMs = static_cast <int>(textBPM.getText().getFloatValue());};
    
    labelBeat.setColour (Label::textColourId, Colours::BandLoopBackground);
    labelBar.setColour (Label::textColourId, Colours::BandLoopBackground);
    
    addAndMakeVisible(labelBeat);
    addAndMakeVisible(labelBar);
    //    transportSource.addChangeListener (this);
    
    Timer::startTimer(5);

}

BPM::~BPM()
{
    // This shuts down the audio device and clears the audio source.

}

void BPM::play()
{
    if (playState == PlayState::Stop)
    {
        playState = PlayState::Play;
//        playButton.setToggleState(true, NotificationType::dontSendNotification);
        DrawablePlayButton.setToggleState(true, NotificationType::dontSendNotification);
        playButton.setButtonText("Playing");
        //        edit.getTransport().play(false);
        HighResolutionTimer::startTimer(1);

        
    }
}

void BPM::stop()
{
    if (playState == PlayState::Play)
    {
        
        playState = PlayState::Stop;
        //        edit.getTransport().stop(true, false);
//        playButton.setToggleState(false, NotificationType::dontSendNotification);
//        playButton.setButtonText("Stopped");
        DrawablePlayButton.setToggleState(false, NotificationType::dontSendNotification);
        HighResolutionTimer::stopTimer();
        timeInMilliseconds = 0;
        Bar = 0;
        Beat = 0;
    }
}

void BPM::hiResTimerCallback()
{
    BPMratio = BPMs/60.;
    
    if(HighResolutionTimer::getTimerInterval()) {
        timeInMilliseconds ++ ;
    }
    
}

void BPM::buttonClicked (Button* button)
{
 
    
    if (button == &DrawablePlayButton)
    {
        if (playState == PlayState::Play)
        {
            DrawablePlayButton.onClick = [&]() { stop(); };
        }
        else
        {
            DrawablePlayButton.onClick = [&]() { play(); };
        }
    }
    
    
//    if (button == &playButton)
//    {
//        if (playState == PlayState::Play)
//        {
//            playButton.onClick = [&]() { stop(); };
//        }
//        else
//        {
//            playButton.onClick = [&]() { play(); };
//        }
//    }
}

//==============================================================================

//==============================================================================
void BPM::paint (Graphics& g)
{
    
//    Rectangle<int> timeUIBounds = (getLocalBounds());
//    Rectangle<int> BarArea (timeUIBounds.getWidth()/10*8, 0, timeUIBounds.getWidth()/10, timeUIBounds.getWidth()/10*3);
//    Rectangle<int> BeatArea (40, 0, timeUIBounds.getWidth()/10, timeUIBounds.getWidth()/10*3);
//

//    g.setFont(40.f);
//    g.setColour(Colours::white);
//    BarToText = static_cast<String>(BarToDisplay);
//    BeatToText = static_cast<String>(Beat);
//    g.drawText(BarToText, BarArea, Justification::left);
//    g.drawText(BeatToText, BeatArea , Justification::left);
    
    
}

void BPM::resized()
{

    Rectangle<int> bounds = getLocalBounds();
    
    DrawablePlayButton.setBounds(0, -10, bounds.getWidth()/10, bounds.getHeight());
    
    textBPM.setBounds(7*(bounds.getWidth()/10), (bounds.getHeight()/2), (bounds.getWidth()/10*3), (bounds.getHeight()/2));
  
    labelBPMProperties.set("fontSize", (bounds.getHeight()/2*0.6));
    textBPMProperties.set("fontSize", (bounds.getHeight()/2));

    
    labelBar.setBounds(9*(bounds.getWidth()/10), 0, (bounds.getWidth()/10*3), (bounds.getHeight()/2));
    labelBarProperties.set("fontSize", (bounds.getHeight()/2*0.6));
                        
    bpmDisplayButton.setBounds(7*(bounds.getWidth()/10), 0, (bounds.getWidth()/10*3), bounds.getHeight());
    
    for(int i = 0; i< BpmCounters.size(); i++) {
        
        BpmCounters[i]->setBounds((i+2)*(bounds.getWidth()/10), /*(bounds.getWidth()/10/2 - (bounds.getWidth()/10/2)*0.8)*/0, (bounds.getWidth()/10)*0.8, (bounds.getHeight())*0.8);
        
    }
    



            }


void BPM::timerCallback() {
    
    timeUIfloat = (static_cast<float>(timeInMilliseconds))/1000;
    Bars = timeUIfloat*BPMratio/4;
    BarToDisplay = static_cast<int>(Bars);
    int BarToSend = roundToInt(Bars);
    Bar = (Bars);
    
    Beats = (timeUIfloat)*BPMratio;
    Beat = (static_cast<int>(Beats))%4 + 1;
    
    //    Identifier  Beating ("beat");
//    tree.setProperty(Beating, Beat, &undoManager);
    
    //    Identifier  Baring ("bar");
    //    tree.setProperty(Baring, Bar, &undoManager);
  
    if (BPMratio != previousBPMratio)
    {
        loopUnit = 4./BPMratio;
    Identifier  BPMratio ("BPMratio");

    tree.setProperty(BPMratio, loopUnit, &undoManager);
    }
    
    if( (fmod (Bar,1)) > 1 - 0.010 && (fmod (Bar,1)) < 1 && justTriggered == false) {
        Identifier  Baring ("bar");
        tree.setProperty(Baring, BarToSend, &undoManager);
        justTriggered = true;
//        DBG("Ready");
    }
    
    if(( (fmod (Bar,1)) == 1  || (fmod (Bar,1)) < 0.005) && justTriggered == true) {
        Identifier  Baring ("bar");
        tree.setProperty(Baring, 0, &undoManager);
        justTriggered = false;
//        DBG("Playing");
    }
    
    
//    check wheter or nt the UI is really to Repaint, improving performances
//  HERE will be probably called the LED function
    if (Beat != previousBeat ){
            for (int i = 0; i< BpmCounters.size(); i++) {
                BpmCounters[i]->setToggleState(false, NotificationType::dontSendNotification);}
             BpmCounters[Beat-1]->setToggleState(true, NotificationType::dontSendNotification);
        
            BarToText = static_cast<String>(BarToDisplay);
            BeatToText = static_cast<String>(Beat);
        labelBeat.setText(BeatToText, dontSendNotification);
        labelBar.setText(BarToText, dontSendNotification);
        
         repaint();
    }
    
    previousBeat = Beat;
    previousBPMratio = BPMratio;
}


