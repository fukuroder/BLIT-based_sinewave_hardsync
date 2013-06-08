#include "BLIT_based_sinewave_hardsync_oscillator_note.h"
#include <math.h>

namespace Steinberg {namespace Vst {

// constructor
BLIT_based_sinewave_hardsync_oscillator_note::BLIT_based_sinewave_hardsync_oscillator_note()
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
void BLIT_based_sinewave_hardsync_oscillator_note::setSampleRate(int srate)
{
	this->srate = srate;
}

//
void BLIT_based_sinewave_hardsync_oscillator_note::release()
{
	_adsr = Off;
}

//---------
//
//---------
void BLIT_based_sinewave_hardsync_oscillator_note::trigger(const NoteOnEvent& noteOn)
{
	_noteOn = noteOn; // コピー
	_adsr = On;

	//
	double freq = 440.0*( ::pow(2.0, (_noteOn.pitch - _note_no_center)/12.0 ));
	n = static_cast<int>(srate / 2.0 / freq);
	dt = freq / srate;
}

/// タグ取得.
int32 BLIT_based_sinewave_hardsync_oscillator_note::id()const
{
	return _noteOn.noteId;
}

//---------
//
//---------
double BLIT_based_sinewave_hardsync_oscillator_note::velocity()const
{
	return _noteOn.velocity;
}

}}
