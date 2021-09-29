#include "tables.h"

const OpenMP3::UInt OpenMP3::kBitRates[15] = {
	//{ 0, 32000, 64000, 96000,128000,160000,192000,224000, 256000,288000,320000,352000,384000,416000,448000 }, //Layer 1
	//{ 0, 32000, 48000, 56000, 64000, 80000, 96000,112000, 128000,160000,192000,224000,256000,320000,384000 }, //Layer 2
	 0, 32000, 40000, 48000, 56000, 64000, 80000, 96000, 112000,128000,160000,192000,224000,256000,320000   //Layer 3
};

const OpenMP3::UInt OpenMP3::kSampleRates[3] = { 44100, 48000, 32000 };

const OpenMP3::ScaleFactorBandIndices OpenMP3::kScaleFactorBandIndices[3] = {
	{
		{ 0,4,8,12,16,20,24,30,36,44,52,62,74,90,110,134,162,196,238,288,342,418,576 },
		{ 0,4,8,12,16,22,30,40,52,66,84,106,136,192 }
	},
	{
		{ 0,4,8,12,16,20,24,30,36,42,50,60,72,88,106,128,156,190,230,276,330,384,576 },
		{ 0,4,8,12,16,22,28,38,50,64,80,100,126,192 }
	},
	{
		{ 0,4,8,12,16,20,24,30,36,44,54,66,82,102,126,156,194,240,296,364,448,550,576 },
		{ 0,4,8,12,16,22,30,42,58,78,104,138,180,192 }
	}
};

const OpenMP3::UInt OpenMP3::kScaleFactorSizes[16][2] = {
	{ 0,0 },{ 0,1 },{ 0,2 },{ 0,3 },{ 3,0 },{ 1,1 },{ 1,2 },{ 1,3 },
	{ 2,1 },{ 2,2 },{ 2,3 },{ 3,1 },{ 3,2 },{ 3,3 },{ 4,2 },{ 4,3 }
};

const OpenMP3::UInt8 OpenMP3::kInfo[4] = { 'I', 'n', 'f', 'o' };
