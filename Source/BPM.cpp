/*
  ==============================================================================

    BPM.cpp
    Created: 17 Mar 2019 3:41:01pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "BPM.h"

#include <functional>

//==============================================================================
BPM::BPM(const ValueTree& v,
         UndoManager& um) :
tree (v), undoManager (um)
{
    
    playButton.setToggleState(false, NotificationType::dontSendNotification);
    playButton.setColour(TextButton::ColourIds::buttonColourId, Colours::red);
    playButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::limegreen);
    playButton.onClick = [this]() { play(); };
    playButton.addListener(this);
    addAndMakeVisible(playButton);
    

    labelBPM.setText ("BPM", dontSendNotification);
    labelBPM.attachToComponent (&textBPM, true);
    labelBPM.setColour (Label::textColourId, Colours::orange);
    labelBPM.setJustificationType (Justification::right);
    addAndMakeVisible (labelBPM);
    
    
    textBPM.setEditable (true);
    textBPM.setColour (Label::backgroundColourId, Colours::darkblue);
    addAndMakeVisible (textBPM);
    
    textBPM.onTextChange = [this] { BPMs = static_cast <int>(textBPM.getText().getFloatValue());};
    
    //    transportSource.addChangeListener (this);
    
    Timer::startTimer(100);
    
    
    setSize (600, 300);

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
        playButton.setToggleState(true, NotificationType::dontSendNotification);
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
        playButton.setToggleState(false, NotificationType::dontSendNotification);
        playButton.setButtonText("Stopped");
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
    
    timeUIfloat = (static_cast<float>(timeInMilliseconds))/1000;
    Bars = timeUIfloat*BPMratio/4;
    Bar = static_cast<int>(Bars);
    
    Beats = (timeUIfloat)*BPMratio;
    Beat = (static_cast<int>(Beats))%4 + 1;
    
    
}

void BPM::buttonClicked (Button* button)
{
    if (button == &playButton)
    {
        if (playState == PlayState::Play)
        {
            playButton.onClick = [&]() { stop(); };
        }
        else
        {
            playButton.onClick = [&]() { play(); };
        }
    }
}

//==============================================================================

//==============================================================================
void BPM::paint (Graphics& g)
{
    Rectangle<int> timeUIBounds { 50, 10, 100, 20 };
    
    g.fillAll(Colours::black);
    g.setFont(15.0f);
    g.setColour(Colours::white);
    BarToText = static_cast<String>(Bar);
    BeatToText = static_cast<String>(Beat);
    g.drawText(BarToText, timeUIBounds, Justification::centred);
    g.drawText(BeatToText, timeUIBounds, Justification::centred - 10);
    
    
}

void BPM::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    
    FlexBox flexbox { FlexBox::Direction::row, FlexBox::Wrap::noWrap, FlexBox::AlignContent::center, FlexBox::AlignItems::center, FlexBox::JustifyContent::center };
    
    flexbox.items.add(FlexItem(100, 100, playButton));
    flexbox.items.add(FlexItem(100, 100, labelBPM));
    flexbox.items.add(FlexItem(100, 100, textBPM));
    flexbox.performLayout(bounds);
    //
    //        repaint();
}


void BPM::timerCallback() {
    
    repaint();
    
    
}

