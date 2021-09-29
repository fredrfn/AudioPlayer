#pragma once

namespace OpenMP3 {
	enum Mode {
		kModeStereo,
		kModeJointStereo,
		kModeDualMono,
		kModeMono
	};
	class Library;
	class Iterator;
	class Decoder;
	class Frame;
	typedef unsigned char UInt8;
	typedef unsigned short UInt16;
	typedef unsigned int UInt32;
	typedef UInt32 UInt;
	typedef float Float32;
};

class OpenMP3::Library {
public:
	Library();
private:
	friend Decoder;
};

class OpenMP3::Iterator {
public:
	Iterator(const Library & library, const UInt8 * data, UInt size);
	bool GetNext(Frame & frame);
private:
	struct Private;
	const UInt8 * m_start, * m_ptr, * m_end;
	bool m_hack_first;		//for Info frame skipping hack
};

class OpenMP3::Decoder {
public:
	Decoder(const Library & library);
	void Reset();															//Call before processing new song, resets internal buffers
	UInt ProcessFrame(const Frame & frame, Float32 output[2][1152]);		//return: number samples extracted from frame (0 or 1152)
private:
	struct Private;
	const Library & library;
	UInt8 m_br[8320];
	UInt8 m_framedata[10496];
	Float32 m_hs_store[2][32][18];
	Float32 m_sbs_v_vec[2][1024];
	const UInt8 * m_stream_ptr;
	UInt m_stream_remainder;
};

class OpenMP3::Frame {
public:
	Frame();
	UInt GetBitRate() const;
	UInt GetSampleRate() const;
	Mode GetMode() const;
private:
	friend Iterator;
	friend Decoder;
	UInt8 m_bitrate_index;
	UInt8 m_sr_index;
	Mode m_mode;
	UInt8 m_mode_extension;
	const UInt8 * m_ptr;		//pointer to data area
	UInt m_datasize;			//size of whole frame, minus headerword + check
	UInt m_length;				//for Info frame skipping
};
