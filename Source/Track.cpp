/*
  ==============================================================================

    Track.cpp
    Created: 11 Mar 2019 12:56:15pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#include "Track.h"


Track::Track(String trackColour,
             Slider* slider,
             TextButton* playStop,
             String inputs,
              MidiMessageCollector& fromPedals,
             const ValueTree& v,
             UndoManager& um)
: tree (v)
, midiCollectorFromPedals (fromPedals)
, undoManager (um)
, recorder()
, recordButton("Press To Record")
{
//   tree.addListener (this);

    
    
    Identifier  BPMratio ("BPMratio");
    
    loopUnit = float(tree.getParent().getProperty(BPMratio));
    
    
    colour = trackColour;
    
    formatManager.registerBasicFormats();
    
    recordButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::red);
    recordButton.setColour(TextButton::ColourIds::buttonColourId, Colours::limegreen);//    recordButton.onClick = [this]() { play(); };
    addAndMakeVisible(recordButton);
    
    selectFolder(nameOfTrack);
    
//    levelSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
//    levelSlider.setColour (Label::textColourId, Colours::orange);
//    addAndMakeVisible (levelSlider);
    recordButton.addListener(this);
    recordButton.onClick = [this]() { startRecording(nameOfTrack); };
//    levelSlider.addListener(this);
    
    decibelSlider.setRange (-100, 6);
    decibelSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    decibelSlider.setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
    decibelSlider.onValueChange = [this] { sliderValue = Decibels::decibelsToGain ((float) decibelSlider.getValue()); };
    decibelSlider.setValue (Decibels::gainToDecibels (sliderValue));
//    decibelLabel.setText ("Noise Level in dB", dontSendNotification);
    decibelLabel.attachToComponent(&decibelSlider, false);
    
    addAndMakeVisible (decibelSlider);
    addAndMakeVisible (decibelLabel);
    
    resized();
    
//    recordButton.onClick = [this]() { startRecording(nameOfTrack);};
    LabelOfTrack.attachToComponent(&decibelSlider, false);
    LabelOfTrack.setText(nameOfTrack, dontSendNotification);
    
}

    Track::~Track(){
    }
    
    void Track::process(AudioBuffer<float> bufferIn, const AudioSourceChannelInfo& bufferOut, Slider* slider, int input){}
    
    void Track::setValue(float value){}

//=============================================================================
    void Track::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {

        bufferToRecordL.setSize(1, samplesPerBlockExpected);
        bufferToRecordR.setSize(1, samplesPerBlockExpected);
        bufferToRecordStereo.setSize(2, samplesPerBlockExpected);
        
        recorder.setSampleRate(sampleRate);
        playback.prepareToPlay(samplesPerBlockExpected, sampleRate);
        midiCollectorFromPedals.reset (sampleRate);
        
    }
    void Track::releaseResources() {}

    void Track::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {

        
        MidiBuffer incomingMidi;
    
        midiCollectorFromPedals.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples); // [11]

    
//        playback.getNe (*bufferToFill.buffer, incomingMidi,
//                               bufferToFill.startSample, bufferToFill.numSamples);
        
        int numSamples = bufferToFill.numSamples ;
        
        if (inputL){
                        if(!isStereo){

            //copy the desired content(channel) from bufferToFill and pass it to the recorder
             const auto numSamples = bufferToFill.buffer->getNumSamples();

                bufferToRecordL.copyFrom(0, 0, *bufferToFill.buffer, (inputL - 1), 0, numSamples);

                bufferToRecordL.applyGain(0, 0, numSamples, sliderValue);
                            
        float* pointers [2] = { bufferToRecordL.getWritePointer (0, bufferToFill.startSample),
                                bufferToRecordL.getWritePointer (0, bufferToFill.startSample) };

                if (recordState == RecordState::Recording)
                    recorder.Record(pointers, numSamples);

                bufferToFill.clearActiveBufferRegion();
                bufferToFill.buffer->addFrom(0, 0, bufferToRecordL, 0, 0, numSamples);
                bufferToFill.buffer->addFrom(1, 0, bufferToRecordL, 0, 0, numSamples);

        }
            if(isStereo){

                //copy the desired content(channel) from bufferToFill and pass it to the recorder
                const auto numSamples = bufferToFill.buffer->getNumSamples();
                bufferToRecordL.copyFrom(0, 0, *bufferToFill.buffer, (inputL - 1), 0, numSamples);
                bufferToRecordR.copyFrom(0, 0, *bufferToFill.buffer, (inputR - 1), 0, numSamples);

                bufferToFill.clearActiveBufferRegion();

                bufferToRecordStereo.copyFrom(0, 0, bufferToRecordL, 0, 0, numSamples);
                bufferToRecordStereo.copyFrom(1, 0, bufferToRecordR, 0, 0, numSamples);

                bufferToRecordStereo.applyGain(0, 0, numSamples, sliderValue);
                bufferToRecordStereo.applyGain(1, 0, numSamples, sliderValue);

        float* pointers [2] = { bufferToRecordStereo.getWritePointer (0, bufferToFill.startSample),
                                bufferToRecordStereo.getWritePointer (1, bufferToFill.startSample) };
                
                if (recordState == RecordState::Recording)
                    recorder.Record(pointers, numSamples);

                bufferToFill.buffer->addFrom(0, 0, bufferToRecordStereo, 0, 0, numSamples);
                bufferToFill.buffer->addFrom(1, 0, bufferToRecordStereo, 1, 0, numSamples);
            }
            
            playback.getNextAudioBlock(bufferToFill);

        }
        
         else {
             
                          bufferToFill.clearActiveBufferRegion();
             
         }
        
        
        
    }
        
       

//=============================================================================
void Track::paint (Graphics& g)
{
    
    Rectangle<int> bounds= getLocalBounds();
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

    
}

void Track::resized()
{
    Rectangle<int> bounds= getLocalBounds();
    Path pedal;
    pedal.addRoundedRectangle(bounds, 20, 20);
    
//    recordButton.setBounds(0, 0, 75, 75);
    decibelSlider.setBounds(bounds.removeFromBottom(bounds.getHeight()-60));

    
}


//=============================================================================
void Track::startRecording(String name) {
    
    isStartingRecording = true ;
    

}

void Track::stopRecording() {
    
    isStoppingRecording = true;

}

void Track::buttonClicked (Button* button) {
    
    if (button == &recordButton)
    {
        if (recordState == RecordState::Playing)
        {
            recordButton.onClick = [&]() { startRecording(nameOfTrack); };
        }
        else
        {
            recordButton.onClick = [&]() { stopRecording(); };
        }
    }
    
};


void Track::sliderValueChanged(Slider* slider) {
//    if(slider == &slider){
//        sliderValue = levelSlider.getValue();
//    }
    
};

//create a folder named as the Track;
//=============================================================================
void Track::selectFolder(String name) {
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
    LabelOfTrack.setText(named, dontSendNotification);
    }

    
// Retrieving the Inputs and isStereo
    Identifier inputs = "Inputs";
    StringArray tokens;
if (property == inputs && treeChanged == tree) {
    
        String named = treeChanged.getProperty(inputs);
        tokens.addTokens (named,"+" );
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

};
    
};

String Track::getColour() {
    return colour;
    
}

void Track::checkEvent(int bar) {
    
    playback.checkEvent(bar);
    

    if (isStoppingRecording) {
        recorder.stopRecording();
        playback.createSound(lastRecording, actualFolder, loopUnit);
        recordState = RecordState::Playing;
        recordButton.setToggleState(false, NotificationType::dontSendNotification);
        recordButton.setButtonText("Playing");
        isStoppingRecording = false;
    }
    
    
}
void Track::triggerEvent() {
 
    if (isStartingRecording){
        lastRecording = actualFolder.getNonexistentChildFile(nameOfTrack, ".wav");
        recorder.startRecording(lastRecording);
        recordState = RecordState::Recording;
        recordButton.setToggleState(true, NotificationType::dontSendNotification);
        recordButton.setButtonText("Recording");
        isStartingRecording = false;
    }

    playback.triggerEvent();
    
    
}

