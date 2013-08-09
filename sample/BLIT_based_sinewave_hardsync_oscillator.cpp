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

		// b[n] coeff
		for( size_t n = 0; n < _b.size(); n++ )
		{
			_b[n] = 0.0;
		}

		// initialize parameter
		setLeak(0.995);
		setSlave(1.2);
	}

	// set leak parameter
	void BLIT_based_sinewave_hardsync_oscillator::setLeak(double value)
	{
		_Leak = value;
	}

	// set slave parameter
	void BLIT_based_sinewave_hardsync_oscillator::setSlave(double value)
	{
		_zzz = -4*::sin(M_PI*value);
				
		
		for( size_t n = 1; n <= _b.size(); n++ )
		{
			_b[n-1] = -::sin(M_PI*value)*2/M_PI*n/((n+value)*(n-value));
		}
	}

	//-------------
	// 
	//-------------
	double BLIT_based_sinewave_hardsync_oscillator::LinearInterpolatedSin( double x )
	{
		// scaling
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
	double BLIT_based_sinewave_hardsync_oscillator::BLIT( double t, int startN, int endN )
	{
		// denominator
		double x_denominator = LinearInterpolatedSin( 0.5*t );

		int N1 = endN+startN;
		double x_numerator1 = LinearInterpolatedSin(::fmod(0.5*t*N1 + 0.25, 1.0));

		int N2 = endN-startN+1;
		if( t < 1.0e-12 || 1.0 - 1.0e-12 < t )
		{
			// 
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
		note.t += note.dt;
		if ( 1.0 <= note.t )note.t -= 1.0;

		//------------------
		// additive section
		//------------------
		double additive = 0.0;
		int N = (_b.size() < note.n)? _b.size() : note.n;
		for( int n = 1; n <= N; n++ )
		{
			additive += _b[n-1]*LinearInterpolatedSin( ::fmod( n*note.t, 1.0 ) );
		}

		//--------------
		// BLIT section
		//--------------
		if( note.n > _b.size() )
		{
			note.blit = note.blit*_Leak + BLIT(note.t,  _b.size()+1, note.n)*note.dt;
		}
		else
		{
			note.blit = 0.0;
		}

		note.sin = additive + _zzz*note.blit;
	}

}} // namespace
