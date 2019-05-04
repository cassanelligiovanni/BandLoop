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
    
  
    
    ~AudioRecorder();
    

    /**
     * Prepare to record ( create an OutputStream, a WavWriter and configure the ActiveWriter )
     @param File, the file to write on
     */
    void startRecording (const File& file);
    
    /**
     * Recording..... by writing on the ActiveWriter
     */
    void  Record (float** pointers, int numSamples);
    
    /**
     *  reset the ActiveWriter
     */
    void stopRecording();
    
    /**
     * Stop the Recording..... call the stopRecording()
     */
    void stop();
    
    /**
     * Set SampleRate
     */
    void setSampleRate(double SampleRate);

    void changeName(String name);

private:

    double sampleRate = 0.0;

    File lastRecording;
    String toRecordName ;
    
    int numSamples;
    int startSamples;
    int numChannels;
 
    TimeSliceThread backgroundThread { "Audio Recorder Thread" }; // the thread that will write our audio data to disk
    std::unique_ptr<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
    std::atomic<AudioFormatWriter::ThreadedWriter*> activeWriter { nullptr };
    std::unique_ptr<AudioFormatWriter> writer = nullptr;
    CriticalSection writerLock;
};





