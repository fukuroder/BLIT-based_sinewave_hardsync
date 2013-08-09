#include "BLITSineHardSync_controller.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "pluginterfaces/base/ibstream.h"
#include <math.h>

namespace Steinberg{ namespace Vst{

	//-------------------------------------------------------------------------
	BLITSineHardSync_controller::BLITSineHardSync_controller()
	{
	}

	//-------------------------------------------------------------------------
	FUnknown* BLITSineHardSync_controller::create(void* context)
	{
		return (IEditController*)new BLITSineHardSync_controller();
	}

	//-------------------------------------------------------------------------
	tresult PLUGIN_API BLITSineHardSync_controller::initialize(FUnknown* context)
	{
		tresult result = EditController::initialize(context); 
		if (result != kResultOk)
		{ 
			return result; 
		}

		parameters.addParameter(new BLITSineHardSync_LeakParameter());
		parameters.addParameter(new BLITSineHardSync_SlaveParameter());

		return kResultOk;
	}

	//-------------------------------------------------------------------------
	BLITSineHardSync_LeakParameter::BLITSineHardSync_LeakParameter()
		:Parameter(L"Leak", 0, L"%", 0.5)
	{
	}

	void BLITSineHardSync_LeakParameter::toString(ParamValue normValue, String128 string)const
	{
		::swprintf_s(string, 128, L"%.3f",( 0.99 + 0.01 * normValue ) * 100);
	}

	//-------------------------------------------------------------------------
	BLITSineHardSync_SlaveParameter::BLITSineHardSync_SlaveParameter()
		:Parameter(L"Slave Pitch", 1, L"", 0.125)
	{
	}

	void BLITSineHardSync_SlaveParameter::toString(ParamValue normValue, String128 string)const
	{
		::swprintf_s(string, 128, L"%.3f", 1.1 + 0.8 * normValue );
	}

}} // namespace
