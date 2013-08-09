#pragma once
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "BLIT_based_sinewave_hardsync_oscillator_note.h"
#include "BLIT_based_sinewave_hardsync_oscillator.h"
#include <array>

namespace Steinberg{ namespace Vst{

	//
	class BLITSineHardSync_processor : public AudioEffect
	{
		// constructor(private)
		BLITSineHardSync_processor();

	public:
		// create
		static FUnknown* create(void* context);

		// from AudioEffect class
		virtual tresult PLUGIN_API initialize(FUnknown* context);
		virtual tresult PLUGIN_API setBusArrangements(
			SpeakerArrangement* inputs,
			int32 numIns,
			SpeakerArrangement* outputs,
			int32 numOuts
			);
		virtual tresult PLUGIN_API setProcessing (TBool state);
		virtual tresult PLUGIN_API process(ProcessData& data);

	protected:

		// ÉpÉâÉÅÅ[É^ID
		enum
		{
			Leak,
			Slave
		};

		// notes
		std::array<BLIT_based_sinewave_hardsync_oscillator_note, 8> _notes;

		//
		BLIT_based_sinewave_hardsync_oscillator blit;
	};

}} // namespace
