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
#include "/Users/giovanni/BandLoop/Source/CustomLookAndFeel2.h"
#include "/Users/giovanni/BandLoop/Source/ADMinfo.h"
#include "/Users/giovanni/BandLoop/Source/PassthroughProcessor.h"
#include "/Users/giovanni/BandLoop/Source/GainProcessor.h"


typedef uint32  NodeId;

class MainComponent   :
                            public AudioAppComponent,
                            public ChangeListener,
                            public ValueTree::Listener,
                            public Button::Listener,
                            private MidiInputCallback
{
    
    public:
    
            MainComponent();
            ~MainComponent();
    
  
// ===============   AUDIO SOURCE Methods  ==================================
    
    /**  This callback allows the source to initialise any resources it might need when playing.
     @param samplesPerBlockExpected  (bufferSize)
     @param sampleRate (Hz)
     */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    /** This callback will be made each
     time the audio playback hardware (or whatever other destination the audio
     data is going to) needs another block of data.
     */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    
    /** Allows the source to release anything it no longer needs after playback has stopped.
    */
    void releaseResources() override;
 
    
// ===============   COMPONENT Methods  ==================================
  
    /**  The paint() Method gets called when a region of a component needs redrawing,
         Never need to call this method directly
     
        @param g    the graphics context that must be used to do the drawing operations.
    */
    void paint (Graphics& g) override;
   
    /** Called when this component's size has been changed.
     */
    void resized() override;

// ===============   LISTENERS Methods  ==================================
    
    /** Called when a Button is Pressed has been changed.
     @param button, use this for filtering the button that is actually pressed if (button == recordButton) then...
     */
    void buttonClicked(Button* button) override;
    void changeListenerCallback (ChangeBroadcaster*) override;
   
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override;
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override;
    void valueTreeParentChanged (ValueTree&) override;
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    
    void handleIncomingMidiMessage (MidiInput* /*source*/,
                                    const MidiMessage& message) override;

// ===========================================================================
    
    /** Initialise the Track :
         - Create the necessary folders,
         - Set the LookAndFeel,
     @see createBandLoopFolder,createTempFolder, createSavedFolder, setupSquareLookAndFeelColours
     */
    void initialise();
    
    /** Create a BandLoop folder in the User Document Folder
     */
    void createBandLoopFolder();
   
    /** Create a "Temp" folder in the BandLoop Folder
     */
    void createTempFolder();
   
    /** Create a "Saved" folder in the BandLoop Folder
     */
    void createSavedFolder();
    
    /** Set the default LookAndFeel Method and its Colours
        It is used to decide appearence of COMPONENTS and how they will be draw,
        by inheriting from their LookAndFeelMethods
    */
    void setupSquareLookAndFeelColours ();
    
    /** crea
     */
    void createNewTrack();
    
    /**  create a ChildTree, append it to the MainTree and return it.
     @return ValueTree
     */
    ValueTree createNewChildTree();
    
    
    /**  Create a deleteButton for each track,
     *   it is outside the Track object because it needs to keep track of the available
     *   colours
     */
    void createDeleteButton();
    
    
    /**  Delete track,
     *   update colours available, trees, track array, and deleteButton array.
     *
     */
    void deleteTrack(String trackToDelete);
    
    
    /** Open setting Window
     @param bool native,
     @param AudioDeviceSelectorComponent& audioSetupComp
     */
    void showSettingWindow(bool native, const ValueTree& newTree, AudioDeviceSelectorComponent& audioSetupComp);
  
    /** Close all Windows
     */
    void closeAllWindows();
    
    void midiResetting();
    
    void numberPedalClicked(int note);
    
    NodeId AddProcessor(AudioProcessor* p);
    
    
private:
    
// DEVICES
//==============================================================================
    
    std::unique_ptr<ADMinfo> admInfo;
    std::unique_ptr<BPM> globalTempo;
    
    AudioDeviceSelectorComponent audioSetupComp;
    MidiKeyboardState keyboardState;
    MidiMessageCollector midiCollector;
    MidiMessageCollector midiSetting;
    AudioSourcePlayer audioSourcePlayerClick;
  
    OwnedArray<MidiMessageCollector> fromPedals;
    

// CLASSES
//==============================================================================

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

    File Main;
    File TempFolder;
    File SavedFolder;

    
    
// GRAPHICS (buttons, sliders, windows)
//==============================================================================
    
    //in the header:
    ScopedPointer<Drawable> deleteTrackImage = Drawable::createFromImageData (BinaryData::DeleteTrack_svg, BinaryData::DeleteTrack_svgSize);

    OwnedArray<DrawableButton> deleteTrackButtons;
    
        CustomLookAndFeel customLookAndFeel;
    CustomLookAndFeel2 customLookAndFeel2;
    
    Array<Component::SafePointer<SettingWindow>> windows;
    TextButton addTrack   { "+" };
    TextButton settings   { "settings" };

    NamedValueSet& addTrackProperties = addTrack.getProperties();
    
    StringArray coloursOfTracks {"darkcyan", "orange", "rebeccapurple", "cornsilk", "pink","grey"} ;
    StringArray trackColourAvailable;
    StringArray trackColourAssigned ;
    
    int maxNumberOfPedals = 5;
    Array<int> pedalsAvailables;
    int sampleHz ;
    
    
    AudioProcessorPlayer fPlayer;
    AudioProcessorGraph  fGraph;
    
    NodeId fInputNode;
    NodeId fOutputNode;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
