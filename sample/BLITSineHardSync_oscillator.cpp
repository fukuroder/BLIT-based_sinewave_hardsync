#define _USE_MATH_DEFINES
#include <math.h>
#include "BLITSineHardSync_oscillator.h"

namespace Steinberg{ namespace Vst{

	// constructor
	BLITSineHardSync_note::BLITSineHardSync_note()
		:_adsr(Off)
		,t(0.0)
		,sin(0.0)
		,blit(0.0)
		,n(0)
		,dt(0.0)
		,srate(44100)
	{
	}

	//
	void BLITSineHardSync_note::setSampleRate(int srate)
	{
		this->srate = srate;
	}

	//
	void BLITSineHardSync_note::release()
	{
		_adsr = Off;
	}

	//
	void BLITSineHardSync_note::trigger(const NoteOnEvent& noteOn)
	{
		_noteOn = noteOn; // copy
		_adsr = On;

		//
		double freq = 440.0*( ::pow(2.0, (_noteOn.pitch - _note_no_center)/12.0 ));
		n = static_cast<int>(srate / 2.0 / freq);
		t = 0.5;
		blit = 0.0;
		sin = 0.0;
		dt = freq / srate;
	}

	//
	int32 BLITSineHardSync_note::id()const
	{
		return _noteOn.noteId;
	}

	//
	double BLITSineHardSync_note::velocity()const
	{
		return _noteOn.velocity;
	}

	//--------------------------------------------------------------------------

	// constructor
	BLITSineHardSync_oscillator::BLITSineHardSync_oscillator()
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
	void BLITSineHardSync_oscillator::setLeak(double value)
	{
		_leak = value;
	}

	// set slave parameter
	void BLITSineHardSync_oscillator::setSlave(double value)
	{
		if( value < 1.0 + 1.0e-12 )
		{
			_b1 = -1.0;
			_b2 = 0.0;
			_b3 = 0.0;
		}
		else if( value < 2 - 1.0e-12 )
		{
			// _b[n] = -2*n*::sin(M_PI*value)/(M_PI*(n+value)*(n-value));

			_b1 =  -2*1*::sin(M_PI*value)/(M_PI*(1+value)*(1-value));
			_b2 =  -2*2*::sin(M_PI*value)/(M_PI*(2+value)*(2-value));
			
			_b3 = -4*::sin(M_PI*value);
		}		
		else
		{
			_b1 = 0.0;
			_b2 = 1.0;
			_b3 = 0.0;
		}
	}

	// calculate linear-interpolated sine wave
	double BLITSineHardSync_oscillator::LinearInterpolatedSin(double x)
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
	double BLITSineHardSync_oscillator::BLIT( double t, int endN )
	{
		const int startN = 3;

		// denominator
		double x_denominator = LinearInterpolatedSin( 0.5*t );

		int N1 = endN+startN;
		double x_numerator1 = LinearInterpolatedSin(::fmod(0.5*t*N1 + 0.25, 1.0));

		int N2 = endN-startN+1;
		if( t < 1.0e-8 || 1.0 - 1.0e-8 < t )
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
	void BLITSineHardSync_oscillator::updateOscillater(BLITSineHardSync_note& note)
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
					 + _b3*note.blit;
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
