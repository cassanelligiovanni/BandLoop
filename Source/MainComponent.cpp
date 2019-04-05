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
    
    
//    background_image = Drawable::createFromImageData (BinaryData::PedalShape3_svg, BinaryData::PedalShape3_svgSize);
//
//
//
//    addAndMakeVisible (background_image);
    
//    customLookAndFeel.setDefaultSansSerifTypefaceName ("CoreText");
    LookAndFeel::setDefaultLookAndFeel(&customLookAndFeel);
 
    setupSquareLookAndFeelColours();
    setLookAndFeel (&customLookAndFeel);
    
    
    
     trackColourAvailable = coloursOfTracks ;

//    addAndMakeVisible (audioSetupComp);
    deviceManager.addChangeListener (this);
    
    parentTree.addListener(this);
    
    createBandLoopFolder();
    createTempFolder();
    createSavedFolder();
    
    setSize (800, 600);

    addAndMakeVisible (settings);
    addAndMakeVisible (addTrack);


    
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
    settings.onClick = [this] {  showSettingWindow(true, parentTree, audioSetupComp);};
   
    

    BPM* newComp = new BPM (parentTree, undoManager);
    addAndMakeVisible (newComp);
    BPMS.add (newComp);
    
    resized();
    
        setAudioChannels (2, 2);

    deviceManager.addAudioCallback (&audioSourcePlayer);
    deviceManager.addAudioCallback (&audioSourcePlayer2);
    
    deviceManager.addMidiInputCallback ({}, this); // [6]
    
}

MainComponent::~MainComponent()
{
    setLookAndFeel (nullptr);

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
  
    
    g.fillAll (Colours::BandLoopBackground);

   
}

void MainComponent::resized()
{
    
 
// the Rest
//    background_image->setBounds (getLocalBounds());
    auto area = getLocalBounds();
    
    int menuEight = 20;
    int usualSpace = 10;
    int trackWidth = 275;
//    Z
    
    if(!BPMS.isEmpty())
    BPMS.getLast()->setBounds(area.getWidth()/4, menuEight, area.getWidth()/2, area.getHeight()/8);
    
    audioSetupComp.setBounds (10, 200, 400, 200);
    addTrack.setBounds (getWidth()-100, getHeight()-100, 100, 100);
    settings.setBounds (0, getHeight()-20, 50, 20);
    
    Rectangle<int> bounds {0, getHeight()/2, getWidth(), getHeight()/2};

    for(int i =0 ; i < fTracks.size(); i++) {
        
        fTracks[i]->setBounds(i*(trackWidth+usualSpace) + usualSpace, area.getHeight()/8+menuEight+usualSpace , 275, area.getHeight()-220);
        deleteTrackButtons[i]->setBounds(i*(trackWidth+usualSpace)+255+ usualSpace, area.getHeight()/8+menuEight+usualSpace-5, 30, 30);

    }


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
    
    String identifier = "Children n*" + String(numOfTrees);
    numOfTrees ++;
    Identifier treeIdentifier (identifier);
    ValueTree newTree (treeIdentifier);
    parentTree.appendChild(newTree, &undoManager);
    
    Identifier inputs ("Inputs");
    newTree.setProperty(inputs, "0", &undoManager);
    Identifier name ("Name");
    newTree.setProperty(name, "0", &undoManager);
    
    MidiMessageCollector* newFromPedal = new MidiMessageCollector;
    fromPedals.add(newFromPedal);
    
    Track* newTrack = new Track(trackColourAvailable[0],
                             &sliderTrack1,
                             &buttonTrack1,
                             "2",
                                *fromPedals.getLast(),
                             newTree,
                             undoManager);
    addAndMakeVisible(newTrack);
    
    trackColourAssigned.add(trackColourAvailable[0]);
    
    DrawableButton* newDeleteTrackButton = new DrawableButton(trackColourAvailable[0], DrawableButton::ImageFitted);
    newDeleteTrackButton->setImages(deleteTrackImage);
    addAndMakeVisible(newDeleteTrackButton);
    deleteTrackButtons.add(newDeleteTrackButton);
    newDeleteTrackButton->addListener(this);
    
    

    trackColourAssigned.add(trackColourAvailable[0]);
    trackColourAvailable.remove(0);
    
    fTracks.add (newTrack);
    
    AudioSourcePlayer* newAudioSourcePlayer = new AudioSourcePlayer;
    audioSourcePlayers.add(newAudioSourcePlayer);
    audioSourcePlayers.getLast()->setSource(fTracks.getLast());
    deviceManager.addAudioCallback(audioSourcePlayers.getLast());
    
    
    parentTree.addListener(fTracks.getLast());
    
    showDocumentWindow(true,  newTree);
    
    resized();

    
}

void MainComponent::deleteTrack(String trackToDelete) {

    for (int i = 0; i < fTracks.size(); i++) {
        
        
        if(fTracks[i]->getColour() == trackToDelete) {
            
          
//            parentTree.removeChild (fTracks[i]->tree, &fTracks[i]->undoManager);
            trackColourAssigned.removeString(trackToDelete);
            trackColourAvailable.add(trackToDelete);
            
    
            
            
            for (int j = 0; j < audioSourcePlayers.size(); j++) {
                
                if(audioSourcePlayers[j]->getCurrentSource() == fTracks[i]) {
                    
                    deviceManager.removeAudioCallback(audioSourcePlayers[j]);
                    audioSourcePlayers.remove(j, true);
                    
                }
                
            }
            
            
            
            parentTree.removeChild(fTracks[i]->tree, &fTracks[i]->undoManager);
            parentTree.removeListener(fTracks[i]);
            
            fTracks.remove(i, true);
            deleteTrackButtons.remove(i, true);
            
            
        }
        
        
    }
    
    resized();
    
}

//=========================================================================================
//  TRACK WINDOW
//=========================================================================================
void MainComponent::showDocumentWindow (bool native, const ValueTree& newTree)
{
    auto* dw = new addTrackWindow ("Add Track", Colours::BandLoopBackground, DocumentWindow::allButtons, inputsAvailable, newTree, undoManager);
    windows.add (dw);
    
    Rectangle<int> area (200, 200, 300, 400);
    RectanglePlacement placement ((native ? RectanglePlacement::xLeft
                                   : RectanglePlacement::xRight)
                                  | RectanglePlacement::yTop
                                  | RectanglePlacement::doNotResize);
    auto result = placement.appliedTo (area, Desktop::getInstance().getDisplays()
                                       .getMainDisplay().userArea.reduced (20));
//    dw->setSize(200, 350);
    dw->setBounds (result);
    dw->setResizable (false, ! native);
    dw->setCentreRelative (0.5, 0.5);
    dw->setUsingNativeTitleBar (native);
    dw->setVisible (true);
    }

//=========================================================================================
//  SETTINGS window
//=========================================================================================

void MainComponent::showSettingWindow(bool native, const ValueTree& newTree, AudioDeviceSelectorComponent& audioSetupComp){
    
    auto* dw = new SettingWindow ("Settings", Colours::BandLoopBackground, DocumentWindow::allButtons, newTree,
                                   undoManager, deviceManager);
    windows.add (dw);
    
    Rectangle<int> area (200, 200, 400, 200);
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



void MainComponent::valueTreeChildAdded (ValueTree& parentTree, ValueTree&) {};
void MainComponent::valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) {};
void MainComponent::valueTreeChildOrderChanged (ValueTree& parentTree, int, int) {};
void MainComponent::valueTreeParentChanged (ValueTree&) {};

void MainComponent::valueTreePropertyChanged (ValueTree& tree, const Identifier& property) {
 
    Identifier  BPMratio ("BPMratio");
    if (property == BPMratio ) {
        float pair = float(parentTree.getProperty(BPMratio));
    
    }
    
    Identifier  Beating ("beat");
    float beating = parentTree.getProperty(Beating);
    
    
    Identifier  Baring ("bar");
    float baring = parentTree.getProperty(Baring);
    
    if(tree == parentTree && property == Baring) {
    
       
        if((float(tree.getProperty(Baring))) > 0){
            for( int i = 0; i<fTracks.size(); i++) {
                fTracks[i]->checkEvent((float(tree.getProperty(Baring))));
            }
        }
        
        
        if(tree.getProperty(Baring).equals(0
                                           )){
            for( int i = 0; i<fTracks.size(); i++)
                fTracks[i]->triggerEvent();
        }
  }
}


void MainComponent::handleIncomingMidiMessage (MidiInput* /*source*/,
                                const MidiMessage& message)
{
    if(message.getNoteNumber ()>30 && message.getNoteNumber ()<35) {
        for(int i= 0; i < fTracks.size(); i++ ) {
            if ( fTracks[i]->getColour() ==  coloursOfTracks[0])
                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
    }
    
    if(message.getNoteNumber ()>35 && message.getNoteNumber ()<41) {
        for(int i= 0; i < fTracks.size(); i++ ) {
            if ( fTracks[i]->getColour() ==  coloursOfTracks[1])
                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
    }
    
    if(message.getNoteNumber ()>42 && message.getNoteNumber ()<47) {
        for(int i= 0; i < fTracks.size(); i++ ) {
            if ( fTracks[i]->getColour() ==  coloursOfTracks[2])
                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
    }
    
    if(message.getNoteNumber ()>48 && message.getNoteNumber ()<53) {
        for(int i= 0; i < fTracks.size(); i++ ) {
            if ( fTracks[i]->getColour() ==  coloursOfTracks[3])
                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
    }
    
    if(message.getNoteNumber ()>54 && message.getNoteNumber ()<59) {
        for(int i= 0; i < fTracks.size(); i++ ) {
            if ( fTracks[i]->getColour() ==  coloursOfTracks[4])
                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
    }
    
    if(message.getNoteNumber ()>60 && message.getNoteNumber ()<65) {
        for(int i= 0; i < fTracks.size(); i++ ) {
            if ( fTracks[i]->getColour() ==  coloursOfTracks[5])
                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
    }
    
    if(message.getNoteNumber ()>66 && message.getNoteNumber ()<71) {
        for(int i= 0; i < fTracks.size(); i++ ) {
            if ( fTracks[i]->getColour() ==  coloursOfTracks[6])
                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
    }
    
}


void MainComponent::buttonClicked(Button* button) {

    for (int i = 0; i < deleteTrackButtons.size(); i++) {
    
        if (button == deleteTrackButtons[i]){
            
            String textToCancel = deleteTrackButtons[i]->getButtonText();
            
            deleteTrack(textToCancel);
            
            
        }
        
        }
};















void MainComponent::setupSquareLookAndFeelColours ()
{
    
    customLookAndFeel.setColour (Slider::thumbColourId,               Colour::greyLevel (0.95f));
    customLookAndFeel.setColour (Slider::textBoxOutlineColourId,      Colours::transparentWhite);
    customLookAndFeel.setColour (Slider::rotarySliderFillColourId,    Colour (0xff00b5f6));
    customLookAndFeel.setColour (Slider::rotarySliderOutlineColourId, Colours::white);
    
    customLookAndFeel.setColour (TextButton::buttonColourId,  Colours::white);
    customLookAndFeel.setColour (TextButton::textColourOffId, Colours::darkgrey);
    
    customLookAndFeel.setColour (TextButton::buttonOnColourId, customLookAndFeel.findColour (TextButton::textColourOffId));
    customLookAndFeel.setColour (TextButton::textColourOnId,   Colours::darkgrey);
    }
