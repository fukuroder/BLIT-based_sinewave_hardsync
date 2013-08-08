#pragma once
#include "BLIT_based_sinewave_hardsync_oscillator_note.h"
#include "pluginterfaces/vst/ivstevents.h"

namespace Steinberg{ namespace Vst{

	/// Note class
	class BLIT_based_sinewave_hardsync_oscillator_note
	{
	public:
		// Constructor
		BLIT_based_sinewave_hardsync_oscillator_note();

		// Note on
		void trigger(const NoteOnEvent& noteOn);

		// タグ取得.
		int32 id()const;

		// ベロシティ取得.
		double velocity()const;

		// ADSR.
		enum ADSR{
			Off,
			On,
		};

		//
		void setSampleRate(int srate);

		// ノートリリース.
		void release();

		// 現在の時間.
		double	t;

		// 現在の値.
		double	sin;

		// 倍音数.
		unsigned int n;

		// 刻み幅.
		double	dt;

		//
		int srate;

		//
		double blit;

		ADSR adsr()const{return _adsr;};

	protected:

		// ADSR.
		ADSR	_adsr;

		NoteOnEvent _noteOn;

		// 中央のノートナンバー
		static const int _note_no_center = 69;
	};
}} // namespace
