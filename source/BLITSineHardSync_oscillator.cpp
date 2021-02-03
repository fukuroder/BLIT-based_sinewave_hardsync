/*
 * BLITSineHardSync_oscillator.cpp
 *
 * Copyright (c) 2017, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "BLITSineHardSync_oscillator.h"
#include "../remez_approx/remez_approx.h"

namespace MyVst {

    // constructor
    BLITSineHardSync_oscillator_note::BLITSineHardSync_oscillator_note()
        :envelope(ADSR::Off)
        , t(0)
        , sampleOffset(0)
        , blit(0.0)
        , value(0.0)
        , n(0)
        , dt(0)
        , note_id(0)
        , velocity(0.0)
    {
    }

    //
    void BLITSineHardSync_oscillator::trigger(const NoteOnEvent& noteOn, int32 sampleOffset, double srate)
    {
        //
        auto available_note = std::find_if(
            _notes.begin(),
            _notes.end(),
            [](const BLITSineHardSync_oscillator_note& n) {return n.envelope == BLITSineHardSync_oscillator_note::Off; });

        if (available_note != _notes.end()) {
            available_note->note_id = noteOn.noteId;
            available_note->velocity = noteOn.velocity;
            available_note->envelope = BLITSineHardSync_oscillator_note::On;

            //
            double freq = 440.0 * (::pow(2.0, (static_cast<double>(noteOn.pitch) - 69) / 12.0));
            available_note->n = static_cast<int>(srate / 2.0 / freq);
            available_note->dt = static_cast<int32_t>(0xFFFFFFFFU / srate * freq);
            available_note->blit = 0.0;
            available_note->t = INT32_MIN;
            available_note->value = 0.0;
        }
    }

    //
    void BLITSineHardSync_oscillator::release(const NoteOffEvent& noteOff)
    {
        const int32 note_id = noteOff.noteId;
        auto target_note = std::find_if(
            _notes.begin(),
            _notes.end(),
            [note_id](const BLITSineHardSync_oscillator_note& n) {return n.note_id == note_id; });

        if (target_note != _notes.end()) {
            //
            target_note->envelope = BLITSineHardSync_oscillator_note::Off;
        }
    }

    bool BLITSineHardSync_oscillator::is_silent()
    {
        return std::all_of(
            _notes.begin(),
            _notes.end(),
            [](const BLITSineHardSync_oscillator_note& n) {return n.envelope == BLITSineHardSync_oscillator_note::Off; });
    }

    // constructor
    BLITSineHardSync_oscillator::BLITSineHardSync_oscillator()
        :_Slave(0.0)
        , _Leak(0.995)
        , _b1(0.0)
        , _b2(0.0)
        , _b3(0.0)
    {
        setSlave(1.2);
    }

    // set slave parameter
    void BLITSineHardSync_oscillator::setSlave(double value)
    {
        _Slave = value;
        if (value < 1.0 + 1.0e-12)
        {
            _b1 = -1.0;
            _b2 = 0.0;
            _b3 = 0.0;
        }
        else if (value < 2.0 - 1.0e-12)
        {
            _b1 = ::sin(M_PI * value) / M_PI * (-1 / (1 + value) - 1 / (1 - value) + 2);
            _b2 = ::sin(M_PI * value) / M_PI * (-1 / (2 + value) - 1 / (2 - value) + 1);
            _b3 = -::sin(M_PI * value) / 0xFFFFFFFFU;
        }
        else
        {
            _b1 = 0.0;
            _b2 = 1.0;
            _b3 = 0.0;
        }
    }

    //
    double BLITSineHardSync_oscillator::BLIT(int32_t t, int n)const
    {
        double den = remez_sin_int32(t / 2);
        double num = remez_sin_int32(t / 2 + n * t);

        if (den < -1.0e-12 || 1.0e-12 < den)
        {
            return 2.0 * num / den;
        }
        else
        {
            // apply L'Hopital's rule
            return 2.0 * (2.0 * n + 1.0);
        }
    }

    //
    double BLITSineHardSync_oscillator::render()
    {
        double value = 0.0;
        for (auto& note : _notes)
        {
            if (note.envelope == BLITSineHardSync_oscillator_note::On) {
                // add
                value += note.value * note.velocity;
            }
        }
        return value;
    }

    //
    void BLITSineHardSync_oscillator::next()
    {
        for (auto& note : _notes)
        {
            // update t
            note.t += note.dt;

            if (note.n >= 3)
            {
                // update BLIT section(n=3 -> Nyquist limit)
                note.blit = note.blit * _Leak + (BLIT(note.t, note.n) - 2.0) * note.dt;

                // synthesize value
                note.value = _b1 * remez_sin_int32(note.t)
                           + _b2 * remez_sin_int32(2 * note.t)
                           + _b3 * note.blit;
            }
            else if (note.n == 2)
            {
                // reset BLIT section
                note.blit = 0.0;

                // synthesize value
                note.value = _b1 * remez_sin_int32(note.t)
                           + _b2 * remez_sin_int32(2 * note.t);
            }
            else
            {
                // reset BLIT section
                note.blit = 0.0;

                // synthesize value
                note.value = _b1 * remez_sin_int32(note.t);
            }
        }
    }

} // namespace