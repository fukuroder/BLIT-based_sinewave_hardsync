#pragma once
#include <array>
#include "pluginterfaces/vst/ivstevents.h"

using namespace Steinberg;
using namespace Steinberg::Vst;
namespace MyVst{
	// note class
	class BLITSineHardSync_note
	{
	public:
		// constructor
		BLITSineHardSync_note();

		// ADSR
		enum ADSR
		{
			Off,
			On
		};

		// note on
		void trigger(const NoteOnEvent& noteOn, double srate);

		// 
		int32 id()const;

		// 
		double velocity()const;

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
		double blit;

		//
		ADSR adsr()const;

	protected:

		// ADSR
		ADSR _adsr;

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
		std::array<double, (1 << 10) + 1> _sinTable;

		// leak
		double _leak;

		// Fourier coefficient for sin(2*PI*1*t)
		double _b1;

		// Fourier coefficient for sin(2*PI*2*t)
		double _b2;

		// used for BLIT section
		double _b3;

		// 
		double LinearInterpolatedSin(double t)const;

		// 
		double BLIT(double t, int N)const;
	};
} // namespace
