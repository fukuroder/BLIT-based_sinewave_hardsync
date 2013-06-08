#define _USE_MATH_DEFINES
#include <math.h>
#include "BLIT_based_sinewave_hardsync_oscillator.h"
#include "BLIT_based_sinewave_hardsync_oscillator_note.h"

#ifdef _DEBUG
#include <windows.h>
#endif

namespace Steinberg { namespace Vst {

// constructor
BLIT_based_sinewave_hardsync_oscillator::BLIT_based_sinewave_hardsync_oscillator()
{
	// sine wave table
	for(size_t ii = 0; ii< _sinTable.size()-1; ii++)
	{
		_sinTable[ii] = sin( 2.0*M_PI * ii/(_sinTable.size()-1));
	}
	_sinTable.back() = 0.0;

	setLeak(0.995);
	setSlave(1.2);
}

//
void BLIT_based_sinewave_hardsync_oscillator::setLeak(double value)
{
	_Leak = value;
}

// 
void BLIT_based_sinewave_hardsync_oscillator::setSlave(double value)
{
	_Slave = value;
	_zzz = -4*::sin( M_PI*value);
}

//-------------
// 
//-------------
double BLIT_based_sinewave_hardsync_oscillator::LinearInterpolatedSin( double x )
{
#ifdef _DEBUG
	if( x < 0.0 || 1.0 < x)
	{
		::OutputDebugString(L"x:range error");
		return 0.0;
	}
#endif

	// スケーリング
	double pos = (_sinTable.size()-1) * x;

	// 位置を計算
	unsigned int idx_A = static_cast<int>(pos);

	// 距離を算出
	double s = pos - idx_A;

	// 線形補間
	return (1.0-s) * _sinTable[idx_A] + s*_sinTable[idx_A+1];
}

//-------------
// BLIT
//-------------
double BLIT_based_sinewave_hardsync_oscillator::BLIT( double t, int startN, int endN )
{
	double tt = ::fmod(t + 0.5, 1.0);

	// 分母
	double x_denominator = LinearInterpolatedSin( 0.5*tt );

	int N1 = endN+startN;
	double x_numerator1 = LinearInterpolatedSin(::fmod(0.5*tt*N1 + 0.25, 1.0));

	int N2 = endN-startN+1;
	if( t < 1.0e-12 || 1.0 - 1.0e-12 < t )// TODO: 要チューニング
	{
#ifdef _DEBUG
		::OutputDebugString(L"ロピタルの定理を適用");
#endif
		// ゼロ割防止。ロピタルの定理を適用
		return x_numerator1*N2;
	}
	else
	{
		// 分子
		double x_numerator2 = LinearInterpolatedSin(::fmod(0.5*tt*N2, 1.0));

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


	int N = std::min(3, note.n);

	double additive = 0.0;
	for( int ii = 1; ii <= N; ii++ )
	{
		int sgn = ii%2==0? -1 : 1;

		double b =sgn*::sin(M_PI*_Slave)*2/M_PI*ii/((ii+_Slave)*(ii-_Slave));
		additive += b*LinearInterpolatedSin( ::fmod( ii*note.t, 1.0 ) );
	}

	if( note.n >= 4 )
	{
		note.blit = note.blit*_Leak + BLIT(note.t, 4, note.n)*note.dt;
	}
	else
	{
		note.blit = 0.0;
	}
	
	note.sin = additive + _zzz*note.blit;
}

}}