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
			//
			Off,

			//
			On,
		};

		//
		ADSR envelope;

		//
		double t;

		// 
		double sin;

		//
		double blit;

		//
		int n;

		//
		double dt;

		//
		int32 note_id;

		//
		double velocity;

	protected:

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

		//
		void trigger(const NoteOnEvent& noteOn, double srate);

		//
		void release(const NoteOffEvent& noteOff);

		//
		bool is_silent();

		//
		double render();

		//
		void next();

	protected:

		//
		static const int _note_no_center = 69;

		//
		std::array<BLITSineHardSync_note, 8> _notes;

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
		double LinearInterpolatedSin(double x)const;

		// 
		double BLIT(double t, int n)const;
	};
} // namespace
