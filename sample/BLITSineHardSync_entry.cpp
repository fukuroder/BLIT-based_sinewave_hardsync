#include "BLITSineHardSync_processor.h"
#include "BLITSineHardSync_controller.h"
#include "BLITSineHardSync_guids.h"
#include "public.sdk/source/main//pluginfactoryvst3.h"

// DLLロード後に呼ばれる
bool InitModule()
{
	// nothing to do
	return true;
}

// DLLアンロード後に呼ばれる
bool DeinitModule()
{
	// nothing to do
	return true;
}

namespace Steinberg{ namespace Vst{
// define factory
BEGIN_FACTORY_DEF(
	"fukuroda",							// vendor
	"https://github.com/fukuroder",		// url
	"mailto:fukuroder@live.jp")			// e-mail

	// register processor
	DEF_CLASS2(
		INLINE_UID_FROM_FUID(BLITSineHardSyncProcessorID),	// processor's GUID
		PClassInfo::kManyInstances,							// ?
		kVstAudioEffectClass,								// category
		"BLIT-based sinewave hardsync",						// plug-in name
		kDistributable,										// component flag
		PlugType::kInstrumentSynth,							// sub category
		"1.0.0.000",										// plug-in version
		kVstVersionString,									// VSTSDK version
		BLITSineHardSync_processor::create)					// create method

	// register controller
	DEF_CLASS2(
		INLINE_UID_FROM_FUID(BLITSineHardSyncControllerID),	// controller's GUID
		PClassInfo::kManyInstances,							// ?
		kVstComponentControllerClass,						// category
		"BLIT-based sinewave hardsync Controller",			// plug-in name
		0,													// N/A
		"",													// N/A
		"1.0.0.000",										// plug-in version
		kVstVersionString,									// create method
		BLITSineHardSync_controller::create)				// create method

END_FACTORY
}} // namespace
