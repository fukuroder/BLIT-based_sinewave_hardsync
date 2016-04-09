/*
 * BLITSineHardSync_sound.h
 *
 * Copyright (c) 2064, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <array>

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
    
    // sine table
    std::array<double, (1 << 10) + 1> _sinTable;
    
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
    double LinearInterpolatedSin(double x)const;
    
    //
    double BLIT(double t, int n)const;
};
