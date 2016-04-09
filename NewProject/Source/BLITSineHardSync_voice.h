/*
 * BLITSineHardSync_voice.h
 *
 * Copyright (c) 2016, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

//
class BLITSineHardSync_voice  : public SynthesiserVoice
{
public:
    //
    BLITSineHardSync_voice();
    
    //
    bool canPlaySound(SynthesiserSound* sound) override;
    
    //
    void startNote(int midiNoteNumber, float velocity,
                   SynthesiserSound* sound,
                   int currentPitchWheelPosition) override;
    
    //
    void stopNote(float velocity,
                  bool allowTailOff) override;
    
    //
    void pitchWheelMoved(int newValue) override;
    
    //
    void controllerMoved(int controllerNumber,
                         int newValue) override;
    
    //
    void renderNextBlock(AudioBuffer<float>& outputBuffer,
                         int startSample,
                         int numSamples) override;
    
    //
    void renderNextBlock(AudioBuffer<double>& outputBuffer,
                         int startSample,
                         int numSamples) override;
    
    //
    double t;
    
    //
    double blit;
    
    //
    int n;
    
    //
    double dt;
    
    //
    double value;
private:
    
    //
    template <typename FloatType>
    void processBlock(AudioBuffer<FloatType>& outputBuffer,
                      int startSample,
                      int numSamples);
};
