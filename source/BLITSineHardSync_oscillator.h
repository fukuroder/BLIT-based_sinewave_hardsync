/*
 * BLITSineHardSync_oscillator.h
 *
 * Copyright (c) 2017, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#pragma once
#include <array>
#include "pluginterfaces/vst/ivstevents.h"

using namespace Steinberg;
using namespace Steinberg::Vst;
namespace MyVst {
    //
    class BLITSineHardSync_oscillator_note
    {
    public:
        BLITSineHardSync_oscillator_note();

        // ADSR
        enum ADSR
        {
            //
            Off,

            //
            On,
        };

        //
        ADSR envelope;

        //
        int32_t t;

        //
        int32 sampleOffset;

        //
        double blit;

        //
        double value;

        //
        int32_t n;

        //
        int32_t dt;

        //
        int32_t note_id;

        //
        double velocity;
    };

    //
    class BLITSineHardSync_oscillator
    {
    public:
        // constructor
        BLITSineHardSync_oscillator();

        //
        void setSlave(double value);

        //
        void trigger(const NoteOnEvent& noteOn, int32 sampleOffset, double srate);

        //
        void release(const NoteOffEvent& noteOff);

        //
        bool is_silent();

        //
        double render();

        //
        void next();

    protected:

        //
        static const int _note_no_center = 69;

        //
        std::array<BLITSineHardSync_oscillator_note, 8> _notes;

        //
        double _Slave;

        //
        const double _Leak;

        // Fourier coefficient for sin(2*PI*1*t)
        double _b1;

        // Fourier coefficient for sin(2*PI*2*t)
        double _b2;

        // used for BLIT section
        double _b3;

        //
        double BLIT(int32_t t, int n)const;
    };
} //  namespace
