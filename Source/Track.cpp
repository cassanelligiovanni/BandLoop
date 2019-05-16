/*
  ==============================================================================

    Track.cpp
    Created: 11 Mar 2019 12:56:15pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "Track.h"


Track::Track(String trackColour,
             String inputs,
              OwnedArray<MidiMessageCollector>& fromPedals, Array<int>& pedalsAvailables,
             const ValueTree& v,
             UndoManager& um,  ADMinfo& AdmInfo )
: tree (v)
, midiCollectorFromPedals (fromPedals)
, undoManager (um)
, recorder()
, admInfo(AdmInfo)
, pedalAvailable(pedalsAvailables)
, playback(recordState)

{
    
    
    setLookAndFeel(&customLookAndFeel);
    
     colour = trackColour;

     initialise();
    
     initialiseMeter();
     initialiseSlider();
     initialiseInputSelector();
     initialiseOutputSelector();
     initialisePedalSelector();
     initialiseNameOfTrack();
     initialiseButton();
    
    startTimer(100);

     resized();
}

    Track::~Track(){
    
        setLookAndFeel(nullptr);
        admInfo.removeAudioCallback(&audioSourcePlayer);
        audioSourcePlayer.setSource(nullptr);
        decibelSlider.setLookAndFeel(nullptr);
        meter.setLookAndFeel (nullptr);
    }


//=============================================================================
    void Track::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {

        levelMeterSource.resize (2, 200* 0.001f * sampleRate / samplesPerBlockExpected);
        
        bufferToRecordL.setSize(1, samplesPerBlockExpected);
        bufferToRecordR.setSize(1, samplesPerBlockExpected);
        bufferToRecordStereo.setSize(2, samplesPerBlockExpected);
        
         bufferOutL.setSize(1, samplesPerBlockExpected);
         bufferOutR.setSize(1, samplesPerBlockExpected);
         bufferOutStereo.setSize(2, samplesPerBlockExpected);
        
        recorder.setSampleRate(sampleRate);
        playback.prepareToPlay(samplesPerBlockExpected, sampleRate);
        
        for (int i = 0; i < midiCollectorFromPedals.size(); i ++ ) {
            
            midiCollectorFromPedals[i]->reset(sampleRate);
            
        }
        
    }

    void Track::releaseResources() {}

    void Track::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {

        MidiBuffer incomingMidi;
 
        if (nPedal) {
            
            midiCollectorFromPedals[nPedal]->removeNextBlockOfMessages(midiBufferFromPedal, bufferToFill.numSamples);
            
            MidiBuffer::Iterator iterator (midiBufferFromPedal);
            MidiMessage message;
            int sampleNumber = 0;
            int note ;
            while (iterator.getNextEvent (message, sampleNumber)) // [5]
            {
                message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);; // [7]
                note = message.getNoteNumber();
                
                lastMidiNote = note;
            
            }
            
        }
        
    
        int numSamples = bufferToFill.numSamples ;
        
        if (inputL){
                        if(!isStereo){

            //copy the desired content(channel) from bufferToFill and pass it to the recorder
             const auto numSamples = bufferToFill.buffer->getNumSamples();

                bufferToRecordL.copyFrom(0, 0, *bufferToFill.buffer, (inputL - 1), 0, numSamples);

                bufferToRecordL.applyGain(0, 0, numSamples, sliderValue.load());
                            
                bufferOutL.copyFrom(0, 0, bufferToRecordL, 0, 0, numSamples);
                            
        float* pointers [2] = { bufferToRecordL.getWritePointer (0, bufferToFill.startSample),
                                bufferToRecordL.getWritePointer (0, bufferToFill.startSample) };

                            if(!(recordState == RecordState::Stopped || recordState == RecordState::Playing ))                    recorder.Record(pointers, numSamples);

                bufferToFill.clearActiveBufferRegion();

                            if(!outIsStereo){
                                
                                playback.getNextAudioBlock(bufferOutL, numSamples);
                                bufferOutL.applyGain(0, 0, numSamples, sliderValue.load());
                                
                                levelMeterSource.measureBlock(bufferOutL);
                                
                                // non sara 0 ma la variabile output
                                 bufferToFill.buffer->addFrom((outputL-1), 0, bufferOutL, 0, 0, numSamples);
                            }
                            
                            if(outIsStereo){
                            
                                playback.getNextAudioBlock(bufferOutL, numSamples);
                                
                                bufferOutL.applyGain(0, 0, numSamples, sliderValue.load());
                                
                                levelMeterSource.measureBlock(bufferOutL);
                                
                                bufferToFill.buffer->addFrom((outputL-1), 0, bufferOutL, 0, 0, numSamples);
                                bufferToFill.buffer->addFrom((outputR-1), 0, bufferOutL, 0, 0, numSamples);
                                
                            }

        }
            if(isStereo){

                //copy the desired content(channel) from bufferToFill and pass it to the recorder
                const auto numSamples = bufferToFill.buffer->getNumSamples();
                bufferToRecordL.copyFrom(0, 0, *bufferToFill.buffer, (inputL - 1), 0, numSamples);
                bufferToRecordR.copyFrom(0, 0, *bufferToFill.buffer, (inputR - 1), 0, numSamples);

                bufferToFill.clearActiveBufferRegion();

                bufferToRecordStereo.copyFrom(0, 0, bufferToRecordL, 0, 0, numSamples);
                bufferToRecordStereo.copyFrom(1, 0, bufferToRecordR, 0, 0, numSamples);

                bufferToRecordStereo.applyGain(0, 0, numSamples, sliderValue.load());
                bufferToRecordStereo.applyGain(1, 0, numSamples, sliderValue.load());
                
                bufferOutStereo.copyFrom(0, 0, bufferToRecordStereo, 0, 0, numSamples);
                bufferOutStereo.copyFrom(1, 0, bufferToRecordStereo, 1, 0, numSamples);

                
        float* pointers [2] = { bufferToRecordStereo.getWritePointer (0, bufferToFill.startSample),
                                bufferToRecordStereo.getWritePointer (1, bufferToFill.startSample) };
                
                if (!(recordState == RecordState::Stopped || recordState == RecordState::Playing ))
                    recorder.Record(pointers, numSamples);
            
                
                if(!outIsStereo){
                    
                    playback.getNextAudioBlock(bufferOutStereo, numSamples);
                  
                    bufferOutStereo.applyGain(0, 0, numSamples, sliderValue.load());
                    bufferOutStereo.applyGain(1, 0, numSamples, sliderValue.load());
                    
                    levelMeterSource.measureBlock(bufferOutStereo);
                    
                    bufferToFill.buffer->addFrom((outputL-1), 0, bufferOutStereo, 0, 0, numSamples);
                     bufferToFill.buffer->addFrom((outputL-1), 0, bufferOutStereo, 1, 0, numSamples);
                }
                
                if(outIsStereo){
                    
                    playback.getNextAudioBlock(bufferOutStereo, numSamples);
                    
                    bufferOutStereo.applyGain(0, 0, numSamples, sliderValue.load());
                    bufferOutStereo.applyGain(1, 0, numSamples, sliderValue.load());
                    
                    levelMeterSource.measureBlock(bufferOutStereo);
                    
                    bufferToFill.buffer->addFrom((outputL-1), 0, bufferOutStereo, 0, 0, numSamples);
                    bufferToFill.buffer->addFrom((outputR-1), 0, bufferOutStereo, 1, 0, numSamples);
                    
                }
            
            }
            
    

        }
        
         else {
             
                          bufferToFill.clearActiveBufferRegion();
             
         }
        
//        if( recordState == RecordState::Playing) {DBG("PLAYING");};
//        if (recordState == RecordState::Recording) {DBG("RECORDING");};
//        if( recordState == RecordState::isStartingPlaying) {DBG("isStartingPlaying");};
//        if (recordState == RecordState::isStartingRecording) {DBG("isStartingRecording");};
//        if( recordState == RecordState::isStartingStopping) {DBG("isStartingStopping");};
//        if (recordState == RecordState::Stopped) {DBG("Stopped");};

    }
        
       

    // ===============   COMPONENT Methods  ==================================

void Track::paint (Graphics& g)
{
    
        Rectangle<int> bounds= getLocalBounds().removeFromTop(getHeight()-20);
    Path pedal;
    pedal.addRoundedRectangle(bounds, 20, 20);
  
    
    float x1 = bounds.getX();
    float y1 = bounds.getY()+bounds.getHeight()/2;
    float x4 = bounds.getWidth();

    ColourGradient gradient (Colours::findColourForName(colour, Colours::BandLoopBackground).darker(0.30), x1, y1,
                               Colours::findColourForName(colour, Colours::BandLoopBackground).darker(0.30), x4, y1, false);

    gradient.addColour(0.9, Colours::findColourForName(colour, Colours::BandLoopBackground));
    gradient.addColour(0.1, Colours::findColourForName(colour, Colours::BandLoopBackground));

    g.setGradientFill (gradient);
    g.fillPath (pedal);
    
    Rectangle<int> gradientLabelBounds{bounds.getWidth()/4, bounds.getHeight()*9/10, bounds.getWidth()/2, bounds.getHeight()/10} ;

}

void Track::resized()
{
    Rectangle<int> bounds= getLocalBounds().removeFromTop(getHeight()-20);
    Path pedal;
    pedal.addRoundedRectangle(bounds, 20, 20);
    
    selectPedals.setBounds(20, bounds.getHeight()*(14./16.), (bounds.getWidth()/ 3)-40, 20);
////    selectPedals.setBounds(20, 5, (getWidth()-60)/4, 20);
    selectInputs.setBounds(bounds.getWidth()/ 3, bounds.getHeight()*(14./16.), (bounds.getWidth()/ 2.5)-40, 20);
    selectOutputs.setBounds((bounds.getWidth()/ 3)*2, bounds.getHeight()*(14./16.), (bounds.getWidth()/ 2.5)-40, 20);

    
    decibelSlider.setBounds(bounds.getWidth()/2 -75, bounds.getHeight()*(4./16.), 60, bounds.getHeight()/5.*3-20) ;
    meter.setBounds(bounds.getWidth()/2 +15, bounds.getHeight()*(4./16.)+ 30, 60, bounds.getHeight()/5.*3.- 50 );
    
    labelNameOfTrack.setBounds(bounds.getWidth()/4, bounds.getHeight()*9.8/10, bounds.getWidth()/2, 30);
    
    lightPressableButton->setBounds( (bounds.getWidth()/6)*3, (bounds.getHeight()*(2./16.)), 30, 30);
    DrawableRecordButton.setBounds((bounds.getWidth()/6), (bounds.getHeight()*(2./16.)),30, 30);
    playback.setBounds((bounds.getWidth()/6)*2, (bounds.getHeight()*(2./16.)), 30, 30);
    
    
    stopImmediatellyButton.setBounds(140, 5, (getWidth()-60)/4, 20);
//    undoButton.setBounds(20, 5, (getWidth()-60)/4, 20);
    saveButton.setBounds(20, 5, (getWidth()-60)/4, 20);
    newSongButton.setBounds(80, 5, (getWidth()-60)/4, 20);
}


//=============================================================================

void Track::buttonClicked (Button* button) {
    
    if (button == &DrawableRecordButton)
    {
        
        firstFootSwitch();
//        if (recordState == RecordState::Playing)
//        {
//            DrawableRecordButton.onClick = [&]() { isStartingRecording = true ; };
//        }
//        else if (recordState == RecordState::Recording && isStoppingRecording )
//        {
//            DrawableRecordButton.onClick = [&]() { isStartingRecording = true ;};
//        }
//        else
//        {
//            DrawableRecordButton.onClick = [&]() { isStoppingRecording = true; };
//        }
    }
    
    if (button == &stopImmediatellyButton){ secondFootSwitch();};
        if (button == &undoButton){ thirdFootSwitch();};
            if (button == &saveButton) { fourthFootSwitch();};
                if (button == &newSongButton)  { sixthFootSwitch();};
};


void Track::sliderValueChanged(Slider* slider) {

};

void Track::labelTextChanged (Label *labelThatHasChanged){
    
    nameOfTrack = labelNameOfTrack.text.getText();
    selectFolder(nameOfTrack);
    
}

//create a folder named as the Track;
//=============================================================================
void Track::selectFolder(String name) {
    
    oldFolder = actualFolder ;
    
auto parentDir = File::getSpecialLocation (File::userDocumentsDirectory).getChildFile("BandLoop").getChildFile("Temporary").getChildFile(name);
    
    if(parentDir.isDirectory()) {
    actualFolder = parentDir.getNonexistentSibling();
    }
    else if(!parentDir.isDirectory()) {
        actualFolder = parentDir;
    }
    
    nameOfTrack = actualFolder.getFileName();
    bool b = actualFolder.createDirectory();
    
    
//    return myFolder;
}



// TREE callbacks;
//=============================================================================
void Track::valueTreeChildAdded (ValueTree& parentTree, ValueTree&) {};
void Track::valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) {};
void Track::valueTreeChildOrderChanged (ValueTree& parentTree, int, int) {};
void Track::valueTreeParentChanged (ValueTree& parentTreeChanged) {
    
};

void Track::valueTreePropertyChanged (ValueTree& treeChanged, const Identifier& property) {
    
    Identifier  BPMratio ("BPMratio");
    if (property == BPMratio ) {
         loopUnit = float(treeChanged.getProperty(BPMratio));
    }
    
    
// Check Tempo
     Identifier Baring = "bar";
    if (property == Baring ) {
    String pair = tree.getParent().getProperty(Baring);

    }

// Retrieving the name of the track
    Identifier name = "Name";
    if (property == name && treeChanged == tree) {
        String named = treeChanged.getProperty(name);
    nameOfTrack = named;
        selectFolder(nameOfTrack);
    }

    
};

String Track::getColour() {
    return colour;
    
}

void Track::checkEvent(int bar) {
  
    playback.checkEvent(bar);

    if (recordState == RecordState::isStartingRecording && lastRecording.existsAsFile()){
        recorder.stopRecording();
        if ( lastRecording.getParentDirectory() == actualFolder) {
        playback.createSound(lastRecording, actualFolder, loopUnit);
       
        //        isStoppingRecording = false;
      }
        
        if ( lastRecording.getParentDirectory() != actualFolder) {
            playback.createSound(lastRecording, oldFolder, loopUnit);
            
            //        isStoppingRecording = false;
        }
        
    }
    
    if (recordState == RecordState::isStartingPlaying && lastRecording.existsAsFile()){
        recorder.stopRecording();
        playback.createSound(lastRecording, actualFolder, loopUnit);
        recordState = RecordState::Playing;
//        isStoppingRecording = false;
    }
    
    if(  recordState == RecordState::isStartingStopping && lastRecording.existsAsFile()) {
        
        recorder.stopRecording();
        playback.createSound(lastRecording, actualFolder, loopUnit);
        recordState = RecordState::Stopped;
        
    }
    
}
void Track::triggerEvent() {
 
//    routeMidiMessage();
    
    if (recordState == RecordState::isStartingRecording){
        lastRecording = actualFolder.getNonexistentChildFile(nameOfTrack, ".wav");
        recorder.startRecording(lastRecording);
        recordState = RecordState::Recording;

//        isStartingRecording = false;
    }

    playback.triggerEvent();
    
      lightPressableButton->updateTrue();
}

void Track::timerCallback() {
  
    routeMidiMessage();
    lastMidiNote = 0;

}

void Track::comboBoxChanged(ComboBox* comboBoxThatHasChanged){
    if (comboBoxThatHasChanged == &selectInputs) {
    String named = selectInputs.getText();
        retrieveInputs(named);
        inputsAvailables = admInfo.inputsAvailable;
        updateInputs();
    }
    
    
    if (comboBoxThatHasChanged == &selectOutputs) {
        String named = selectOutputs.getText();
        retrieveOutputs(named);
        outputsAvailables = admInfo.outputsAvailable;
        updateOutputs();
    }

    
    if (comboBoxThatHasChanged == &selectPedals) {
        nPedal =  (selectPedals.getSelectedId());
      
    }
    
    
    
}

void Track::retrieveInputs(String fromComboBox){
    StringArray tokens;
    tokens.addTokens (fromComboBox,"+");
    if(tokens.size() == 1)
        isStereo = false;
    else
        isStereo = true;
    if(isStereo){
        inputL = tokens[0].getIntValue();
        inputR = tokens[2].getIntValue();
    }
    else if (!isStereo){
        inputL = tokens[0].getIntValue();
    }
    if(isStereo){
        inputL = tokens[0].getIntValue();
        inputR = tokens[2].getIntValue();
    }
    else if (!isStereo){
        inputL = tokens[0].getIntValue();
        inputR = 0;
    }
    
 
}

void Track::retrieveOutputs(String fromComboBox){
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


    


void Track::changeListenerCallback (ChangeBroadcaster*) {
 //Inputs Changed
    selectInputs.clear();
    for (int i = 0; i < admInfo.inputsAvailable.size(); i++){
        selectInputs.addItem (admInfo.inputsAvailable[i], i+1);
    }
    inputsAvailables = admInfo.inputsAvailable;
    updateInputs();

    //Inputs Changed
    selectOutputs.clear();
    for (int i = 0; i < admInfo.outputsAvailable.size(); i++){
        selectOutputs.addItem (admInfo.outputsAvailable[i], i+1);
    }
    outputsAvailables = admInfo.outputsAvailable;
    updateOutputs();
    
    
//Pedals Changed
    
    selectPedals.clear();
    
    for (int i = 1; i < pedalAvailable.size() + 1; i++){
        selectPedals.addItem (String(i), i);
    }
};

void Track::updateInputs() {
    String inputsLmomentary = String(inputL);
    String inputsRmomentary = String(inputR);
    inputL  = inputsAvailables.indexOf(inputsLmomentary) + 1;
    inputR  = inputsAvailables.indexOf(inputsRmomentary) + 1;

}

void Track::updateOutputs() {
    String outputLmomentary = String(outputL);
    String outputRmomentary = String(outputR);
    outputL  = outputsAvailables.indexOf(outputLmomentary) + 1;
    outputR  = outputsAvailables.indexOf(outputRmomentary) + 1;
}



void Track::initialise() {
  
    admInfo.addChangeListener(this);
    lightPressableButton.reset(new LightPressableButton(recordState, isStartingRecording, isStoppingRecording, nPedal, recordState));
    audioSourcePlayer.setSource(this);
    admInfo.addAudioCallback(&audioSourcePlayer);
    Identifier  BPMratio ("BPMratio");
    loopUnit = float(tree.getParent().getProperty(BPMratio));
    formatManager.registerBasicFormats();
    selectFolder(nameOfTrack);

    setDefaultLookAndFeelColours();
    
}


void Track::initialiseMeter() {
    
    meter.setLookAndFeel (&levelMeterLookAndFeel);
    meter.setMeterSource (&levelMeterSource);
    addAndMakeVisible (&meter);
    levelMeterLookAndFeel.setColour (FFAU::LevelMeter::lmBackgroundColour,       Colours::transparentWhite);
    levelMeterLookAndFeel.setColour (FFAU::LevelMeter::lmTicksColour,            Colours::BandLoopBackground);
    levelMeterLookAndFeel.setColour (FFAU::LevelMeter::lmOutlineColour,           Colours::transparentWhite);
    meter.setMeterFlags(FFAU::LevelMeter::SingleChannel);
    meter.setSelectedChannel(0);
    levelMeterLookAndFeel.setColour (FFAU::LevelMeter::lmMeterForegroundColour,  Colours::transparentWhite.darker(1));
    levelMeterLookAndFeel.setColour (FFAU::LevelMeter::lmMeterOutlineColour,     Colours::transparentWhite.darker(1));
    levelMeterLookAndFeel.setColour (FFAU::LevelMeter::lmMeterBackgroundColour,  Colours::transparentWhite.darker(1));

}

void Track::initialiseSlider() {
    
    decibelSlider.setRange (-100, 6);
    decibelSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    decibelSlider.setTextBoxStyle (Slider::TextBoxAbove, false, 100, 20);
    decibelSlider.onValueChange = [this] { sliderValue.store(Decibels::decibelsToGain ((float) decibelSlider.getValue())); };
    decibelSlider.setValue (Decibels::gainToDecibels (sliderValue.load()));
    addAndMakeVisible (decibelSlider);
    decibelSlider.setLookAndFeel (&customLookAndFeel);
    
    
}

void Track::initialiseInputSelector() {
    
    selectInputs.addListener(this);
    labelSelectInputs.attachToComponent(&selectInputs, false);
    labelSelectInputs.setText ("Input:", dontSendNotification);
    addAndMakeVisible(labelSelectInputs);
    addAndMakeVisible(selectInputs);
    inputsAvailables = admInfo.inputsAvailable;
    for (int i = 0; i < admInfo.inputsAvailable.size(); i++){
        selectInputs.addItem (admInfo.inputsAvailable[i], i+1);
    }
    
     selectInputs.setSelectedId(4);
    
}

void Track::initialiseOutputSelector() {
    
    selectOutputs.addListener(this);
    labelSelectOutputs.attachToComponent(&selectOutputs, false);
    labelSelectOutputs.setText ("Output:", dontSendNotification);
    addAndMakeVisible(labelSelectOutputs);
    addAndMakeVisible(selectOutputs);
    outputsAvailables = admInfo.outputsAvailable;
    for (int i = 0; i < admInfo.outputsAvailable.size(); i++){
        selectOutputs.addItem (admInfo.outputsAvailable[i], i+1);
    }
    
    selectOutputs.setTextWhenNothingSelected("1+2");
    
}


void Track::initialisePedalSelector() {
    
    selectPedals.addListener(this);
    addAndMakeVisible(selectPedals);
    labelSelectPedals.attachToComponent(&selectPedals, false);
    labelSelectPedals.setText ("Pedal:", dontSendNotification);
    addAndMakeVisible(labelSelectPedals);
    for (int i = 1; i < pedalAvailable.size() + 1; i++){
        selectPedals.addItem (String(i), pedalAvailable[i-1]);
    }
    
}

void Track::initialiseNameOfTrack() {
    
    labelNameOfTrack.setColour(Colours::BandLoopBackground.brighter(0.15));
    addAndMakeVisible(labelNameOfTrack);
    labelNameOfTrack.text.addListener(this);
    labelNameOfTrack.setName(nameOfTrack, 20);
    
}


void Track::initialiseButton() {
    
    addAndMakeVisible(playback);
    
    playButtonImage = Drawable::createFromImageData (BinaryData::StartRecordingButton_svg, BinaryData::StartRecordingButton_svgSize);
    stopButtonImage = Drawable::createFromImageData (BinaryData::StopRecordingButton_svg, BinaryData::StopRecordingButton_svgSize);
    DrawableRecordButton.setImages ( playButtonImage,
                                  playButtonImage,
                                  playButtonImage,
                                  playButtonImage,
                                  stopButtonImage,
                                  stopButtonImage,
                                  stopButtonImage, stopButtonImage);
    addAndMakeVisible(DrawableRecordButton);
    
    DrawableRecordButton.addListener(this);
    
//    DrawableRecordButton.onClick = [this]() { isStartingRecording = true ; };
    addAndMakeVisible(*lightPressableButton);
    
    addAndMakeVisible (stopImmediatellyButton);
    addAndMakeVisible (undoButton);
    addAndMakeVisible (saveButton);
    addAndMakeVisible (newSongButton);

    undoButton.addListener(this);
    saveButton.addListener(this);
    newSongButton.addListener(this);
    stopImmediatellyButton.addListener(this);
    


}

void Track::routeMidiMessage(){
    
    
    if (lastMidiNote) {
        DBG("note played:");
        DBG(String(lastMidiNote));
        if (lastMidiNote == 1) {firstFootSwitch();}
        if (lastMidiNote == 2) {secondFootSwitch();}
        if (lastMidiNote == 3) {thirdFootSwitch();}
        if (lastMidiNote == 4) {fourthFootSwitch();}
        if (lastMidiNote == 5) {fifthFootSwitch();}
        if (lastMidiNote == 6) {sixthFootSwitch();}
  
    midiBufferFromPedal.clear();
    }
    }



// ===============   FROM Pedals Methods  =================================

void Track::firstFootSwitch(){
    
    if (recordState == RecordState::Playing)
    {
        DrawableRecordButton.setToggleState(true, dontSendNotification);
        recordState = RecordState::isStartingRecording;
        lastThingDoneFirstButton = "isStartingRecording";
        lastThingDoneGeneral = "isStartingRecording";
    }
    
    else if (recordState == RecordState::isStartingPlaying)
    {
        DrawableRecordButton.setToggleState(true, dontSendNotification);
        recordState = RecordState::isStartingRecording;
        lastThingDoneFirstButton = "isStartingRecording";
        lastThingDoneGeneral = "isStartingRecording";
    }
    
    else if (recordState == RecordState::Recording)
    {
        DrawableRecordButton.setToggleState(false, dontSendNotification);
        recordState = RecordState::isStartingPlaying;
        lastThingDoneFirstButton = "isStartingPlaying";
        lastThingDoneGeneral = "isStartingPlaying";
    }
    
    else if (recordState == RecordState::isStartingRecording)
    {
        DrawableRecordButton.setToggleState(false, dontSendNotification);
        recordState = RecordState::isStartingPlaying;
        lastThingDoneFirstButton = "isStartingPlaying";
        lastThingDoneGeneral = "isStartingPlaying";
    }
    
  
   
    
    
//    else
//    {
//        DrawableRecordButton.setToggleState(false, dontSendNotification);
//
//        isStartingRecording = false ;
//        isStoppingRecording = true ;
//        lastThingDoneFirstButton = "isStoppingRecording";
//        lastThingDoneGeneral = "isStoppingRecording";
//    }
//    
//    playback.startRecording();
    
};


void Track::secondFootSwitch(){
    
    recordState = RecordState::Stopped ;

//    if (lastThingDoneFirstButton == "isStartingRecording") {isStartingRecording = false ;}
//
//    if (lastThingDoneFirstButton == "isStoppingRecording") {isStoppingRecording = false ;}
//
//    if (recordState == RecordState::Recording) {recordState = RecordState::Playing;
//
        DrawableRecordButton.setToggleState(false, dontSendNotification);
//
//    }

    playback.stopImmediatelly();
    
};


void Track::thirdFootSwitch(){
    
    
    
    
};

void Track::fourthFootSwitch(){save();};

void Track::fifthFootSwitch(){ playback.startOrStopAtNextBar(); };

void Track::sixthFootSwitch(){ playback.newSong(oldFolder, actualFolder);};


void Track::setDefaultLookAndFeelColours(){
    
    customLookAndFeel.setColour (ComboBox::backgroundColourId, Colours::BandLoopBackground);
    customLookAndFeel.setColour (ComboBox::textColourId,Colours::BandLoopText);
    customLookAndFeel.setColour (ComboBox::buttonColourId,Colours::BandLoopBackground);
    customLookAndFeel.setColour (ComboBox::arrowColourId, Colours::BandLoopText);
    customLookAndFeel.setColour (ComboBox::focusedOutlineColourId,Colours::BandLoopText);
        customLookAndFeel.setColour (Label::textColourId,Colours::BandLoopText);
}


void Track::updateGuiAndPedals() {
    
    lightPressableButton->updateTrue();
    
    
};


void Track::save(){
  
    auto parentDir = File::getSpecialLocation (File::userDocumentsDirectory).getChildFile("BandLoop").getChildFile("Saved").getChildFile(nameOfTrack);
    
    if(parentDir.isDirectory()) {
        savedFolder = parentDir.getNonexistentSibling();
    }
    else if(!parentDir.isDirectory()) {
        savedFolder = parentDir;
    }
    
//    nameOfTrack = savedFolder.getFileName();
    bool b = savedFolder.createDirectory();
    
    playback.save(savedFolder, actualFolder);
};
