#pragma once
#include <array>
#include "pluginterfaces/vst/ivstevents.h"

namespace Steinberg{ namespace Vst{

	// note class
	class BLITSineHardSync_note
	{
	public:
		// constructor
		BLITSineHardSync_note();

		// note on
		void trigger(const NoteOnEvent& noteOn);

		// 
		int32 id()const;

		// 
		double velocity()const;

		// ADSR
		enum ADSR
		{
			Off,
			On
		};

		//
		void setSampleRate(int srate);

		// 
		void release();

		// current position
		double t;

		// 
		double sin;

		// Nyquist limit (round down)
		unsigned int n;

		// delta t
		double dt;

		// 
		int srate;

		//
		double blit;

		ADSR adsr()const;

	protected:

		// ADSR
		ADSR	_adsr;

		//
		NoteOnEvent _noteOn;

		// 
		static const int _note_no_center = 69;
	};


	// oscillator class
	class BLITSineHardSync_oscillator
	{
	public:
		// constructor
		BLITSineHardSync_oscillator();

		// leak
		void setLeak(double value);

		// slave
		void setSlave(double value);

		// update
		void updateOscillater(BLITSineHardSync_note& note);

	protected:
		// sine table
		std::array<double, (1<<10)+1> _sinTable;

		// leak
		double _leak;

		// Fourier coefficient for sin(2*PI*1*t)
		double _b1;

		// Fourier coefficient for sin(2*PI*2*t)
		double _b2;

		// used for BLIT section
		double _b3;

		// 
		double LinearInterpolatedSin(double t);

		// 
		double BLIT(double t, int endN);
	};

}} // namespace
