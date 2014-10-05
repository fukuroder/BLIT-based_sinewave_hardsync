#pragma once
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "BLITSineHardSync_oscillator.h"
#include <array>

using namespace Steinberg;
using namespace Steinberg::Vst;
namespace MyVst{
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
			int32 numOuts);
		virtual tresult PLUGIN_API process(ProcessData& data);

	protected:

		// parameter ID
		enum
		{
			Leak,
			Slave
		};

		// notes
		std::array<BLITSineHardSync_note, 8> _notes;

		// oscillator
		BLITSineHardSync_oscillator _blit;
	};
} // namespace
