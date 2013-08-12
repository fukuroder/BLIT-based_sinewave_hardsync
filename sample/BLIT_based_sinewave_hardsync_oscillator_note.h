#pragma once
#include "BLIT_based_sinewave_hardsync_oscillator_note.h"
#include "pluginterfaces/vst/ivstevents.h"

namespace Steinberg{ namespace Vst{

	/// note class
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
}} // namespace
