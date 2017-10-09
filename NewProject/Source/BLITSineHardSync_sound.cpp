/*
 * BLITSineHardSync_sound.cpp
 *
 * Copyright (c) 2017, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#include "BLITSineHardSync_sound.h"
#include "BLITSineHardSync_voice.h"
#include "../../remez_approx/remez_approx.h"

//
BLITSineHardSync_sound::BLITSineHardSync_sound():_leak(0.995),_slave(1.0)
{
}

//
bool BLITSineHardSync_sound::appliesToNote (int /*midiNoteNumber*/)
{
    return true;
}

//
bool BLITSineHardSync_sound::appliesToChannel (int /*midiChannel*/)
{
    return true;
}

// set slave parameter
void BLITSineHardSync_sound::setSlave(double salve)
{
    if( _slave != salve )
    {
        _slave = salve;
        if (salve < 1.0 + 1.0e-12)
        {
            _b1 = -1.0;
            _b2 = 0.0;
            _b3 = 0.0;
        }
        else if (salve < 2.0 - 1.0e-12)
        {   
            _b1 = ::sin(M_PI*salve) / M_PI * (-1 / (1 + salve) - 1 / (1 - salve) + 2);
            _b2 = ::sin(M_PI*salve) / M_PI * (-1 / (2 + salve) - 1 / (2 - salve) + 1);
            _b3 = -::sin(M_PI*salve) / 0xFFFFFFFFU;
        }
        else
        {
            _b1 = 0.0;
            _b2 = 1.0;
            _b3 = 0.0;
        }
    }
}

// BLIT ?
double BLITSineHardSync_sound::blit(int32_t t, int N)const
{
    double den = remez_sin_int32(t/2);
	double num = remez_sin_int32(t/2+N*t);
    
    if (den < -1.0e-12 || 1.0e-12 < den)
    {
        return 2.0*num / den;
    }
    else
    {
        // apply L'Hopital's rule
        return 2.0*(2 * N + 1);
    }
}

//
void BLITSineHardSync_sound::next(BLITSineHardSync_voice *voice)
{
    // update t
    voice->t += voice->dt;
    
    if (voice->n >= 3)
    {
        // update BLIT section(n=3 -> Nyquist limit)
        voice->blit = voice->blit*_leak + (blit(voice->t, voice->n)-2.0)*voice->dt;
        
        // synthesize value
        voice->value =
        ( _b1*remez_sin_int32(voice->t)
        + _b2*remez_sin_int32(2*voice->t)
        + _b3*voice->blit)*voice->_velocity;
    }
    else if (voice->n == 2)
    {
        // reset BLIT section
        voice->blit = 0.0;
        
        // synthesize value
        voice->value =
        ( _b1*remez_sin_int32(voice->t)
        + _b2*remez_sin_int32(2*voice->t))*voice->_velocity;
    }
    else
    {
        // reset BLIT section
        voice->blit = 0.0;
        
        // synthesize value
        voice->value = _b1*remez_sin_int32(voice->t)*voice->_velocity;
    }
}
