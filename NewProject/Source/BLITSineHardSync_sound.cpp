/*
 * BLITSineHardSync_sound.cpp
 *
 * Copyright (c) 2016, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#include "BLITSineHardSync_sound.h"
#include "BLITSineHardSync_voice.h"
#include "../../remez_approx/remez_approx.h"

#define _USE_MATH_DEFINES
#include <math.h>

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
void BLITSineHardSync_sound::setSlave(double value)
{
    if( _slave != value )
    {
        _slave = value;
        if (value < 1.0 + 1.0e-12)
        {
            _b1 = -1.0;
            _b2 = 0.0;
            _b3 = 0.0;
        }
        else if (value < 2.0 - 1.0e-12)
        {
            // _b[n] = -2*n*::sin(M_PI*value)/(M_PI*(n+value)*(n-value));
            
            _b1 = -2 * ::sin(M_PI*value) / (M_PI*(1 + value)*(1 - value));
            _b2 = -4 * ::sin(M_PI*value) / (M_PI*(2 + value)*(2 - value));
            _b3 = -2 * ::sin(M_PI*value) / M_PI / 0xFFFFFFFF;
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
    int N1 = N + 3;
    double cos1 = remez_cos_uint32(N1*(t/2));
    
    int N2 = N - 3 + 1;
    double sin1 = remez_sin_uint32(N2*(t/2));
    
    double sin2 = remez_sin_uint32(t/2);
    
    if ( sin2 < -1.0e-8 || 1.0e-8 < sin2 )
    {
        return cos1*sin1 / sin2;
    }
    else
    {
        // apply L'Hopital's rule
        return N2;
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
        voice->blit = voice->blit*_leak + blit(voice->t, voice->n)*voice->dt;
        
        // synthesize value
        voice->value =
          _b1*remez_sin_uint32(voice->t)
        + _b2*remez_sin_uint32(2*voice->t)
        + _b3*voice->blit;
    }
    else if (voice->n == 2)
    {
        // reset BLIT section
        voice->blit = 0.0;
        
        // synthesize value
        voice->value =
          _b1*remez_sin_uint32(voice->t)
        + _b2*remez_sin_uint32(2*voice->t);
    }
    else
    {
        // reset BLIT section
        voice->blit = 0.0;
        
        // synthesize value
        voice->value = _b1*remez_sin_uint32(voice->t);
    }
}
