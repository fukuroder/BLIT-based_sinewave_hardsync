#pragma once
#include <array>
#include "pluginterfaces/vst/ivstevents.h"

namespace Steinberg{ namespace Vst{

	// note class
	class BLIT_based_sinewave_hardsync_oscillator_note
	{
	public:
		// constructor
		BLIT_based_sinewave_hardsync_oscillator_note();

		// note on
		void trigger(const NoteOnEvent& noteOn);

		// 
		int32 id()const;

		// 
		double velocity()const;

		// ADSR.
		enum ADSR{
			Off,
			On,
		};

		//
		void setSampleRate(int srate);

		// 
		void release();

		// current position
		double	t;

		// 
		double	sin;

		// Nyquist limit (round down)
		unsigned int n;

		// delta t
		double	dt;

		// 
		int srate;

		//
		double blit;

		ADSR adsr()const{return _adsr;};

	protected:

		// ADSR
		ADSR	_adsr;

		//
		NoteOnEvent _noteOn;

		// 
		static const int _note_no_center = 69;
	};


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
		std::array<double, (1<<10)+1> _sinTable;

		// leak
		double _leak;

		//
		double _b1;

		//
		double _b2;

		//
		double _b3;

		// 
		double LinearInterpolatedSin(double t);

		// 
		double BLIT(double t, int endN);
	};

}} // namespace
