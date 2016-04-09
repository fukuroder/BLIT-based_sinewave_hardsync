/*
 * BLITSineHardSync_processor.h
 *
 * Copyright (c) 2016, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <array>

//
class BLITSineHardSync_processor : public AudioProcessor
{
public:
    //
    BLITSineHardSync_processor();
    
    //
    virtual ~BLITSineHardSync_processor();
    
    //
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    
    //
    void releaseResources() override;
    
    //
    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    
    //
    AudioProcessorEditor* createEditor() override;
    
    //
    bool hasEditor() const override;
    
    //
    const String getName() const override;
    
    //
    bool acceptsMidi() const override;
    
    //
    bool producesMidi() const override;
    
    //
    double getTailLengthSeconds() const override;
    
    //
    int getNumPrograms() override;
    
    //
    int getCurrentProgram() override;
    
    //
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
private:
    //
    AudioParameterFloat* _slaveParam;
    
    //
    Synthesiser synth;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BLITSineHardSync_processor)
};
