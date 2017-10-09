/*
 * BLITSineHardSync_sound.h
 *
 * Copyright (c) 2017, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class BLITSineHardSync_voice;

//
class BLITSineHardSync_sound : public SynthesiserSound
{
public:
    //
    BLITSineHardSync_sound();
    
    //
    bool appliesToNote(int midiNoteNumber) override;
    
    //
    bool appliesToChannel(int midiChannel) override;
    
    //
    void next(BLITSineHardSync_voice *voice);
    
    // slave
    void setSlave(double value);
    
private:
    // leak
    double _leak;
    
    // slave
    double _slave;
    
    // Fourier coefficient for sin(2*PI*1*t)
    double _b1;
    
    // Fourier coefficient for sin(2*PI*2*t)
    double _b2;
    
    // used for BLIT section
    double _b3;
    
    //
    double blit(int32_t t, int n)const;
};
