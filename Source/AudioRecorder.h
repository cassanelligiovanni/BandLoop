//
//  AudioRecorder.hpp
//  IoDevice - App
//
//  Created by Giovanni Cassanelli on 05/02/2019.
//



#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AudioRecorder
{
public:
    
    AudioRecorder(String name, const File& file);
    
    /**
     * dtor.
     */
    
    ~AudioRecorder();
    
    /**
     * dtor.
     */
    
    
    void startRecording (AudioBuffer<float>* bufferToRecord, const File& file);
    
    /**
     * dtor.
     */
    
    void stop();
    
    /**
     * dtor.
     */
    
    void stopRecording();
    
    bool isRecording() const;
    
    void Record(AudioBuffer<float>* bufferToRecord);

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    
//
//    void Record(const AudioSourceChannelInfo& bufferToFill, const File& file, StringArray inputs);
    
    
    void changeName(String name);

private:
    
     AudioBuffer<float> toRecord;
    
    TimeSliceThread backgroundThread; // the thread that will write our audio data to disk
    
    std::unique_ptr<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
    
    double sampleRate ;
    
    int64 nextSampleNum ;
    
    CriticalSection writerLock;
    
    std::atomic<AudioFormatWriter::ThreadedWriter*> activeWriter ;
    
     AudioFormatWriter* wavWriter;
    
    File lastRecording;
    
    String toRecordName ;

    String mood = "";
    
    OwnedArray<AudioBuffer<float>> inputs;
    
    WavAudioFormat format;
    std::unique_ptr<AudioFormatWriter> writer = nullptr;
    
};





