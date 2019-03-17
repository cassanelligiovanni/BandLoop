//
//  AudioRecorder.cpp
//  IoDevice - App
//
//  Created by Giovanni Cassanelli on 05/02/2019.
//

#include "AudioRecorder.h"

AudioRecorder::AudioRecorder(String name, const File& file): backgroundThread("Audio Recorder Thread")
, threadedWriter() // the FIFO used to buffer the incoming data
, sampleRate(0.0)
, nextSampleNum(0)
, writerLock()
, activeWriter(nullptr)
{
    
    backgroundThread.startThread();
    lastRecording = file;

    
}

AudioRecorder::~AudioRecorder()    {
    //    stop();
}

//==============================================================================
//Prepare to record a new File

void AudioRecorder::startRecording (AudioBuffer<float>* bufferToRecord, const File& file)
{
    stop();
    
    //    file.deleteFile();
    
    //get numChannel based on the track need (Mono or Stereo)
    auto numChannels = bufferToRecord->getNumChannels();
    
    
    //create a new file based on the passed argument
    
    writer.reset (format.createWriterFor (new FileOutputStream (file),
                                          44100.0,
                                          numChannels,
                                          16,
                                          {},
                                          0));
}

//=============================================================================
//Actual Recording

void AudioRecorder::Record(AudioBuffer<float>* bufferToRecord)
{
    const auto numSamples = bufferToRecord->getNumSamples();
    if (writer != 0)
    {
        writer->writeFromAudioSampleBuffer(*bufferToRecord, 0, numSamples);
    }
}

//=============================================================================
//Stop Recording
void AudioRecorder::stop()
{
    writer = nullptr;
}

void AudioRecorder::stopRecording()
{
    this->stop();
    lastRecording = File();
}



bool AudioRecorder::isRecording() const
{
    return wavWriter != nullptr;
}

//=============================================================================

void AudioRecorder::changeName(String name )
{
    toRecordName = name;
}


//void AudioRecorder::changeFile(const File& file) {
//    
//    
//    
//}


