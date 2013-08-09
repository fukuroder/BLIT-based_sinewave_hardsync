#pragma once
#include "BLIT_based_sinewave_hardsync_oscillator_note.h"
#include "pluginterfaces/vst/ivstevents.h"

namespace Steinberg{ namespace Vst{

	/// Note class
	class BLIT_based_sinewave_hardsync_oscillator_note
	{
	public:
		// Constructor
		BLIT_based_sinewave_hardsync_oscillator_note();

		// Note on
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

		// 
		double	t;

		// 
		double	sin;

		// 
		unsigned int n;

		// 
		double	dt;

		//
		int srate;

		//
		double blit;

		ADSR adsr()const{return _adsr;};

	protected:

		// ADSR.
		ADSR	_adsr;

		NoteOnEvent _noteOn;

		// 
		static const int _note_no_center = 69;
	};
}} // namespace
