#pragma once
#include <array>

namespace Steinberg{ namespace Vst{

	// note class
	class BLIT_based_sinewave_hardsync_oscillator_note;

	// oscillator class
	class BLIT_based_sinewave_hardsync_oscillator
	{
	public:
		// constructor
		BLIT_based_sinewave_hardsync_oscillator();

		// leak
		void setLeak(double value);

		// slave
		void setSlave(double value);

		// update
		void updateOscillater(BLIT_based_sinewave_hardsync_oscillator_note& note);


	protected:
		// sine table
		std::array<double, (1<<13)+1> _sinTable;

		// leak
		double _leak;

		//
		double _zzz;

		//
		std::array<double, 3> _b;

		// 
		double LinearInterpolatedSin(double t);

		// 
		double BLIT(double t, int endN);
	};

}} // namespace
