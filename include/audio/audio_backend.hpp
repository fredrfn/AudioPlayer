#ifndef AUDIO_BACKEND_HPP
#define AUDIO_BACKEND_HPP

class AudioPlayer;
class AudioFile;

typedef bool(*AudioBackendCallback)(
    AudioPlayer* player, 
    const void* output, 
    void* input,
    unsigned long frameCount
);

class AudioBackend {
protected:
    AudioBackendCallback callback;
    AudioPlayer* player;
    AudioFile* currentFile;
public:
    void setup(AudioPlayer* p, AudioBackendCallback handler) { player = p; callback = handler; }
    virtual bool open(AudioFile* file) = 0;
    virtual void close() = 0;
};

#endif