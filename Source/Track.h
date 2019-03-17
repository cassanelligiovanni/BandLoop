/*
  ==============================================================================

    Track.h
    Created: 11 Mar 2019 12:56:15pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "/Users/giovanni/Recorder+Sampler/Source/SynthAudioSource.h"
#include "/Users/giovanni/Recorder+Sampler/Source/AudioRecorder.h"

class Track : public AudioSource,
              public ValueTree::Listener
{
    public :
    Track(String name, Slider* slider, TextButton* playStop,
          String inputs, MidiMessageCollector* midiCollector,
          const ValueTree& v, UndoManager& um);
    ~Track();
    
    void process(AudioBuffer<float> bufferIn, const AudioSourceChannelInfo& bufferOut, Slider* slider, int input);
    
    void setValue(float value);
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    void releaseResources() override;
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    
    void startRecording(String name);
    
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override;
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override;
    void valueTreeParentChanged (ValueTree&) override;
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    
   File selectFolder(String name);
    
    AudioRecorder recorder;
    
private:
    
    ValueTree tree;
    UndoManager& undoManager;
    
    String name;
    File folder;
    
    float sliderValue;
    Slider* sliderChild;
    TextButton* playStop;
    String* inputs;
    Random random;
    
    MidiMessageCollector midiCollector;
//    MidiKeyboardState& keyboardState;
    
    SynthAudioSource playback;
    
    AudioBuffer<float> bufferToRecord;
    
    bool recording = false;

    
};
