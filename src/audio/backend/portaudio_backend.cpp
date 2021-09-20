#include "audio/backend/portaudio_backend.hpp"
#include <iostream>
#include "audio/audio_file.hpp"

PortaudioBackend::PortaudioBackend() {
    auto error = Pa_Initialize();
    if (error != paNoError) {
        std::cout << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
    }
}

PortaudioBackend::~PortaudioBackend() {
    auto error = Pa_Terminate();
    if(error != paNoError) {
        std::cout << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
    }
    delete currentFile;
}

bool PortaudioBackend::open(AudioFile* file) {
    if (
        currentFile != nullptr &&
        currentFile->channelsCount() == file->channelsCount() &&
        currentFile->sampleRate() == file->sampleRate() &&
        currentFile->isSampleTypeFloat() == file->isSampleTypeFloat() &&
        currentFile->sampleSize() == file->sampleSize()
    ) {
        currentFile = file;
        return true;   
    }

    close();

    PaStreamParameters outputParameters = {};
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;
    
    if (outputParameters.device == paNoDevice) {
        std::cout << "PortaudioBackend: could not find a suitable device" << std::endl;
        return false;
    }

	outputParameters.channelCount = file->channelsCount();
    if (file->isSampleTypeFloat()) {
        outputParameters.sampleFormat = paFloat32;
    } else if (file->sampleSize() == 1) {
        outputParameters.sampleFormat = paInt8;
    } else if (file->sampleSize() == 2) {
        outputParameters.sampleFormat = paInt16;
    } else if (file->sampleSize() == 3) {
        outputParameters.sampleFormat = paInt24;
    } else if (file->sampleSize() == 4) {
        outputParameters.sampleFormat = paInt32;
    } else {
        std::cout << "PortaudioBackend: open failure due to incompatible format" << std::endl;
        return false;
    }

    currentFile = file;
	
	PaError ret = Pa_OpenStream(
		&stream,
		nullptr, // no input
		&outputParameters,
		(double)file->sampleRate(),
		paFramesPerBufferUnspecified, // framesPerBuffer
		0, // flags
		[](
            const void *input, void *output,
            unsigned long frameCount,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData 
        ) {
            statusFlags; timeInfo;
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

