/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()

: audioSetupComp (deviceManager,  0,  256,  0, 256,  false,  false,  false, false)


{
     initialise();
    
     admInfo.reset(new ADMinfo(deviceManager));
    
     trackColourAvailable = coloursOfTracks ;
     deviceManager.addChangeListener (this);
     parentTree.addListener(this);
    
    
    addTrackProperties.set ("fontSize", 40);
    addTrack.setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible (settings);
    addAndMakeVisible (addTrack);
    
    
    addTrack.onClick = [this] { createNewTrack();};
    settings.onClick = [this] {  showSettingWindow(true, parentTree, audioSetupComp);};
   
    globalTempo.reset(new BPM (parentTree, undoManager, *admInfo));
    addAndMakeVisible (globalTempo.get());

    deviceManager.addAudioCallback (&audioSourcePlayerClick);
    audioSourcePlayerClick.setSource(&globalTempo.get()->click);
    deviceManager.addMidiInputCallback ({}, this); // [6]
    
    
    setSize (800, 600);
    
    resized();
    
    
    

    fPlayer.setProcessor(&fGraph);
    deviceManager.addAudioCallback(&fPlayer);
    
    fGraph.clear();
    
    // Create and add new input/output processor nodes.
    AudioProcessorGraph::AudioGraphIOProcessor* in =
    new AudioProcessorGraph::AudioGraphIOProcessor(
                                                   AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
//    fInputNode = fGraph->AddProcessor(in);
    
    AudioProcessorGraph::AudioGraphIOProcessor* out =
    new AudioProcessorGraph::AudioGraphIOProcessor(
                                                   AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
    fOutputNode = this->AddProcessor(out);
    
//    this->Connect(fInputNode, fOutputNode);
}

MainComponent::~MainComponent()
{

// Clear AudioSourcesPlayers and AudioDeviceManager
  audioSourcePlayerClick.setSource(nullptr);
  deviceManager.removeAudioCallback(&audioSourcePlayerClick);
  deviceManager.removeChangeListener (this);

setLookAndFeel (nullptr); // to avoid an error
    
// This shuts down the audio device and clears the audio source.
    closeAllWindows();
    shutdownAudio();
}


void MainComponent::initialise() {
    
    setupSquareLookAndFeelColours();
    
    createBandLoopFolder();
    createTempFolder();
    createSavedFolder();
    
    for (int i = 0 ; i < maxNumberOfPedals; i++ ) {
        
        MidiMessageCollector* newFromPedal = new MidiMessageCollector;
        //    newFromPedal.reset();
        fromPedals.add(newFromPedal);
        
    }
    

    midiResetting();
    setAudioChannels (2, 2);
    
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    for (int i = 0; i < fromPedals.size(); i++)
        fromPedals[i]->reset(sampleRate);
    
    
    sampleHz = sampleRate;

    
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
   
    
//    midiResetting(bufferToFill);
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
    
    auto area = getLocalBounds();
    
    int menuEight = 20;
    int usualSpace = 20;
    int trackWidth = 275;
    
  
    globalTempo.get()->setBounds(area.getWidth()/6, menuEight, area.getWidth()/1.5, area.getHeight()/8);
    
    audioSetupComp.setBounds (10, 200, 400, 200);
    addTrack.setBounds (getWidth()-75, getHeight()-75, 75, 75);
    settings.setBounds (0, getHeight()-30, 60, 30);
    
    Rectangle<int> bounds {0, getHeight()/2, getWidth(), getHeight()/2};

    for(int i =0 ; i < fTracks.size(); i++) {
        
        fTracks[i]->setBounds(i*(trackWidth+usualSpace) + usualSpace, area.getHeight()/8+menuEight+usualSpace , 275, area.getHeight()-(area.getHeight()/8+menuEight+usualSpace) - 85);
        deleteTrackButtons[i]->setBounds(i*(trackWidth+usualSpace)+255+ usualSpace, area.getHeight()/8+menuEight+usualSpace-5, 30, 30);

    }


}


void MainComponent::changeListenerCallback (ChangeBroadcaster*)
{

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
    
    Track* newTrack = new Track(*trackColourAvailable.begin(),
                                "Track N^" + String(numOfTrees),
                                fromPedals,
                                pedalsAvailables, 
                                createNewChildTree(),
                                undoManager,
                                *admInfo);
    addAndMakeVisible(newTrack);
    fTracks.add (newTrack);
    parentTree.addListener(newTrack);
    
    createDeleteButton() ;

    trackColourAssigned.add(trackColourAvailable[0]);
    trackColourAvailable.remove(0);
    
    resized();

    
}

ValueTree MainComponent::createNewChildTree() {
    
    String identifier = "Children n*" + String(numOfTrees);
    numOfTrees ++;
    Identifier treeIdentifier (identifier);
    ValueTree newTree (treeIdentifier);
    parentTree.appendChild(newTree, &undoManager);
    
    return newTree ;
    
}


void MainComponent::createDeleteButton() {
    
    DrawableButton* newDeleteTrackButton = new DrawableButton(trackColourAvailable[0], DrawableButton::ImageFitted);
    newDeleteTrackButton->setImages(deleteTrackImage);
    addAndMakeVisible(newDeleteTrackButton);
    deleteTrackButtons.add(newDeleteTrackButton);
    newDeleteTrackButton->addListener(this);
    
}

//=========================================================================================
//  DELETE TRACK ()
//=========================================================================================
void MainComponent::deleteTrack(String trackToDelete) {

    for (int i = 0; i < fTracks.size(); i++) {
        
        if(fTracks[i]->getColour() == trackToDelete) {
            
            trackColourAssigned.removeString(trackToDelete);
            trackColourAvailable.add(trackToDelete);
            
            parentTree.removeChild(fTracks[i]->tree, &fTracks[i]->undoManager);
            parentTree.removeListener(fTracks[i]);
            
            fTracks.remove(i, true);
            deleteTrackButtons.remove(i, true);
            
        }
        
    }
    
    resized();
    
}


//=========================================================================================
//  SETTINGS window
//=========================================================================================

void MainComponent::showSettingWindow(bool native, const ValueTree& newTree, AudioDeviceSelectorComponent& audioSetupComp){
    
    auto* dw = new SettingWindow ("Settings", Colours::BandLoopBackground, DocumentWindow::allButtons, newTree,
                                   undoManager, deviceManager, pedalsAvailables, midiCollector);
    windows.add (dw);
    
    dw->setCentreRelative(0.5, 0.5);
    dw->setSize(400, 600);
    dw->setResizable (false, ! native);
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
//  GLOBAL TEMPO + QUANTIZE (achieved thought ValueTREE)
//=========================================================================================

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

//=========================================================================================
//  Midi from Pedals
//=========================================================================================

void MainComponent::handleIncomingMidiMessage (MidiInput* /*source*/,
                                const MidiMessage& message)
{
  

// Pedals ---->  Software
    
  if (message.isForChannel(1)) {
    
      if( message.getNoteNumber() > 9 ) {
        
          
            for (int i = 0; i < maxNumberOfPedals ; i++) {
        
                
                if (message.getNoteNumber() < (i+1) * 10  && message.getNoteNumber() > i*10 ) {
                    if(message.isNoteOn()) {
                    auto messageToPedal = MidiMessage::noteOn (1, message.getNoteNumber()- (i*10), 0.8f);
                        
                        
                    messageToPedal.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
                        DBG("note in Component:");
                        DBG(String(message.getNoteNumber()));
            fromPedals[i]->addMessageToQueue(messageToPedal);
                     }
                    }
                }
            }

        
      if( message.getNoteNumber() < 6 ) {
          
//         midiSetting.addMessageToQueue(message.getNoteNumber());
          
          if(!pedalsAvailables.contains(message.getNoteNumber())) {
          
          pedalsAvailables.add( message.getNoteNumber());
              
          }
          
          pedalsAvailables.sort();
          
          for (int i =0; i < pedalsAvailables.size() ; i++) {

              
          }
      }
      
      numberPedalClicked(message.getNoteNumber());
      
      for (int i = 0; i < fTracks.size(); i ++) {
          
          fTracks[i]->updateGuiAndPedals();
      }
        
  }
    
// Software ---->  Pedals

    
//    if(message.getNoteNumber ()>30 && message.getNoteNumber ()<35) {
//        for(int i= 0; i < fTracks.size(); i++ ) {
//            if ( fTracks[i]->getColour() ==  coloursOfTracks[0])
//                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
//    }
//
//    if(message.getNoteNumber ()>35 && message.getNoteNumber ()<41) {
//        for(int i= 0; i < fTracks.size(); i++ ) {
//            if ( fTracks[i]->getColour() ==  coloursOfTracks[1])
//                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
//    }
//
//    if(message.getNoteNumber ()>42 && message.getNoteNumber ()<47) {
//        for(int i= 0; i < fTracks.size(); i++ ) {
//            if ( fTracks[i]->getColour() ==  coloursOfTracks[2])
//                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
//    }
//
//    if(message.getNoteNumber ()>48 && message.getNoteNumber ()<53) {
//        for(int i= 0; i < fTracks.size(); i++ ) {
//            if ( fTracks[i]->getColour() ==  coloursOfTracks[3])
//                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
//    }
//
//    if(message.getNoteNumber ()>54 && message.getNoteNumber ()<59) {
//        for(int i= 0; i < fTracks.size(); i++ ) {
//            if ( fTracks[i]->getColour() ==  coloursOfTracks[4])
//                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
//    }
//
//    if(message.getNoteNumber ()>60 && message.getNoteNumber ()<65) {
//        for(int i= 0; i < fTracks.size(); i++ ) {
//            if ( fTracks[i]->getColour() ==  coloursOfTracks[5])
//                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
//    }
//
//    if(message.getNoteNumber ()>66 && message.getNoteNumber ()<71) {
//        for(int i= 0; i < fTracks.size(); i++ ) {
//            if ( fTracks[i]->getColour() ==  coloursOfTracks[6])
//                fTracks[i]->midiCollectorFromPedals.addMessageToQueue (message);}
//    }
    
}


void MainComponent::buttonClicked(Button* button) {

    for (int i = 0; i < deleteTrackButtons.size(); i++) {
    
        if (button == deleteTrackButtons[i]){
            
            String textToCancel = deleteTrackButtons[i]->getButtonText();
            
            deleteTrack(textToCancel);
   
        }

    }
};


//=========================================================================================
//  set LookAndFeel and Colours
//=========================================================================================

void MainComponent::setupSquareLookAndFeelColours ()
{
//    LookAndFeel::setDefaultLookAndFeel(&customLookAndFeel);
    setLookAndFeel (&customLookAndFeel2
                    );
    
    customLookAndFeel.setColour (Slider::thumbColourId, Colour::greyLevel (0.95f));
    customLookAndFeel.setColour (Slider::textBoxOutlineColourId,Colours::transparentWhite);
    customLookAndFeel.setColour (Slider::rotarySliderFillColourId, Colour (0xff00b5f6));
    customLookAndFeel.setColour (Slider::rotarySliderOutlineColourId, Colours::white);
    customLookAndFeel.setColour (TextButton::buttonColourId, Colours::BandLoopText);
    customLookAndFeel.setColour (TextButton::textColourOffId,Colours::BandLoopText);
    customLookAndFeel.setColour (TextButton::textColourOnId, Colours::BandLoopText);
    customLookAndFeel.setColour (TextButton::buttonOnColourId, Colours::BandLoopBackground);
    customLookAndFeel.setColour (Label::textColourId, Colours::BandLoopText);
    
    customLookAndFeel.setColour (ComboBox::backgroundColourId, Colours::BandLoopText);
    customLookAndFeel.setColour (ComboBox::textColourId,Colours::BandLoopText);
    customLookAndFeel.setColour (ComboBox::buttonColourId, customLookAndFeel.findColour(TextButton::textColourOffId));
    customLookAndFeel.setColour (ComboBox::arrowColourId, Colours::BandLoopText);
    customLookAndFeel.setColour (ComboBox::focusedOutlineColourId,Colours::BandLoopText);
   

    }




void MainComponent::midiResetting() {
//    
//    MidiBuffer midiAvailableNotes ;
//    
//    midiSetting.removeNextBlockOfMessages(midiAvailableNotes, bufferToFill.numSamples);
//
//    for (int i = 0; i < fromPedals.size(); i++)
//        fromPedals[i]->reset(sampleHz);
    
}


void MainComponent::numberPedalClicked (int note) {
    
    for (int i = 0; i < windows.size(); i++) {
    
        windows[i]->pedalClicked( pedalsAvailables.indexOf(round(note/10)));
    }
        
}


NodeId MainComponent::AddProcessor(AudioProcessor* p)
{
//    AudioProcessorGraph::Node* node;
//    node = fGraph.addNode(p,node->nodeID);
//    return node->nodeID;
}
