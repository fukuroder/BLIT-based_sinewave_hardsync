/*
 * BLITSineHardSync_voice.cpp
 *
 * Copyright (c) 2017, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#include "BLITSineHardSync_voice.h"
#include "BLITSineHardSync_sound.h"

//
BLITSineHardSync_voice::BLITSineHardSync_voice()
: t(0)
, n(0)
, dt(0)
, blit(0.0)
, value(0.0)
{
}

//
bool BLITSineHardSync_voice::canPlaySound (SynthesiserSound* sound)
{
    return dynamic_cast<BLITSineHardSync_sound*>(sound) != nullptr;
}

//
void BLITSineHardSync_voice::startNote (int midiNoteNumber, float velocity,
                SynthesiserSound* /*sound*/,
                int currentPitchWheelPosition)
{
	_midiNoteNumber = midiNoteNumber;
	_currentPitchWheelPosition = currentPitchWheelPosition;
	_velocity = velocity;

	double freq = 440.0 * pow(2.0, (midiNoteNumber - 69) / 12.0 + (currentPitchWheelPosition - 8192.0) / 8192.0);
    this->n = static_cast<int>(getSampleRate() / 2.0 / freq);
    this->dt = static_cast<int32_t>(0xFFFFFFFFU / getSampleRate() * freq);
    this->blit = 0.0;
    this->t = INT32_MIN;
    this->value = 0.0;
}

//
void BLITSineHardSync_voice::stopNote (float /*velocity*/, bool allowTailOff)
{
    clearCurrentNote();
}

//
void BLITSineHardSync_voice::pitchWheelMoved (int newValue)
{
	_currentPitchWheelPosition = newValue;
	double freq = 440.0 * pow(2.0, (_midiNoteNumber - 69) / 12.0 + (_currentPitchWheelPosition - 8192.0) / 8192.0);
	this->n = static_cast<int>(getSampleRate() / 2.0 / freq);
	this->dt = static_cast<int32_t>(0xFFFFFFFFU / getSampleRate() * freq);
}

//
void BLITSineHardSync_voice::controllerMoved (int /*controllerNumber*/, int /*newValue*/)
{
}

//
void BLITSineHardSync_voice::renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    processBlock (outputBuffer, startSample, numSamples);
}

//
void BLITSineHardSync_voice::renderNextBlock (AudioBuffer<double>& outputBuffer, int startSample, int numSamples)
{
    processBlock (outputBuffer, startSample, numSamples);
}
    
//
template <typename FloatType> void BLITSineHardSync_voice::processBlock (AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
{
    BLITSineHardSync_sound* sound = dynamic_cast<BLITSineHardSync_sound*>(this->getCurrentlyPlayingSound().get());
    
    if( sound == nullptr )
    {
        return;
    }
    
    while (--numSamples >= 0)
    {
        for (int i = outputBuffer.getNumChannels()-1; i >= 0; --i)
        {
            outputBuffer.addSample(i, startSample, this->value);
        }
        sound->next(this);
        startSample++;
    }
}




