/*
 ==============================================================================
 
 SynthAudioSource.h
 Created: 2 Mar 2019 4:55:36pm
 Author:  Giovanni Cassanelli
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "/Users/giovanni/BandLoop/Source/Sound.h"
#include "/Users/giovanni/BandLoop/Source/RecordState.h"
#pragma once

class Playback : public Component,
public Button::Listener
{
    
public :
    Playback(RecordState& recordState);
    ~Playback();

// ===============   Methods inspired by AUDIO SOURCE  ==================================
    
    /**  This callback allows the source to initialise any resources it might need when playing.
     @param samplesPerBlockExpected  (bufferSize)
     @param sampleRate (Hz)
     */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    
    /** Allows the source to release anything it no longer needs after playback has stopped.
     */
    void releaseResources();
    
    /** This callback will be made each
     time the audio playback hardware (or whatever other destination the audio
     data is going to) needs another block of data.
     */
    void getNextAudioBlock(AudioBuffer<float>& outputBuffer, int numSamples);
    
    /** Add the just recorded ".wav" to the Synthesiser(Sampler), and then create a Sound instance to keep track of
     * its attributes ( length, note, startPoint, etc...)
     */

    // ===============   COMPONENT Methods  ==================================
    
    /**  The paint() Method gets called when a region of a component needs redrawing,
     Never need to call this method directly
     
     @param g    the graphics context that must be used to do the drawing operations.
     */
    void paint (Graphics& g) override;
    
    /** Called when this component's size has been changed.
     */
    void resized() override;
    
    /** Called when a button is pressed
     */
    void buttonClicked (Button* button) override;
    
    
    void updateGui();
    
// ===============   Other Methods  ==================================

    
    void createSound(File lastRecording, File actualFolder, float loopUnit);
    
    /** Called just before the new Bar.
     \n It update the actual Bar from the Global Tempo(BPM.h)
     * It will be used by the triggerEvent()
     * @see triggerEvent
     */
    void checkEvent(int bar);
    
    /** Check wheter a loop is came to an end and has to be trigger again
     */
    void triggerEvent();
    
    /** Stop All sounds immediatelly
     */
    void stopImmediatelly();
    
    /** Start or stop playing again at next bar
     */
    void startOrStopAtNextBar();
    
    /** Advise that te system is starting recording,
     ** therefore we have to switch in PlayMode
     */
    void startRecording();
 
    void newSong(File savedFolder, File actualFolder);
    
    void save(File savedFolder, File actualFolder);
    
    
    
private:
    
    RecordState& recState;
    
    AudioFormatManager formatManager;  
    OwnedArray<Sound> sounds;
    int actualBar;
    
    Array<int> notesAvailable;
    Array<int> notesAssigned;

    
public :
//    MidiMessageCollector& midiCollector;
    
  bool  isStartingPlaying = false;
  bool  isStoppingPlaying = false;
    
    bool isPlaying = true;
    
    
    
    MidiMessageCollector playbackCollector;
    Synthesiser synth;
    
    MidiBuffer midiQueue;
    float loopUnit;
    
    
    ScopedPointer<Drawable> playButtonImage;
    ScopedPointer<Drawable> stopButtonImage;
    
    DrawableButton DrawablePlayButton {"playButton", DrawableButton::ImageFitted };

    
};
