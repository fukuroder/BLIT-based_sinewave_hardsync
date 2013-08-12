#define _USE_MATH_DEFINES
#include <math.h>
#include "BLIT_based_sinewave_hardsync_oscillator.h"
#include "BLIT_based_sinewave_hardsync_oscillator_note.h"

namespace Steinberg{ namespace Vst{

	// constructor
	BLIT_based_sinewave_hardsync_oscillator::BLIT_based_sinewave_hardsync_oscillator()
	{
		// sine wave table
		for(size_t ii = 0; ii < _sinTable.size()-1; ii++)
		{
			_sinTable[ii] = sin( 2.0*M_PI * ii/(_sinTable.size()-1));
		}
		_sinTable.back() = 0.0;

		// initialize parameter
		setLeak(0.995);
		setSlave(1.2);
	}

	// set leak parameter
	void BLIT_based_sinewave_hardsync_oscillator::setLeak(double value)
	{
		_leak = value;
	}

	// set slave parameter
	void BLIT_based_sinewave_hardsync_oscillator::setSlave(double value)
	{
		if( value < 1.0 + 1.0e-12 )
		{
			_b1 = -1.0;
			_b2 = 0.0;
			_zzz = 0.0;
		}
		else if( value < 2 - 1.0e-12 )
		{
			// _b[n] = -2*n*::sin(M_PI*value)/(M_PI*(n+value)*(n-value));

			_b1 =  -2*1*::sin(M_PI*value)/(M_PI*(1+value)*(1-value));
			_b2 =  -2*2*::sin(M_PI*value)/(M_PI*(2+value)*(2-value));
			
			_zzz = -4*::sin(M_PI*value);
		}		
		else
		{
			_b1 = 0.0;
			_b2 = 1.0;
			_zzz = 0.0;
		}
	}

	// calculate linear-interpolated sine wave
	double BLIT_based_sinewave_hardsync_oscillator::LinearInterpolatedSin(double x)
	{
		// scale table size
		double pos = (_sinTable.size()-1) * x;

		// cast to int
		unsigned int idx_A = static_cast<int>(pos);

		// linear interpolate
		double s = pos - idx_A;
		return (1.0-s)*_sinTable[idx_A] + s*_sinTable[idx_A+1];
	}

	//-------------
	// BLIT
	//-------------
	double BLIT_based_sinewave_hardsync_oscillator::BLIT( double t, int endN )
	{
		const int startN = 3;

		// denominator
		double x_denominator = LinearInterpolatedSin( 0.5*t );

		int N1 = endN+startN;
		double x_numerator1 = LinearInterpolatedSin(::fmod(0.5*t*N1 + 0.25, 1.0));

		int N2 = endN-startN+1;
		if( t < 1.0e-12 || 1.0 - 1.0e-12 < t )
		{
			// apply L'Hopital's rule
			return x_numerator1*N2;
		}
		else
		{
			// numerator
			double x_numerator2 = LinearInterpolatedSin(::fmod(0.5*t*N2, 1.0));

			return x_numerator1*x_numerator2/x_denominator;
		}
	}

	//-------------
	//
	//-------------
	void BLIT_based_sinewave_hardsync_oscillator::updateOscillater(BLIT_based_sinewave_hardsync_oscillator_note& note)
	{
		// update t
		note.t += note.dt;
		if ( 1.0 <= note.t )note.t -= 1.0;

		if( note.n >= 3)
		{
			// update BLIT section(n=3 -> Nyquist limit)
			note.blit = note.blit*_leak + BLIT(note.t, note.n)*note.dt;

			// update value
			note.sin = _b1*LinearInterpolatedSin(note.t)
			         + _b2*LinearInterpolatedSin(::fmod( 2*note.t, 1.0 ) )
					 + _zzz*note.blit;
		}
		else if( note.n == 2 )
		{
			// reset BLIT section
			note.blit = 0.0;

			// update value
			note.sin = _b1*LinearInterpolatedSin(note.t)
			         + _b2*LinearInterpolatedSin(::fmod( 2*note.t, 1.0 ) );
		}
		else
		{
			// reset BLIT section
			note.blit = 0.0;

			// update value
			note.sin = _b1*LinearInterpolatedSin(note.t);
		}
	}

}} // namespace
