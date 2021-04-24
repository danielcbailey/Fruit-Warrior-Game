

#ifndef AUDIO_MANAGER_INCLUDED
#define AUDIO_MANAGER_INCLUDED

#include "audioPlayer.hpp"
#include <list>

#define AUDIO_BUFFER_SIZE 4096
#define SAMPLES_PER_BUFFER 2048
#define MAX_AUDIO_CHANNELS 2
#define AUDIO_BUFFERSPACE_START 0x2007C000
#define WAVEFORM_SAMPLE_RATE 22050

class AudioManager {
    private:

    int bufferAlloc;
    std::list<AudioPlayer*> players;
    Ticker ticker;
    bool bufNeedsUpdate;
    bool usingBuffer1;
    uint16_t* buffer1;
    uint16_t* buffer2;
    int16_t* loadBuffer;

    AnalogOutAsync aOut;

    void onAudioTick();

    public:

    AudioManager();

    void* getAudioBuffer();

    void releaseAudioBuffer(void* ptr);

    void registerAudioPlayer(AudioPlayer* player);

    void removeAudioPlayer(AudioPlayer* player);

    inline void setBufNeedUpdate() { this->bufNeedsUpdate = true; }
};

extern AudioManager* _audioManager;

extern dac_t dac;

#endif //AUDIO_MANAGER_INCLUDED