/*
  ==============================================================================

    Sound
    Created: 26 Mar 2019 5:56:09pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

//==============================================================================




#include "Sound.h"

Sound::Sound(File fFolder, File fFile, int fstart, int flength, int fnote){
    
    folder = fFolder;
    file = fFile;
    start = fstart;
    length = flength;
    note =  fnote ;
    
}

Sound::~Sound(){};




int Sound::getStart(){
    
    return start;
    
};

int Sound::getLength(){
    
    return length;
    
};

int Sound::getNote(){
    
    return note;
    
};

void Sound::setStart(int newStart){
    
    start = newStart;
    
};

void Sound::setLength(int newLength){
    
    length = newLength;
    
};
void Sound::setNote(int newNote) {
    
    note = newNote;
    
};
