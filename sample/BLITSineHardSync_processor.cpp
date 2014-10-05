#include "BLITSineHardSync_processor.h"
#include "BLITSineHardSync_guids.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include <algorithm>

namespace MyVst{

//-------------------------------------------------------------------------
// BLITSineHardSync_processor Implementation
//-------------------------------------------------------------------------
BLITSineHardSync_processor::BLITSineHardSync_processor()
{
	setControllerClass(BLITSineHardSyncControllerID);
}

//
FUnknown* BLITSineHardSync_processor::create(void* context)
{
	return (IAudioProcessor*)new BLITSineHardSync_processor();
}

//
tresult PLUGIN_API BLITSineHardSync_processor::initialize(FUnknown* context)
{
	// base class initialization 
	tresult result = AudioEffect::initialize(context);
	if(result != kResultOk)
	{
		return result;
	}

	// set bus
	addAudioOutput(STR16("Stereo Out"), SpeakerArr::kStereo);

	return kResultOk;
}

//
tresult PLUGIN_API BLITSineHardSync_processor::setBusArrangements(
	SpeakerArrangement* inputs,
	int32 numIns,
	SpeakerArrangement* outputs,
	int32 numOuts)
{
		if (numIns == 0 && numOuts == 1 && outputs[0] == SpeakerArr::kStereo)
		{
			return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
		}
		return kResultFalse;
}

//
tresult PLUGIN_API BLITSineHardSync_processor::setProcessing (TBool state)
{
	if( state == 1)
	{
		// setup
		for(auto note = _notes.begin(); note != _notes.end(); ++note)
		{
			// set sample rate
			note->setSampleRate( processSetup.sampleRate );
		}
	}

	return kResultOk;
}

//
tresult PLUGIN_API BLITSineHardSync_processor::process(ProcessData& data)
{
	//-------------------
	// update parameters
	//-------------------
	if( data.inputParameterChanges )
	{
		int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
		for( int32 ii = 0; ii < numParamsChanged; ii++ )
		{
			IParamValueQueue* paramQueue = data.inputParameterChanges->getParameterData(ii);
			if( paramQueue )
			{
				int32 offsetSamples;
				double valueNormalized;

				// get parameter
				if(paramQueue->getPoint(paramQueue->getPointCount() - 1, offsetSamples, valueNormalized) == kResultTrue)
				{
					ParamID id = paramQueue->getParameterId();
					if( id == Leak )
					{
						// -> [0.99, 1.0]
						double value = 0.99 + 0.01 * valueNormalized;  
						_blit.setLeak(value);
					}
					else if( id == Slave )
					{
						// -> [1.0, 2.0]
						double value = 1.0 + valueNormalized;  
						_blit.setSlave(value);
					}
				}
			}
		}
	}

	//----------------
	// process events
	//----------------
	if( data.inputEvents )
	{
		int nEventCount = data.inputEvents->getEventCount();

		for( int ii = 0; ii < nEventCount; ii++ )
		{
			Event e;
			tresult result = data.inputEvents->getEvent(ii, e);
			if( result != kResultOk )continue;

			if( e.type == Event::kNoteOnEvent )
			{
				// find available note
				auto available_note = std::find_if(
					_notes.begin(),
					_notes.end(), 
					[](const BLITSineHardSync_note& n){return n.adsr() == BLITSineHardSync_note::Off;}); 

				if( available_note != _notes.end() )
				{
					// note on
					available_note->trigger( e.noteOn );
				}
			}
			else if( e.type == Event::kNoteOffEvent )
			{
				int32 note_id = e.noteOff.noteId;
				auto target_note = std::find_if(
					_notes.begin(),
					_notes.end(), 
					[note_id](const BLITSineHardSync_note& n){return n.id() == note_id;});

				if( target_note != _notes.end() )
				{
					// note off
					target_note->release();
				}
			}
		}
	}

	bool bAllSilent= std::all_of(
		_notes.begin(),
		_notes.end(),
		[](const BLITSineHardSync_note& n){return n.adsr() == BLITSineHardSync_note::Off;});

	if( bAllSilent )
	{
		return kResultOk;
	}

	// 
	if (data.numInputs == 0 && data.numOutputs == 1 && data.outputs[0].numChannels == 2 )
	{
		float** out = data.outputs[0].channelBuffers32;

		const int32 sampleFrames = data.numSamples;
		for( int ii = 0; ii < sampleFrames; ii++ )
		{
			double value = 0.0;
			for(auto note = _notes.begin(); note != _notes.end(); ++note)
			{	
				if( note->adsr() == BLITSineHardSync_note::Off )continue;

				// add
				value += note->sin * note->velocity();

				// update oscillater
				_blit.updateOscillater( *note );
			}

			// set output buffer
			out[0][ii] = out[1][ii] = static_cast<float>( value );
		}
	}
	return kResultOk;
}

} // namespace
