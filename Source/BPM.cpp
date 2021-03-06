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

BPM::BPM(const ValueTree& v, UndoManager& um, ADMinfo& ADM, std::atomic<float>& newBar)
: tree (v)
, undoManager (um)
,  admInfo(ADM)
, click(outIsStereo, outputL, outputR)
, bar(newBar)
{
    
    drawButton();
    drawBPMdisplay();
    drawCounter();
    
    admInfo.addChangeListener(this);
    initialiseOutputSelector();

    DrawablePlayButton.onClick = [this](){play();};
    DrawablePlayButton.addListener(this);
    
    Identifier  BPMratio ("BPMratio");
    tree.setProperty(BPMratio, loopUnit, &undoManager);
    
    Timer::startTimer(5);
    

}

BPM::~BPM() {
    Timer::stopTimer();

}

void BPM::play()
{
    if (playState == PlayState::Stop)
    {
        playState = PlayState::Play;
        DrawablePlayButton.setToggleState(true, NotificationType::dontSendNotification);
        HighResolutionTimer::startTimer(1);
    }
}

void BPM::stop()
{
    if (playState == PlayState::Play)
    {
        
        playState = PlayState::Stop;
        DrawablePlayButton.setToggleState(false, NotificationType::dontSendNotification);
        HighResolutionTimer::stopTimer();
    
        //reset globalTempo
        timeInMilliseconds = 0;
        Bar = 0;
        Beat = 1;
    }
}




//================= GRAPHICS ====================================================================

void BPM::paint (Graphics& g){ }

void BPM::resized()
{

    Rectangle<int> bounds = getLocalBounds();
    DrawablePlayButton.setBounds(0, -10, bounds.getWidth()/13, bounds.getHeight());
    textBPM.setBounds(7*(bounds.getWidth()/13), (bounds.getHeight()/2), (bounds.getWidth()/13*3), (bounds.getHeight()/2));
    labelBPMProperties.set("fontSize", (bounds.getHeight()/2*0.6));
    textBPMProperties.set("fontSize", (bounds.getHeight()/2));
    labelBar.setBounds(9*(bounds.getWidth()/13), 0, (bounds.getWidth()/13*3), (bounds.getHeight()/2));
    labelBarProperties.set("fontSize", (bounds.getHeight()/2*0.6));
    bpmDisplayButton.setBounds(7*(bounds.getWidth()/13), 0, (bounds.getWidth()/13*3), bounds.getHeight());
    for(int i = 0; i< BpmCounters.size(); i++) {
        BpmCounters[i]->setBounds((i+2)*(bounds.getWidth()/13), /*(bounds.getWidth()/10/2 - (bounds.getWidth()/10/2)*0.8)*/0, (bounds.getWidth()/13)*0.8, (bounds.getHeight())*0.8);
    }
    
//    headphonesOutputSelector.setBounds(11*(bounds.getWidth()/13), 20, (bounds.getWidth()/13*2), 25);
}


void BPM::drawButton () {
    playButtonImage = Drawable::createFromImageData (BinaryData::PlayButton_svg, BinaryData::PlayButton_svgSize);
    stopButtonImage = Drawable::createFromImageData (BinaryData::StopButton_svg, BinaryData::StopButton_svgSize);
    DrawablePlayButton.setImages ( playButtonImage,
                                playButtonImage,
                                  playButtonImage,
                                  playButtonImage,
                                  stopButtonImage,
                                  stopButtonImage,
                                  stopButtonImage, stopButtonImage);
    addAndMakeVisible(DrawablePlayButton);
}


void BPM::drawCounter() {
    
    ON = Drawable::createFromImageData (BinaryData::BPMCounterON_svg, BinaryData::BPMCounterON_svgSize);
    OFF = Drawable::createFromImageData (BinaryData::BPMCounterOFF_svg, BinaryData::BPMCounterOFF_svgSize);
    
    for(int i  = 0; i< 4; i ++) {
        
        DrawableButton* newBpmCounter = new DrawableButton {"newButton", DrawableButton::ImageFitted };
        BpmCounters.add(newBpmCounter);
        addAndMakeVisible(BpmCounters[i]);
        BpmCounters[i]->setImages ( OFF,nullptr,nullptr,nullptr, ON,nullptr,nullptr, nullptr);
        
    }
    
}

void BPM::drawBPMdisplay() {
    
    bpmDisplay = Drawable::createFromImageData (BinaryData::BPMDisplay_svg, BinaryData::BPMDisplay_svgSize);
    bpmDisplayButton.setImages(bpmDisplay);
    addAndMakeVisible(bpmDisplayButton);
    
    labelBPM.setText ("BPM", dontSendNotification);
    labelBPM.attachToComponent (&textBPM, false);
    labelBPM.setColour (Label::textColourId, Colours::BandLoopBackground);
    labelBPM.setJustificationType (Justification::left);
    
    
    labelBPMProperties.set ("fontSize", 30);
    labelBPMProperties.set ("fontStyleFlag", Font::italic);
    addAndMakeVisible (labelBPM);
    
    textBPM.setText ("120", dontSendNotification);
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
    
}

//================= FUNCTIONALITY ====================================================================

void BPM::hiResTimerCallback()
{
    
    if(HighResolutionTimer::getTimerInterval()) {
        timeInMilliseconds ++ ;
    }
    
    
    BPMratio = BPMs/60.;
    
    timeUIfloat = (static_cast<float>(timeInMilliseconds))/1000;
    Bars = timeUIfloat*BPMratio/4;
    BarToDisplay = static_cast<int>(Bars);
    int BarToSend = roundToInt(Bars);
    Bar = (Bars);
    
    Beats = (timeUIfloat)*BPMratio;
    Beat = (static_cast<int>(Beats))%4 + 1;
    
    bar.store(BarToDisplay) ;
    
    // BPM has been changed
    if (BPMratio != previousBPMratio)
    {
        
        updateTempo();
    }
    
    // RightBefore newBar
    if( (isRightBeforeNewBar(Bars)) && justTriggered == false) {
        rightBeforeNewBar();
        
    }
    
    //  newBar
    if( isNewBar(Bars) && justTriggered == true) {
        newBar();
    }
    
    
    
    //    check wheter there is a new Beat
    //   then check if the UI is really to Repaint, improving performances
    //    and play the Metronome
    //  HERE will be probably called the LED function
    
 
    
   
    
}



void BPM::timerCallback() {
    
    
    //    check wheter there is a new Beat
    //   then check if the UI is really to Repaint, improving performances
    //    and play the Metronome
    //  HERE will be probably called the LED function
    if(HighResolutionTimer::getTimerInterval()) {

    if (Beat != previousBeat ){
        hasToBeRepainted();
        playClick(Beat);
    }
    
    previousBeat = Beat;
    previousBPMratio = BPMratio;
    
    }
//    BPMratio = BPMs/60.;
//
//    timeUIfloat = (static_cast<float>(timeInMilliseconds))/1000;
//    Bars = timeUIfloat*BPMratio/4;
//    BarToDisplay = static_cast<int>(Bars);
//    int BarToSend = roundToInt(Bars);
//    Bar = (Bars);
//
//    Beats = (timeUIfloat)*BPMratio;
//    Beat = (static_cast<int>(Beats))%4 + 1;
//
//
//    // Tempo has been changed
//    if (BPMratio != previousBPMratio)
//    {
//        updateTempo();
//    }
//
//    // RightBefore newBar
//    if( (isRightBeforeNewBar(Bars)) && justTriggered == false) {
//        rightBeforeNewBar();
//
//    }
//
//    //  newBar
//    if( isNewBar(Bars) && justTriggered == true) {
//        newBar();
//    }
//
//
//
//    //    check wheter there is a new Beat
//    //   then check if the UI is really to Repaint, improving performances
//    //    and play the Metronome
//    //  HERE will be probably called the LED function
//
//    if (Beat != previousBeat ){
//        hasToBeRepainted();
//        playClick(Beat);
//    }
//
//    previousBeat = Beat;
//    previousBPMratio = BPMratio;
    
}


void BPM::updateTempo() {
    loopUnit = 4./BPMratio;
    Identifier  BPMratio ("BPMratio");
    tree.setProperty(BPMratio, loopUnit, &undoManager);
    
}

void BPM::rightBeforeNewBar() {
    int BarToSend = roundToInt(Bars);
    Identifier  Baring ("bar");
    tree.setProperty(Baring, BarToSend, &undoManager);
    justTriggered = true;
//    DBG("isRightBefore");
}

void BPM::newBar() {
    Identifier  Baring ("bar");
    tree.setProperty(Baring, 0, &undoManager);
    justTriggered = false;
//    DBG("isNewBar");
}

bool BPM::isRightBeforeNewBar(float Bars) {

    return ((fmod (Bars,1)) > 1 - 0.010 && (fmod (Bars,1)) < 1);
    
}

bool BPM::isNewBar(float Bars) {
    
    return ( (fmod (Bars,1)) == 1  || (fmod (Bars,1)) < 0.005);
    
}


void BPM::hasToBeRepainted()  {
    
    for (int i = 0; i< BpmCounters.size(); i++) {
        BpmCounters[i]->setToggleState(false, NotificationType::dontSendNotification);}
 
    BpmCounters[Beat-1]->setToggleState(true, NotificationType::dontSendNotification);
    
    BarToText = static_cast<String>(BarToDisplay);
    BeatToText = static_cast<String>(Beat);
    labelBeat.setText(BeatToText, dontSendNotification);
    labelBar.setText(BarToText, dontSendNotification);
    
    repaint();
    
    
}

void BPM::playClick(int Beat) {
    
    // 1...
    if (Beat == 1) {
        click.flagOne = true;
        click.nextBar();
        auto message = MidiMessage::noteOn (2, 7, 0.8f);
        message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
        //                midiCollector.addMessageToQueue(message);
        
        for (int i  = 0; i < MidiOutput::getDevices().size(); i++) {
            MidiOutput* midiOutput;
            midiOutput = MidiOutput::openDevice(i);
            midiOutput->sendMessageNow (message);
            midiOutput = nullptr;
        }
    }
    
    // ... 2 - 3 - 4
    if (Beat != 1) {
        
        click.flagTwoThreeFour = true;
        click.nextBar();
        
        auto message = MidiMessage::noteOn (2, 8, 0.8f);
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
}



void BPM::changeListenerCallback (ChangeBroadcaster*) {
    
    //Inputs Changed
    headphonesOutputSelector.clear();
    for (int i = 0; i < admInfo.outputsAvailable.size(); i++){
        headphonesOutputSelector.addItem (admInfo.outputsAvailable[i], i+1);
    }
    outputsAvailables = admInfo.outputsAvailable;
    updateOutputs();

    
    
    
}


void BPM::retrieveOutputs(String fromComboBox){
    StringArray tokens;
    
    tokens.addTokens (fromComboBox,"+");
    if(tokens.size() == 1)
        outIsStereo = false;
    else
        outIsStereo = true;
    if(outIsStereo){
        outputL = tokens[0].getIntValue();
        outputR = tokens[2].getIntValue();
    }
    else if (!outIsStereo){
        outputL = tokens[0].getIntValue();
    }
    if(outIsStereo){
        outputL = tokens[0].getIntValue();
        outputR = tokens[2].getIntValue();
    }
    else if (!outIsStereo){
        outputL = tokens[0].getIntValue();
        outputR = 0;
    }
    
    
    
}



void BPM::updateOutputs() {
    String outputLmomentary = String(outputL);
    String outputRmomentary = String(outputR);
    outputL  = outputsAvailables.indexOf(outputLmomentary) + 1;
    outputR  = outputsAvailables.indexOf(outputRmomentary) + 1;
}


void BPM::initialiseOutputSelector() {
    
    headphonesOutputSelector.addListener(this);
    labelHeadphonesOutputSelector.attachToComponent(&headphonesOutputSelector, false);
    labelHeadphonesOutputSelector.setText ("Metronome Output:", dontSendNotification);
    addAndMakeVisible(labelHeadphonesOutputSelector);
    addAndMakeVisible(headphonesOutputSelector);
    outputsAvailables = admInfo.outputsAvailable;
    for (int i = 0; i < admInfo.outputsAvailable.size(); i++){
        headphonesOutputSelector.addItem (admInfo.outputsAvailable[i], i+1);
    }
    
    headphonesOutputSelector.setTextWhenNothingSelected("1+2");
    
}


void BPM::comboBoxChanged(ComboBox* comboBoxThatHasChanged){

if (comboBoxThatHasChanged == &headphonesOutputSelector) {
    String named = headphonesOutputSelector.getText();
    retrieveOutputs(named);
    outputsAvailables = admInfo.outputsAvailable;
    updateOutputs();
    }

}
