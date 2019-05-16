/*
  ==============================================================================

    RecordState.h
    Created: 21 Apr 2019 2:58:19pm
    Author:  Giovanni Cassanelli

  ==============================================================================
*/

#pragma once

enum class RecordState
{
    null,
    readyToRecord,
    Recording,
    readyToPlayback,
    Playing,
    Stopping
    
};
