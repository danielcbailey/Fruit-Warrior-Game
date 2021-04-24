

#ifndef AUDIO_PLAYER_INCLUDED
#define AUDIO_PLAYER_INCLUDED

#include "files.hpp"
#include <stdint.h>

//Testing
#include "analogOutAsync.hpp"

class AudioPlayer {
    private:

    File* audioFile;
    bool playing;
    int samples;
    int sampleCounter;
    int volume;

    public:

    //Only accepts files at 22.05kHz
    AudioPlayer(const char* filePath);

    ~AudioPlayer();

    void play(int volume);

    void stop();

    inline bool isPlaying() { return this->playing; }

    inline int getVolume() { return this->volume; }

    //Called by the audio manager to let the player refresh its buffer if it needs to
    //It exists because SD functions can not take place from an interrupt context.
    void bufferUpdate(int16_t* buffer);
};

#endif //AUDIO_PLAYER_INCLUDED