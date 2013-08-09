#pragma once
#include <array>

namespace Steinberg{ namespace Vst{

	// Note class
	class BLIT_based_sinewave_hardsync_oscillator_note;

	// Oscillator class
	class BLIT_based_sinewave_hardsync_oscillator
	{
	public:
		// Constructor
		BLIT_based_sinewave_hardsync_oscillator();

		// Leak
		void setLeak(double value);

		// Slave
		void setSlave(double value);

		// Update
		void updateOscillater(BLIT_based_sinewave_hardsync_oscillator_note& note);


	protected:
		// 
		std::array<double, (1<<13)+1> _sinTable;

		// 
		double _Leak;

		//
		double _zzz;

		//
		std::array<double, 3> _b;

		// 
		double LinearInterpolatedSin( double iT );

		// 
		double BLIT( double t, int startN, int endN );
	};

}} // namespace
