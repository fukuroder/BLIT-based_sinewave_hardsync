#pragma once
#include "public.sdk/source/vst/vsteditcontroller.h"

namespace Steinberg{ namespace Vst{

	// controller
	class BLITSineHardSync_controller : public EditController
	{
		// constructor(private)
		BLITSineHardSync_controller();

	public:
		// create
		static FUnknown* create(void* context);

		// from EditController class
		virtual tresult PLUGIN_API initialize(FUnknown* context);
	};

	// leak parameter class
	class BLITSineHardSync_LeakParameter : public Parameter
	{
	public:
		BLITSineHardSync_LeakParameter();
		virtual void toString(ParamValue normValue, String128 string)const;
	};

	// slave parameter class
	class BLITSineHardSync_SlaveParameter : public Parameter
	{
	public:
		BLITSineHardSync_SlaveParameter();
		virtual void toString(ParamValue normValue, String128 string)const;
	};

	// N parameter class
	class BLITSineHardSync_NParameter : public Parameter
	{
	public:
		BLITSineHardSync_NParameter();
		virtual void toString(ParamValue normValue, String128 string)const;
	};

}} // namespace
