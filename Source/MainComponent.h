/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "/Users/giovanni/Recorder+Sampler/Source/Track.h"
#include "/Users/giovanni/Recorder+Sampler/Source/AudioRecorder.h"
#include "/Users/giovanni/Recorder+Sampler/Source/SynthAudioSource.h"
#include "/Users/giovanni/Recorder+Sampler/Source/BPM.h"
#include "/Users/giovanni/Recorder+Sampler/Source/addTrackWindow.h"

class MainComponent   : public AudioAppComponent,
public ChangeListener,
public HighResolutionTimer,
public Timer,
public Button::Listener
{
public:
    
    //==============================================================================
    
    MainComponent();
    ~MainComponent();
    
    //==============================================================================
    
    void transportSetup();
    void playTransport();
    void stopTransport();
    void hiResTimerCallback() override;
    void timerCallback() override;
    void buttonClicked (Button* button) override;
    
    //==============================================================================
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //==============================================================================
    
    void paint (Graphics& g) override;
    void resized() override;
    
    void startRecording();
    void stopRecording();
    
    void createBandLoopFolder();
    void createTempFolder();
    void createSavedFolder();
    
    void changeListenerCallback (ChangeBroadcaster*) override;

    void createInputSelections(Array<int> toStereo);
    
    void closeAllWindows();
    void showDocumentWindow (bool native, String& inputTrackToPass, String& nameTrackToPass );
    
    void createNewTrack();
    
private:
    
// DEVICES
//==============================================================================
    
    AudioDeviceSelectorComponent audioSetupComp;
    MidiKeyboardState keyboardState;
    MidiMessageCollector midiCollector;
    AudioSourcePlayer audioSourcePlayer;
    

    
// CLASSES
//==============================================================================
    
    ScopedPointer<Track> fTracks;
    
    Identifier  myNodeType = ("mainTree");
    ValueTree mainTree {myNodeType}  ;
    
    UndoManager undoManager;
    

    
// FILES & FOLDERS
//==============================================================================
    
    File lastRecording;
    File Main;
    File TempFolder;
    File SavedFolder;

    
    
// GRAPHICS (buttons, sliders, windows)
//==============================================================================
    
    Array<Component::SafePointer<Component>> windows;
    TextButton addTrack   { "+" };
    Slider sliderTrack1;
    TextButton buttonTrack1;
    TextButton recordButtonTrack1;
    
    String nameTrackToPass ;
    
    String inputTrackToPass ;
    
    
// BOOLS and VALUES
//==============================================================================
   
    bool recording = false ;
    int pos = 0;
    
    BigInteger bigIntegerInputsAvailable ;
    Array<int> toStereo;
    StringArray inputsAvailable;
    
    static Array<int> getListOfActiveBits (const BigInteger& b) //  Get Inputs from BigInteger
    {Array<int> inputsAvailable;
        for (auto i = 0; i <= b.getHighestBit(); ++i)
            if (b[i])
                inputsAvailable.add(int (i)+1);
        return inputsAvailable;
    }
  
    
// BPM Transport
//==============================================================================
    
    enum class PlayState
    {
        Play,
        Stop
    };
    
    float previousBar = 0;
    float startBar = 0;
    
    float loopUnit;
    int loopLenght;
    
    std::atomic<float> timeUIfloat;
    
    String timeUI { "" };
    int timeInMilliseconds = 0;
    
    float Bars = 0;
    int Bar;
    float Beats = 0;
    int Beat;
    
    String BarToText = "0";
    String BeatToText = "0";
    
    int BPMs = 120;
    float BPMratio = 0.;
    
    Label labelBPM;
    Label textBPM;
    
    PlayState playState { PlayState::Stop };
    TextButton playButton { "Stopped" };
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
