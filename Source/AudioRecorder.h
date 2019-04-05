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
    
    AudioRecorder();
    
    /**
     * dtor.
     */
    
    ~AudioRecorder();
    
    /**
     * dtor.
     */
    
    void setSampleRate(double SampleRate);
    
    void startRecording (const File& file);
    
    /**
     * dtor.
     */
    
    void stop();
    
    /**
     * dtor.
     */
    
    void stopRecording();
    
    bool isRecording() const;
    
    void Record (float** pointers, int numSamples);

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    
//
//    void Record(const AudioSourceChannelInfo& bufferToFill, const File& file, StringArray inputs);
    
    
    void changeName(String name);

private:
    
     AudioBuffer<float> toRecord;
    
//    TimeSliceThread backgroundThread; // the thread that will write our audio data to disk
    

    TimeSliceThread backgroundThread { "Audio Recorder Thread" }; // the thread that will write our audio data to disk
    std::unique_ptr<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
    double sampleRate = 0.0;
    int64 nextSampleNum = 0;
    
    CriticalSection writerLock;
    std::atomic<AudioFormatWriter::ThreadedWriter*> activeWriter { nullptr };
    
    
    File lastRecording;
    
    String toRecordName ;

    String mood = "";
    
    OwnedArray<AudioBuffer<float>> inputs;
    
    int numSamples;
    int startSamples;
    int numChannels;
    
    std::unique_ptr<AudioFormatWriter> writer = nullptr;
    
};





