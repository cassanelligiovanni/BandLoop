/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "/Users/giovanni/BandLoop/Source/Track.h"
#include "/Users/giovanni/BandLoop/Source/AudioRecorder.h"
#include "/Users/giovanni/BandLoop/Source/Playback.h"
#include "/Users/giovanni/BandLoop/Source/BPM.h"
#include "/Users/giovanni/BandLoop/Source/addTrackWindow.h"
#include "/Users/giovanni/BandLoop/Source/SettingWindow.h"
#include "/Users/giovanni/BandLoop/Source/Sound.h"
#include "/Users/giovanni/BandLoop/Source/CustomLookAndFeel.h"



class MainComponent   : public AudioAppComponent,
public ChangeListener,
public ValueTree::Listener,
public Button::Listener,
private MidiInputCallback
{
public:
    
    //==============================================================================
    
    MainComponent();
    ~MainComponent();
    
    
    //==============================================================================
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //==============================================================================
    
    void paint (Graphics& g) override;
    void resized() override;
    
    void buttonClicked(Button* button) override;
    
    void startRecording();
    void stopRecording();
    
    void createBandLoopFolder();
    void createTempFolder();
    void createSavedFolder();
    
    void changeListenerCallback (ChangeBroadcaster*) override;

    void createInputSelections(Array<int> toStereo);
    
    void setupSquareLookAndFeelColours ();
    
    void showDocumentWindow (bool native, const ValueTree& newTree);
    void showSettingWindow(bool native, const ValueTree& newTree, AudioDeviceSelectorComponent& audioSetupComp);
    void closeAllWindows();
    
    
    
    void createNewTrack();
    void deleteTrack(String trackToDelete);
    
    
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override;
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override;
    void valueTreeParentChanged (ValueTree&) override;
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    
    void handleIncomingMidiMessage (MidiInput* /*source*/,
                                    const MidiMessage& message) override;
    
    
private:
    
// DEVICES
//==============================================================================
    
    AudioDeviceSelectorComponent audioSetupComp;
    MidiKeyboardState keyboardState;
    MidiMessageCollector midiCollector;
    AudioSourcePlayer audioSourcePlayer;
    AudioSourcePlayer audioSourcePlayer2;
    
    OwnedArray<AudioSourcePlayer> audioSourcePlayers;
    OwnedArray<MidiMessageCollector> fromPedals;
    

// CLASSES
//==============================================================================

    OwnedArray<FlexItem> flexItems ;
    OwnedArray<Track> fTracks;
    OwnedArray<BPM> BPMS;
 
    
// TREEs
//==============================================================================
    
    Identifier  myNodeType = ("mainTree");
    ValueTree parentTree {myNodeType} ;
    OwnedArray<ValueTree> treeChildren;
    UndoManager undoManager;
    int numOfTrees = 0 ;
    

    
// FILES & FOLDERS
//==============================================================================
    
    File lastRecording;
    File Main;
    File TempFolder;
    File SavedFolder;

    
    
// GRAPHICS (buttons, sliders, windows)
//==============================================================================
    
    //in the header:
    ScopedPointer<Drawable> background_image;
    
    ScopedPointer<Drawable> deleteTrackImage = Drawable::createFromImageData (BinaryData::DeleteTrack_svg, BinaryData::DeleteTrack_svgSize);
    
    
    OwnedArray<DrawableButton> deleteTrackButtons;
    
    CustomLookAndFeel customLookAndFeel;
    
    OwnedArray<TextButton> cancelTrackButtons;
    
    Array<Component::SafePointer<Component>> windows;
    TextButton addTrack   { "+" };
    TextButton settings   { "settings" };
    Slider sliderTrack1;
    TextButton buttonTrack1;
    TextButton recordButtonTrack1;
   
    
    String nameTrackToPass ;
    
    String inputTrackToPass ;
    
//    FlexBox tracks { FlexBox::Direction::row, FlexBox::Wrap::wrap, FlexBox::AlignContent::center, FlexBox::AlignItems::center, FlexBox::JustifyContent::center };
//    FlexBox cancelTracks { FlexBox::Direction::row, FlexBox::Wrap::wrap, FlexBox::AlignContent::center, FlexBox::AlignItems::center, FlexBox::JustifyContent::center };

    
//    Array<Colour> colours { Colours::yellow, Colours::orange, Colours::blue };
    StringArray coloursOfTracks {"darkcyan", "orange", "rebeccapurple", "cornsilk", "pink","grey"} ;
    StringArray trackColourAvailable;
    StringArray trackColourAssigned ;
    
    
// BOOLS and VALUES
//==============================================================================
   
    bool recording = false ;
    int pos = 0;
    
// Inputs Routing
    BigInteger bigIntegerInputsAvailable ;
    Array<int> toStereo;
    StringArray inputsAvailable;
    
    static Array<int> getListOfActiveBits (const BigInteger& b) //  Get Inputs from BigInteger
    {Array<int> inputsAvailable;
        for (auto i = 0; i <= b.getHighestBit(); ++i)
            if (b[i])
                inputsAvailable.add(int (i)+1);
        return inputsAvailable; }

    
  
    
// BPM Transport
//==============================================================================

    float Bars = 0;
    int Bar;
    float Beats = 0;
    int Beat;
    
    String BarToText = "0";
    String BeatToText = "0";
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
