/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()

: audioSetupComp (deviceManager,  0,  256,  0, 256,  false,  false,  false, false)

, recordButtonTrack1 { "recordButtonTrack1" }

, lastRecording()

{
    addAndMakeVisible (audioSetupComp);
    deviceManager.addChangeListener (this);
    
  
    
    createBandLoopFolder();
    createTempFolder();
    createSavedFolder();
    
    setSize (800, 600);

    addAndMakeVisible (addTrack);
    addAndMakeVisible (recordButtonTrack1);
    recordButtonTrack1.setColour (TextButton::buttonColourId, Colour (0xffff5c5c));
    recordButtonTrack1.setColour (TextButton::textColourOnId, Colours::black);
    
    recordButtonTrack1.onClick = [this]
    {
        
        if (recording == false) {
    
            
            recording = true;
        }
        else
        {
         
            recording = false;
        }

    };
    
    addTrack.onClick = [this] { createNewTrack();};
    

    addAndMakeVisible (audioSetupComp);
    
    BPM* newComp = new BPM (mainTree, undoManager);
    addAndMakeVisible (newComp);
    BPMS.add (newComp);
    
 
    
    DBG(BPMS.size());
        setAudioChannels (2, 2);

    deviceManager.addAudioCallback (&audioSourcePlayer);
    audioSourcePlayer.setSource(&fTracks[0]);
    
}

MainComponent::~MainComponent()
{
    deviceManager.removeChangeListener (this);
    // This shuts down the audio device and clears the audio source.
    closeAllWindows();
    shutdownAudio();
}


void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{

    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
}


void MainComponent::paint (Graphics& g)
{
    
    BPMS[0]->setBounds(getLocalBounds());
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
   
}

void MainComponent::resized()
{
    
 
// the Rest
    auto area = getLocalBounds();
    audioSetupComp.setBounds (10, 200, 400, 200);
    recordButtonTrack1      .setBounds (area.removeFromTop (36).removeFromLeft (140).reduced (8));
    addTrack.setBounds (area.removeFromTop (36).removeFromLeft (260).reduced (1));
}

void MainComponent::startRecording()

{
    
}

void MainComponent::stopRecording()
{
    lastRecording = File();
    recordButtonTrack1.setButtonText ("Record");
    
    if (pos>0) {
        
        keyboardState.noteOn(1, 74, 0.8);
    }
}

void MainComponent::changeListenerCallback (ChangeBroadcaster*)
{
    bigIntegerInputsAvailable = deviceManager.getCurrentAudioDevice()->getActiveInputChannels();
    toStereo = getListOfActiveBits(bigIntegerInputsAvailable);
    createInputSelections(toStereo);
}


//=========================================================================================
// All the functions needed for setting up the necessary folders in Documents User Folder.
//=========================================================================================

void MainComponent::createBandLoopFolder() {
    auto parentDir = File::getSpecialLocation (File::userDocumentsDirectory).getChildFile("BandLoop");
    File myFolder(parentDir);
    bool b = myFolder.createDirectory();
    Main = myFolder;
}
void MainComponent::createTempFolder() {
    auto parentDir = Main.getChildFile("Temporary");
    File myFolder(parentDir);
    bool b = myFolder.createDirectory();
    TempFolder = myFolder;
}
void MainComponent::createSavedFolder() {
    auto parentDir = Main.getChildFile("Saved");
    File myFolder(parentDir);
    bool b = myFolder.createDirectory();
    SavedFolder = myFolder;
}


//=========================================================================================
//  CREATE TRACK ()
//=========================================================================================
void MainComponent::createNewTrack() {
    
    
    Identifier inputs ("Inputs");
    mainTree.setProperty(inputs, "0", nullptr);
    Identifier name ("name");
    mainTree.setProperty(name, "0", nullptr);
    
    fTracks = new Track("Track",
                        &sliderTrack1,
                        &buttonTrack1,
                        "2",
                        &midiCollector,
                        mainTree,
                        undoManager);
    
    showDocumentWindow(true,  inputTrackToPass,  inputTrackToPass);

}


//=========================================================================================
//  TRACK WINDOW
//=========================================================================================
void MainComponent::showDocumentWindow (bool native, String& inputTrackToPass, String& nameTrackToPass )
{
    auto* dw = new addTrackWindow ("Add Track", getLookAndFeel().findColour (ResizableWindow::backgroundColourId), DocumentWindow::allButtons, inputsAvailable, mainTree,
                                   undoManager);
    windows.add (dw);
    
    Rectangle<int> area (200, 200, 300, 400);
    RectanglePlacement placement ((native ? RectanglePlacement::xLeft
                                   : RectanglePlacement::xRight)
                                  | RectanglePlacement::yTop
                                  | RectanglePlacement::doNotResize);
    auto result = placement.appliedTo (area, Desktop::getInstance().getDisplays()
                                       .getMainDisplay().userArea.reduced (20));
    dw->setBounds (result);
    dw->setResizable (true, ! native);
    dw->setUsingNativeTitleBar (native);
    dw->setVisible (true);

    }


void MainComponent::closeAllWindows()
{
    for (auto& window : windows)
        window.deleteAndZero();
    windows.clear();
}


//=========================================================================================
//  Retrieve INPUTS AVAILABLE
//=========================================================================================
void MainComponent::createInputSelections(Array<int> toStereo) {
    
    inputsAvailable.clear();
    
    for (int i = 0; i < toStereo.size(); i++) {
        
        inputsAvailable.add(static_cast<String>(toStereo[i]));
    }
    
    for (int i = 0; i < toStereo.size(); i++) {
        
        if ( toStereo[i] % 2 != 0 && toStereo[i+1] == toStereo[i]+1) {
            String odd = static_cast<String>(toStereo[i]);
            odd += " + " ;
            odd += static_cast<String>(toStereo[i+1]) ;
            inputsAvailable.add(odd);
            
        }
        
    }
    
}



/* ------------------------------------------------------------------------------------
    Transport  -  BPM  -  Transport  -  BPM  -  Transport  -  BPM  -  Transport  -  BPM
---------------------------------------------------------------------------------------- */
//
//void MainComponent::transportSetup() {
//    
//    playButton.setToggleState(false, NotificationType::dontSendNotification);
//    playButton.setColour(TextButton::ColourIds::buttonColourId, Colours::red);
//    playButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::limegreen);
//    playButton.onClick = [this]() { playTransport(); };
//    playButton.addListener(this);
//    addAndMakeVisible(playButton);
//    
//    
//    labelBPM.setText ("BPM", dontSendNotification);
//    labelBPM.attachToComponent (&textBPM, true);
//    labelBPM.setColour (Label::textColourId, Colours::orange);
//    labelBPM.setJustificationType (Justification::right);
//    addAndMakeVisible (labelBPM);
//    
//    
//    textBPM.setEditable (true);
//    textBPM.setColour (Label::backgroundColourId, Colours::darkblue);
//    addAndMakeVisible (textBPM);
//    
//    textBPM.onTextChange = [this] { BPMs = static_cast <int>(textBPM.getText().getFloatValue());};
//    
//    
//    Timer::startTimer(100);
//    
//
//}
//
//void MainComponent::playTransport()
//{
//    if (playState == PlayState::Stop)
//    {
//        playState = PlayState::Play;
//        playButton.setToggleState(true, NotificationType::dontSendNotification);
//        playButton.setButtonText("Playing");
//        //        edit.getTransport().play(false);
//        HighResolutionTimer::startTimer(1);
//        
//        
//    }
//}
//
//void MainComponent::stopTransport()
//{
//    if (playState == PlayState::Play)
//    {
//        
//        playState = PlayState::Stop;
//        //        edit.getTransport().stop(true, false);
//        playButton.setToggleState(false, NotificationType::dontSendNotification);
//        playButton.setButtonText("Stopped");
//        HighResolutionTimer::stopTimer();
//        timeInMilliseconds = 0;
//        Bar = 0;
//        Beat = 0;
//    }
//}
//
//void MainComponent::hiResTimerCallback()
//{
//    BPMratio = BPMs/60.;
//    
//    if(HighResolutionTimer::getTimerInterval()) {
//        timeInMilliseconds ++ ;
//    }
//    
//    timeUIfloat = (static_cast<float>(timeInMilliseconds))/1000;
//    Bars = timeUIfloat*BPMratio/4;
//    Bar = static_cast<int>(Bars);
//    
//    Beats = (timeUIfloat)*BPMratio;
//    Beat = (static_cast<int>(Beats))%4 + 1;
//    
//    
//}
//
//void MainComponent::buttonClicked (Button* button)
//{
//    if (button == &playButton)
//    {
//        if (playState == PlayState::Play)
//        {
//            playButton.onClick = [&]() { stopTransport(); };
//        }
//        else
//        {
//            playButton.onClick = [&]() { playTransport(); };
//        }
//    }
//}
//
//
//
//void MainComponent::timerCallback() {
//    
//    repaint();
//    
//    
//}
//
