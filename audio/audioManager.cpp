

#include "audioManager.hpp"
#include "globals.hpp"

AudioManager* _audioManager;

dac_t dac;

AudioManager::AudioManager() : bufferAlloc(0) {
    //analogout_init(&dac, p18);
    std::chrono::microseconds time = std::chrono::microseconds((long long int)(1000000 * SAMPLES_PER_BUFFER / WAVEFORM_SAMPLE_RATE));
    this->ticker.attach(callback(this, &AudioManager::onAudioTick), time);
    _audioManager = this;

    this->usingBuffer1 = true;
    this->buffer1 = (uint16_t*) this->getAudioBuffer();
    this->buffer2 = (uint16_t*) this->getAudioBuffer();
    this->loadBuffer = (int16_t*) this->getAudioBuffer();

    for (int i = 0; SAMPLES_PER_BUFFER > i; i++) {
        this->buffer1[i] = 0x8000;
    }
}

void AudioManager::onAudioTick() {
    //int sample = 0;

    //Clearing the buffer and playing audio in the other buffer
    if (this->usingBuffer1) {
        this->aOut.write_u16((unsigned short int*)this->buffer1, SAMPLES_PER_BUFFER << 1, 22050);
        for (int i = 0; SAMPLES_PER_BUFFER > i; i++) {
            this->buffer2[i] = 0x8000;
        }
    } else {
        this->aOut.write_u16((unsigned short int*)this->buffer2, SAMPLES_PER_BUFFER << 1, 22050);
        for (int i = 0; SAMPLES_PER_BUFFER > i; i++) {
            this->buffer1[i] = 0x8000;
        }
    }

    //Checking if any audio players can be removed
    auto itr = this->players.begin(); 
    while (itr != this->players.end()) {

        if (!(*itr)->isPlaying()) {
            //Can remove
            auto pIt = itr;
            ++itr;

            this->players.remove(*pIt);

            break;
        }

        ++itr;
    }

    for (std::list<AudioPlayer*>::iterator it = this->players.begin(); it != this->players.end(); ++it) {
        //sample += (*it)->getNextSample();
        (*it)->bufferUpdate(this->loadBuffer);

        //Blending it with the other channels
        if (this->usingBuffer1) {
            //Load buffer 2
            for (int i = 0; SAMPLES_PER_BUFFER > i; i++) {
                this->buffer2[i] = ((int) this->loadBuffer[i] * ((*it)->getVolume()) / 200 + (int)this->buffer2[i] - 0x8000) + 0x8000;
            }
        } else {
            //Load buffer 1
            for (int i = 0; SAMPLES_PER_BUFFER > i; i++) {
                this->buffer1[i] = ((int) this->loadBuffer[i] * ((*it)->getVolume()) / 200 + (int)this->buffer1[i] - 0x8000) + 0x8000;
            }
        }
    }

    this->usingBuffer1 = !this->usingBuffer1;
}

void* AudioManager::getAudioBuffer() {
    if ((this->bufferAlloc & 0x1) == 0) {
        this->bufferAlloc |= 0x1;
        return (void*)AUDIO_BUFFERSPACE_START;
    } else if ((this->bufferAlloc & 0x2) == 0) {
        this->bufferAlloc |= 0x2;
        return (void*)(AUDIO_BUFFERSPACE_START + AUDIO_BUFFER_SIZE);
    } else if ((this->bufferAlloc & 0x4) == 0) {
        this->bufferAlloc |= 0x4;
        return (void*)(AUDIO_BUFFERSPACE_START + AUDIO_BUFFER_SIZE * 2);
    } else if ((this->bufferAlloc & 0x8) == 0) {
        this->bufferAlloc |= 0x8;
        return (void*)(AUDIO_BUFFERSPACE_START + AUDIO_BUFFER_SIZE * 3);
    } else {
        //Ran out of space
        printfdbg("No enough room to allocate another audio buffer.\n");
        return nullptr;
    }
}

void AudioManager::releaseAudioBuffer(void* ptr) {
    int index = (((int)ptr) - AUDIO_BUFFERSPACE_START) / AUDIO_BUFFER_SIZE;
    if (index < 0 || index > 3) {
        printfdbg("Invalid pointer passed to release audio buffer.\n");
        return;
    }

    this->bufferAlloc &= (0x1 << index) ^ 0xF;
}

void AudioManager::registerAudioPlayer(AudioPlayer* player) {
    if (this->players.size() >= 2) {
        printfdbg("Already contains more than the maximum number of audio players.\n");
        return;
    }

    __disable_irq(); //Cannot afford to have the list interrupted mid-adding of a new player because that could cause a fault
    this->players.push_back(player);
    __enable_irq();
}

void AudioManager::removeAudioPlayer(AudioPlayer* player) {
    __disable_irq();
    this->players.remove(player);
    __enable_irq();
}