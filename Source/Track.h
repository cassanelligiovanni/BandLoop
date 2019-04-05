/*
  ==============================================================================

    Track.h
    Created: 11 Mar 2019 12:56:15pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "/Users/giovanni/BandLoop/Source/Playback.h"
#include "/Users/giovanni/BandLoop/Source/AudioRecorder.h"
#include "/Users/giovanni/BandLoop/Source/DecibelSlider.h"

class Track : public AudioSource,
              public Component,
              public ValueTree::Listener,
              public Slider::Listener,
              public Button::Listener
{
    public :
    Track(String trackColour, Slider* slider, TextButton* playStop,
          String inputs, MidiMessageCollector& fromPedal,
          const ValueTree& v, UndoManager& um);
    ~Track() override;
    
    void process(AudioBuffer<float> bufferIn, const AudioSourceChannelInfo& bufferOut, Slider* slider, int input);
    
    void setValue(float value);
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    
    void paint (Graphics& g) override;
    void resized() override;
    
    void buttonClicked (Button* button) override;
    void sliderValueChanged(Slider*     slider) override;
    
    void checkEvent(int bar);
    void triggerEvent();
    
    void startRecording(String name);
    void stopRecording();
    
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override;
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override;
    void valueTreeParentChanged (ValueTree&) override;
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    
   void selectFolder(String name);
    
  String getColour();


    ValueTree tree;
    UndoManager& undoManager;
    MidiMessageCollector& midiCollectorFromPedals;
    
private:
    
    String colour;
    
    AudioRecorder recorder;

    int actualBar;
    
    File folder;
     
    Slider* sliderChild;
    TextButton* playStop;
    Random random;
    
//    MidiKeyboardState& keyboardState;
    
    Playback playback {midiCollectorFromPedals};
    
AudioFormatManager formatManager; //da rivedere
    
    
    AudioBuffer<float> bufferToRecordL;
    AudioBuffer<float> bufferToRecordR;
    AudioBuffer<float> bufferToRecordStereo;
    
    bool recording = false;

//Final Development
//    Slider levelSlider    { Slider::LinearVertical, Slider::NoTextBox};
    DecibelSlider decibelSlider ;
    Label decibelLabel ;
//    Slider levelSlider;
    
    float sliderValue;
    float sliderGain;
    
    TextButton recordButton { "Press To Record" };;
    String nameOfTrack { Time::getCurrentTime().toString(true, false)};
    Label LabelOfTrack;
    File actualFolder;
    File lastRecording;
    
    String inputs;
    bool isStereo;
    int inputL = 0;
    int inputR;
    
    enum class RecordState
    {
        null,
        readyToRecord,
        Recording,
        readyToPlayback,
        Playing
    };
    
    RecordState recordState { RecordState::Playing };
    
    float loopUnit;
    bool isStartingRecording = false;
    bool isStoppingRecording = false;

    
    // SVG

    
};


