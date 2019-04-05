//
//  AudioRecorder.cpp
//  IoDevice - App
//
//  Created by Giovanni Cassanelli on 05/02/2019.
//

#include "AudioRecorder.h"

AudioRecorder::AudioRecorder():
//backgroundThread("Audio Recorder Thread")
 threadedWriter() // the FIFO used to buffer the incoming data
, sampleRate(0.0)
, nextSampleNum(0)
, writerLock()
, activeWriter(nullptr)
{
    
    backgroundThread.startThread();
    
    
}

AudioRecorder::~AudioRecorder()    {
    //    stop();
}

//==============================================================================
//Prepare to record a new File

void AudioRecorder::startRecording (const File& file)
{
    stop();
    
    if (sampleRate > 0)
    {
        DBG("startRecording");
        // Create an OutputStream to write to our destination file...
        file.deleteFile();
        
        if (auto fileStream = std::unique_ptr<FileOutputStream> (file.createOutputStream()))
        {
            // Now create a WAV writer object that writes to our output stream...
            WavAudioFormat wavFormat;
            
            if (auto writer = wavFormat.createWriterFor (fileStream.get(), sampleRate, 1, 16, {}, 0))
            {
                fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)
                
                // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
                // write the data to disk on our background thread.
                threadedWriter.reset (new AudioFormatWriter::ThreadedWriter (writer, backgroundThread, 32768));
                
                
                // And now, swap over our active writer pointer so that the audio callback will start using it..
                const ScopedLock sl (writerLock);
                activeWriter = threadedWriter.get();
            }
        }
    }
}

//=============================================================================
//Actual Recording

void AudioRecorder::Record (float** pointers, int numSamples)
{
    const ScopedLock sl (writerLock);
    if (activeWriter.load() != nullptr) {
        activeWriter.load()->write (pointers, numSamples);

    }
}

//=============================================================================
//Stop Recording
void AudioRecorder::stop()
{
    // First, clear this pointer to stop the audio callback from using our writer object..
    {
        const ScopedLock sl (writerLock);
        activeWriter = nullptr;
    }
    
    // Now we can delete the writer object. It's done in this order because the deletion could
    // take a little time while remaining data gets flushed to disk, so it's best to avoid blocking
    // the audio callback while this happens.
    threadedWriter.reset();
}

void AudioRecorder::stopRecording()
{
    lastRecording = File();
    this->stop();
    
}


//bool AudioRecorder::isRecording() const
//{
//    return wavWriter != nullptr;
//}

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


void AudioRecorder::setSampleRate(double SampleRate) {
    
    sampleRate = SampleRate;
    
}
