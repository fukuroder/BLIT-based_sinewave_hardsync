/*
 * BLITSineHardSync_sound.cpp
 *
 * Copyright (c) 2016, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#include "BLITSineHardSync_sound.h"
#include "BLITSineHardSync_voice.h"

//
BLITSineHardSync_sound::BLITSineHardSync_sound()
{
    // sine wave table
    for (size_t ii = 0; ii < _sinTable.size(); ii++)
    {
        _sinTable[ii] = sin(2.0*M_PI * ii / (_sinTable.size() - 1));
    }
    
    _leak = 0.995;
    _slave = 1.0;
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
    if( _slave != value)
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
            
            _b1 = -2 * 1 * ::sin(M_PI*value) / (M_PI*(1 + value)*(1 - value));
            _b2 = -2 * 2 * ::sin(M_PI*value) / (M_PI*(2 + value)*(2 - value));
            _b3 = -4 * ::sin(M_PI*value);
        }
        else
        {
            _b1 = 0.0;
            _b2 = 1.0;
            _b3 = 0.0;
        }
    }
}

// calculate linear-interpolated sine wave
double BLITSineHardSync_sound::LinearInterpolatedSin(double x)const
{
    // scale table size
    double pos = (_sinTable.size() - 1) * x;
    
    // cast to int
    unsigned int idx_A = static_cast<int>(pos);
    
    // linear interpolate
    double s = pos - idx_A;
    return (1.0 - s)*_sinTable[idx_A] + s*_sinTable[idx_A + 1];
}

// BLIT ?
double BLITSineHardSync_sound::BLIT(double t, int N)const
{
    int N1 = N + 3;
    double cos1 = LinearInterpolatedSin(::fmod(0.5*t*N1 + 0.25, 1.0));
    
    int N2 = N - 3 + 1;
    double sin1 = LinearInterpolatedSin(::fmod(0.5*t*N2, 1.0));
    
    double sin2 = LinearInterpolatedSin(0.5*t);
    
    if ((1.0e-8) < t && t < 1.0 - (1.0e-8))
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
    if (1.0 <= voice->t)voice->t -= 1.0;
    if (voice->n >= 3)
    {
        // update BLIT section(n=3 -> Nyquist limit)
        voice->blit = voice->blit*_leak + BLIT(voice->t, voice->n)*voice->dt;
        
        // synthesize value
        voice->value = _b1*LinearInterpolatedSin(voice->t)
        + _b2*LinearInterpolatedSin(::fmod(2 * voice->t, 1.0))
        + _b3*voice->blit;
    }
    else if (voice->n == 2)
    {
        // reset BLIT section
        voice->blit = 0.0;
        
        // synthesize value
        voice->value = _b1*LinearInterpolatedSin(voice->t)
        + _b2*LinearInterpolatedSin(::fmod(2 * voice->t, 1.0));
    }
    else
    {
        // reset BLIT section
        voice->blit = 0.0;
        
        // synthesize value
       
        voice->value = _b1*LinearInterpolatedSin(voice->t);
    }
}
