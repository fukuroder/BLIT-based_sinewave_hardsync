/*
 * BLITSineHardSync_entry.cpp
 *
 * Copyright (c) 2017, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#include "BLITSineHardSync_processor.h"
#include "BLITSineHardSync_controller.h"
#include "BLITSineHardSync_guids.h"
#include "public.sdk/source/main/pluginfactory.h"

 //
bool InitModule()
{
    // nothing to do
    return true;
}

//
bool DeinitModule()
{
    // nothing to do
    return true;
}

using namespace Steinberg::Vst;

// define factory
BEGIN_FACTORY_DEF(
    "fukuroda",                     // vendor
    "https://github.com/fukuroder", // url
    "mailto:fukuroder@live.jp")     // e-mail

    // register processor
    DEF_CLASS2(
        INLINE_UID_FROM_FUID(BLITSineHardSyncProcessorID), // processor's GUID
        PClassInfo::kManyInstances,                        // ?
        kVstAudioEffectClass,                              // category
        "BLITSineHardSync",                                // plug-in name
        kDistributable,                                    // Component flag
        PlugType::kInstrumentSynth,                        // sub category
        "1.0.0.000",                                       // plug-in version
        kVstVersionString,                                 // VSTSDK version
        MyVst::BLITSineHardSync_processor::create)         // create method

        // register controller
    DEF_CLASS2(
        INLINE_UID_FROM_FUID(BLITSineHardSyncControllerID), // controller's GUID
        PClassInfo::kManyInstances,                         // ?
        kVstComponentControllerClass,                       // category
        "BLITSineHardSync Controller",                      // plug-in name
        0,                                                  // N/A
        "",                                                 // N/A
        "1.0.0.000",                                        // plug-in version
        kVstVersionString,                                  // VSTSDK version
        MyVst::BLITSineHardSync_controller::create)         // create method
END_FACTORY
