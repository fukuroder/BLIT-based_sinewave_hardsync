/*
 * BLITSineHardSync_voice.cpp
 *
 * Copyright (c) 2017, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#include "BLITSineHardSync_voice.h"
#include "BLITSineHardSync_sound.h"

#define M_PI       3.14159265358979323846   // pi

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
                SynthesiserSound* sound,
                int currentPitchWheelPosition)
{
	_midiNoteNumber = midiNoteNumber;
	_currentPitchWheelPosition = currentPitchWheelPosition;
	_velocity = velocity;
	_envelope = 0.0;
	_state = state::Attack;

	BLITSineHardSync_sound *bsound = static_cast<BLITSineHardSync_sound*>(sound);

	double freq = 440.0 * pow(2.0, (midiNoteNumber - 69) / 12.0 + (currentPitchWheelPosition - 8192.0) / 8192.0);
    this->n = static_cast<int>(getSampleRate() / 2.0 / freq);
    this->dt = static_cast<int32_t>(0xFFFFFFFFU / getSampleRate() * freq);
    this->blit = 0.0;
    this->t = INT32_MIN;
    this->value = 0.0;
	this->_attack = bsound->getAttack();
	this->_cutoff = bsound->getCutoff();

	_filter.update(freq*this->_cutoff, 20);
	_filter.reset_buffer();
}

//
void BLITSineHardSync_voice::stopNote (float /*velocity*/, bool allowTailOff)
{
	if (allowTailOff)
	{
		_state = state::Release;
	}
	else {
		clearCurrentNote();
	}
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
void BLITSineHardSync_voice::setAttack(double attack)
{
	_attack = attack;
}

//
void BLITSineHardSync_voice::setCufoff(double cutoff)
{
	_cutoff = cutoff;
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
            outputBuffer.addSample(i, startSample, static_cast<float>(this->value));
        }
        sound->next(this);
        startSample++;
    }
}

// コンストラクタ
biquad_filter::biquad_filter()
{
}

void biquad_filter::next()
{
	_cutoff *= 0.9999;
	update(_cutoff, _resonance);
}

// 実行
double biquad_filter::process(double in)
{
	const double denormal_canceler = 1.0e-100;
	const double out = _a0*in + _a1*_in_z + _a2*_in_zz + _b1*_out_z + _b2*_out_zz;
	_in_zz = _in_z;
	_in_z = in;
	_out_zz = _out_z + denormal_canceler;
	_out_z = out + denormal_canceler;
	return out;
}

// フィルタ係数更新
void biquad_filter::update(double cutoff, double resonance)
{
	if (resonance < 1.0e-3)
	{
		throw std::runtime_error("resonance < 1.0e-3");
	}

	_cutoff = cutoff;
	_resonance = resonance;

	const double s = ::tan(cutoff * M_PI / 44100.0);
	const double t = s / resonance;
	const double u = s*s + t + 1.0;

	_a0 = s*s / u;
	_a1 = 2.0*s*s / u;
	_a2 = s*s / u;
	_b1 = -2.0*(s*s - 1.0) / u;
	_b2 = -(s*s - t + 1.0) / u;
}

// バッファのリセット
void biquad_filter::reset_buffer()
{
	_in_z = 0.0; _in_zz = 0.0; _out_z = 0.0; _out_zz = 0.0;
}