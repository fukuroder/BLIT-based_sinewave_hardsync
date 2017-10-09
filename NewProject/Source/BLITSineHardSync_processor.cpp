/*
 * BLITSineHardSync_processor.cpp
 *
 * Copyright (c) 2017, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#include "BLITSineHardSync_processor.h"
#include "BLITSineHardSync_voice.h"
#include "BLITSineHardSync_sound.h"

//
BLITSineHardSync_processor::BLITSineHardSync_processor()
{
    _slaveParam = new AudioParameterFloat ("Slave", "Slave", 1.0f, 2.0f, 1.2f);
    addParameter(_slaveParam);

	//_attackParam = new AudioParameterFloat("Attack", "Attack", 0.0f, 1.0f, 0.2f);
	//addParameter(_attackParam);

	//_releaseParam = new AudioParameterFloat("Release", "Release", 0.0f, 1.0f, 0.2f);
	//addParameter(_releaseParam);

	//_cutoffParam = new AudioParameterFloat("Cufoff Freq.", "Cufoff Freq.", 0.0f, 1.0f, 0.2f);
	//addParameter(_cutoffParam);

	//_resonanceParam = new AudioParameterFloat("Resonance", "Resonance", 0.0f, 1.0f, 0.2f);
	//addParameter(_resonanceParam);

	//_egintensityParam = new AudioParameterFloat("EG Int.", "EG Int.", 0.0f, 1.0f, 0.2f);
	//addParameter(_egintensityParam);
    
    const int numVoices = 8;
    
    // add voices
    for (int i = 0; i < numVoices; i++)
    {
        _synth.addVoice (new BLITSineHardSync_voice());
    }
    
    // add sound
    _synth.addSound (new BLITSineHardSync_sound());
}

//
BLITSineHardSync_processor::~BLITSineHardSync_processor()
{
}

//
const String BLITSineHardSync_processor::getName() const
{
    return JucePlugin_Name;
}

//
bool BLITSineHardSync_processor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

//
bool BLITSineHardSync_processor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

//
bool BLITSineHardSync_processor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

//
double BLITSineHardSync_processor::getTailLengthSeconds() const
{
    return 0.0;
}

//
int BLITSineHardSync_processor::getNumPrograms()
{
    return 1;
}

//
int BLITSineHardSync_processor::getCurrentProgram()
{
    return 0;
}

//
void BLITSineHardSync_processor::setCurrentProgram (int index)
{
}

//
const String BLITSineHardSync_processor::getProgramName (int index)
{
    return {};
}

//
void BLITSineHardSync_processor::changeProgramName (int index, const String& newName)
{
}

//
void BLITSineHardSync_processor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    _synth.setCurrentPlaybackSampleRate(sampleRate);
}

//
void BLITSineHardSync_processor::releaseResources()
{
}

//
void BLITSineHardSync_processor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    //-------------------
    // update parameters
    //-------------------
    BLITSineHardSync_sound* sound = static_cast<BLITSineHardSync_sound*>(_synth.getSound(0));
    jassert(sound != nullptr);
    
    sound->setSlave(static_cast<double>(_slaveParam->get()));
    
    //----------------
    // process events
    //----------------
    _synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//
bool BLITSineHardSync_processor::hasEditor() const
{
    return false;
}

//
AudioProcessorEditor* BLITSineHardSync_processor::createEditor()
{
    return nullptr;
}

//
void BLITSineHardSync_processor::getStateInformation (MemoryBlock& destData)
{
}

//
void BLITSineHardSync_processor::setStateInformation (const void* data, int sizeInBytes)
{
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BLITSineHardSync_processor();
}
