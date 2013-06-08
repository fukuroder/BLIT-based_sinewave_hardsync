#pragma once

// include
#include <array>

namespace Steinberg {namespace Vst {

// class
class BLIT_based_sinewave_hardsync_oscillator_note;

//
class BLIT_based_sinewave_hardsync_oscillator
{
public:
	// constructor
	BLIT_based_sinewave_hardsync_oscillator();

	//
	void setLeak(double value);

	//
	void setSlave(double value);

	//
	void updateOscillater(BLIT_based_sinewave_hardsync_oscillator_note& note);


protected:
	//
	std::array<double, (1<<13)+1> _sinTable;
	
	//
	double _Leak;

	//
	double _Slave;

	double _zzz;


	//
	double LinearInterpolatedSin( double iT );

	//
	double BLIT( double t, int startN, int endN );
};

}}