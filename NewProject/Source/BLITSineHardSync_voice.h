/*
 * BLITSineHardSync_voice.h
 *
 * Copyright (c) 2017, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


class biquad_filter
{
protected:

	// フィルタ係数
	double _a0, _a1, _a2, _b1, _b2;

	// バッファ
	double _in_z, _in_zz, _out_z, _out_zz;

	double _cutoff;
	double _resonance;

public:
	// コンストラクタ
	biquad_filter();

	// 実行
	double process(double in);

	void next();

	// フィルタ係数更新
	void update(double cutoff, double resonance);

	// バッファのリセット
	void reset_buffer();
};


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
	void setAttack(double attack);
	
	//
	void setCufoff(double cutoff);
    
    //
    int32_t t;
    
    //
    int32_t n;
    
    //
    int32_t dt;
    
    //
    double blit;
    
    //
    double value;

	//
	int _midiNoteNumber;

	//
	int _currentPitchWheelPosition;

	//
	double _velocity;

	enum state {
		Attack,
		Sustain,
		Release
	} _state;
	
	//
	double _envelope;

	double _attack;

	double _cutoff;

	//
	biquad_filter _filter;
private:
    
    //
    template <typename FloatType>
    void processBlock(AudioBuffer<FloatType>& outputBuffer,
                      int startSample,
                      int numSamples);
};
