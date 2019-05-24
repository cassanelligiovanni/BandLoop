/*
  ==============================================================================

    Track.h
    Created: 11 Mar 2019 12:56:15pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Playback.h"
#include "../Source/AudioRecorder.h"
#include "../Source/DecibelSlider.h"
#include "../Source/CustomLookAndFeel2.h"
#include "../Source/CustomLookAndFeel.h"
#include "../ff_meters-master 2/ff_meters.h"
#include "../Source/ADMinfo.h"
#include "../Source/LightPressableButton.h"
#include "../Source/RecordState.h"
#include "../Source/GradientLabel.h"


class Track : public AudioSource,
              public Component,
              public ValueTree::Listener,
              public Label::Listener,
              public Slider::Listener,
              public Button::Listener,
              public ComboBox::Listener,
              public Timer,
              public ChangeListener
{
    public :
   
    Track(String trackColour, String inputs, OwnedArray<MidiMessageCollector>& fromPedal, Array<int>& pedalsAvailables,
          const ValueTree& v, UndoManager& um, ADMinfo& AdmInfo);
    
    ~Track() override;
    
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
    void buttonClicked (Button* button) override;
    
    /** Called when a Slider is moved.
     @param slider, use this for filtering the button that is actually pressed if (slider == decibelSlider) then...
     */
    void sliderValueChanged(Slider*     slider) override;
    
    /** Called when a ComboBox option is choosen (drop Down menu) .
     @param comboBoxThatHasChanged, use this for filtering the button that is actually pressed if (comboBoxThatHasChanged == selectInput) then...
     */
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    /** Called when a Label option is choosen (drop Down menu) .
     @param labelTextChanged, use this for filtering the button that is actually pressed if (labelThatHasChanged == selectInput) then...
     */
    void labelTextChanged (Label *labelThatHasChanged)override;

    /** Called the inputs available from the AudioDeviceManager changes,
     \n the callback is made listening to the AMDinfo Wrapper
     @see retrieveInputs
     */
    void changeListenerCallback (ChangeBroadcaster*)override;
// =========================================================================================================
    

    /** It translates the inputs Strings from the ComboBox (ex : "1 + 2" or "1"), into
     channel references (inputL or inputR).
     */
    void retrieveInputs(String fromComboBox);
    
    /** When the Inputs available are changed, it updates the channel references (inputL or inputR).
     * \n This is because sometimes if only input 1 and 3 are available, then input 3 is the channel (2) and not (3)
     */
    void updateInputs();
    
    
    /** It translates the inputs Strings from the ComboBox (ex : "1 + 2" or "1"), into
     channel references (outputL or outputR).
     */
    void retrieveOutputs(String fromComboBox);
    
    /** When the Outputs available are changed, it updates the channel references (outputL or outputR).
     * \n This is because sometimes if only output 1 and 3 are available, then output 3 is the channel (2) and not (3)
     */
    void updateOutputs();
    
    void timerCallback()override;
    
    
    /** It is called by the mainComponent each time we are close to the next Bar :
     * \n  Update the Playback Object what bar will be,
     * \n  Update the Recorder if it has to stop record
     */
    void checkEvent(int bar);
    
    /** It is called by the mainComponent each time we get to a new Bar :
     * \n  Update the Playback Object what bar it is, and the playback eventually will trigger the sounds
     * \n  Update the Recorder if it has to start record
     */
    void triggerEvent();

// ===============   INITIALISATION Methods  ==================================
    void initialise();
    void initialiseMeter();
    void initialiseSlider();
    void initialiseInputSelector();
    void initialiseOutputSelector();
    void initialisePedalSelector();
    void initialiseNameOfTrack();
    void initialiseButton();

// ===============  VALUE TREE methods ==========================================
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override;
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override;
    void valueTreeParentChanged (ValueTree&) override;
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    
// ===============   FROM Pedals Methods  ==================================
   
    void routeMidiMessage();
    
    void firstFootSwitch();
    void secondFootSwitch();
    void thirdFootSwitch();
    void fourthFootSwitch();
    void fifthFootSwitch();
    void sixthFootSwitch();
    

    /** It is the function that create a folder with the name of the Track,
     it is called during initialisation and give the name of the actual Date and Hour,
     \* It is called each time the name is changed trhough the labelNameOfTrack
     */
    
    void save();
    
    void selectFolder(String name);
    
    void setDefaultLookAndFeelColours();
    
    String getColour();

    void updateGuiAndPedals();

    ValueTree tree;
    UndoManager& undoManager;
    OwnedArray<MidiMessageCollector>& midiCollectorFromPedals;


private:
 
    
// CLASSES and INTERFACEs
//==============================================================================
   
    AudioRecorder recorder; //! class used to Record sound into .wav files.
    Playback playback ; //! class which manage the playback of the Loops
    
    ADMinfo& admInfo; //! Wrapper class of AudioDeviceManager, gives information about available IO
    
    AudioSourcePlayer audioSourcePlayer;
   
    AudioFormatManager formatManager;
    
CustomLookAndFeel2 customLookAndFeel2;
CustomLookAndFeel customLookAndFeel;

//    MidiKeyboardState& keyboardState;
    

// GUI
//==============================================================================
    
    std::unique_ptr<LightPressableButton> lightPressableButton;
    
    DecibelSlider decibelSlider ;
    Label decibelLabel ;
    
    String colour;
   
    GradientLabel labelNameOfTrack;
    
    Label labelSelectInputs;
    ComboBox selectInputs;
    
    Label labelSelectOutputs;
    ComboBox selectOutputs;
   
    Label labelSelectPedals;
    ComboBox selectPedals;
    
    
    TextButton undoButton {"Undo"};
    TextButton saveButton{"Save"};
    TextButton newSongButton{"New Song"};
    TextButton stopImmediatellyButton{"Stop"};
    
    ScopedPointer<Drawable> playButtonImage;
    ScopedPointer<Drawable> stopButtonImage;
    ScopedPointer<Drawable> recordButtonImage;
    ScopedPointer<Drawable> stopRecordButtonImage;
    
    DrawableButton DrawablePlayButton {"playButton", DrawableButton::ImageFitted };
    DrawableButton DrawableRecordButton {"bpmDisplayButton", DrawableButton::ImageStretched };
    
    
// METER
//==============================================================================
    FFAU::LevelMeterSource levelMeterSource;
    FFAU::LevelMeterLookAndFeel levelMeterLookAndFeel;
    FFAU::LevelMeter meter;
    
    
// AUDIOBUFFERs
//==============================================================================
    AudioBuffer<float> bufferToRecordL;
    AudioBuffer<float> bufferToRecordR;
    AudioBuffer<float> bufferToRecordStereo;
    
    AudioBuffer<float> bufferOutL;
    AudioBuffer<float> bufferOutR;
    AudioBuffer<float> bufferOutStereo;
    
// AUDIO ROUTING
//==============================================================================
    bool isStereo;
    int inputL = 0;
    int inputR;
    
    bool outIsStereo = true;
    int outputL = 1;
    int outputR = 2;
    
    
    
// ===============   FROM Pedals  Attributes ==============
    
    int firstPedalPressed = 0;
    MidiBuffer midiBufferFromPedal ;
    String lastThingDoneFirstButton ;
    String lastThingDoneSecondButton;
    String lastThingDoneGeneral ;
    
// Others
//==============================================================================
    
    std::atomic<float> sliderValue;
    
    String nameOfTrack { Time::getCurrentTime().toString(true, false)};
    File actualFolder;
    File oldFolder;
    File savedFolder;
    File lastRecording;
    
    RecordState recordState { RecordState::Playing };
    float loopUnit;
    
    bool isStartingRecording = false;
    bool isStoppingRecording = false;
    
    StringArray inputsAvailables ;
    StringArray outputsAvailables ;
    
    Array<int>& pedalAvailable;
    int nPedal = 0;
    
    
   int lastMidiNote = 0;
   bool  updateGui = true ;

    
};



