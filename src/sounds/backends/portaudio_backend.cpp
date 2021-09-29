#include "sounds/backends/portaudio_backend.hpp"
#include <iostream>

PortaudioBackend::PortaudioBackend() {
    auto error = Pa_Initialize();
    if (error != paNoError) {
        std::cout << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
    }
}

PortaudioBackend::~PortaudioBackend() {
    close();
    auto error = Pa_Terminate();
    if(error != paNoError) {
        std::cout << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
    }
}

bool PortaudioBackend::open(ChannelsCount channelsCount, SamplingRate samplingRate) {
    close();

    PaStreamParameters outputParameters = {};
    outputParameters.device = Pa_GetDefaultOutputDevice(); 
    if (outputParameters.device == paNoDevice) {
        std::cout << "PortaudioBackend: could not find a suitable device" << std::endl;
        return false;
    }
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;
	outputParameters.channelCount = channelsCount;
    outputParameters.sampleFormat = paFloat32;
	
	PaError ret = Pa_OpenStream(
		&stream,
		nullptr, // no input
		&outputParameters,
		(double)samplingRate,
		paFramesPerBufferUnspecified, // framesPerBuffer
		0, // flags
		[](
            const void *input, void *output,
            unsigned long frameCount,
            [[maybe_unused]] const PaStreamCallbackTimeInfo* timeInfo,
            [[maybe_unused]] PaStreamCallbackFlags statusFlags,
            void *userData 
        ) {
            PortaudioBackend* backend = reinterpret_cast<PortaudioBackend*>(userData);
            return (int)(backend->callback(backend->player, input, output, frameCount) ? paContinue : paComplete);
        }, 
        (void*)this
    );
	
    if(ret != paNoError) {
		std::cout << "Pa_OpenStream failed: " << Pa_GetErrorText(ret) << std::endl;
		close();
		return false;
	}
	
    ret = Pa_StartStream(stream);
	if(ret != paNoError) {
        std::cout << "Pa_StartStream failed: " << Pa_GetErrorText(ret) << std::endl;
        return false;
    }
    
    return true;
}

void PortaudioBackend::close() {
    if(stream) {
        Pa_CloseStream(stream);
    }
    stream = nullptr;
}

