/*
 * BLITSineHardSync_controller.h
 *
 * Copyright (c) 2017, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#pragma once
#include "public.sdk/source/vst/vsteditcontroller.h"

using namespace Steinberg;
using namespace Steinberg::Vst;
namespace MyVst {

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

    // 
    class BLITSineHardSync_SlaveParameter : public Parameter
    {
    public:
        BLITSineHardSync_SlaveParameter();
        virtual void toString(ParamValue normValue, String128 string)const;
    };
} // namespace
