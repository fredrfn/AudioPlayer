#pragma once

#include "types.h"

namespace OpenMP3 {
	void Requantize(UInt sfreq, const FrameData & data, UInt gr, UInt ch, Float32 is[576]);
	void Reorder(UInt sfreq, const FrameData & data, UInt gr, UInt ch, Float32 is[576]);
}

