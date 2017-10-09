/*
 * BLITSineHardSync_processor.h
 *
 * Copyright (c) 2017, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

//
class BLITSineHardSync_processor : public AudioProcessor
{
public:
    //
    BLITSineHardSync_processor();
    
    //
    ~BLITSineHardSync_processor();
    
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
    bool isMidiEffect () const override;
    
    //
    double getTailLengthSeconds() const override;

    //
    int getNumPrograms() override;
    
    //
    int getCurrentProgram() override;
    
    //
    void setCurrentProgram (int index) override;
    
    //
    const String getProgramName (int index) override;
    
    //
    void changeProgramName (int index, const String& newName) override;
    
    //
    void getStateInformation (MemoryBlock& destData) override;
    
    //
    void setStateInformation (const void* data, int sizeInBytes) override;
    
private:
    //
	AudioParameterFloat* _slaveParam;

	//
	AudioParameterFloat* _attackParam;

	//
	AudioParameterFloat* _releaseParam;

	//
	AudioParameterFloat* _cutoffParam;

	//
	AudioParameterFloat* _resonanceParam;

	//
	AudioParameterFloat* _egintensityParam;
    
    //
    Synthesiser _synth;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BLITSineHardSync_processor)
};
