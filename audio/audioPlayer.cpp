

#include "audioPlayer.hpp"
#include "audioManager.hpp"
#include "globals.hpp"
#include <cstdio>

AudioPlayer::AudioPlayer(const char* filePath) {
    this->audioFile = new File(_globalFileSystem, filePath);
    
    if (!this->audioFile->readable()) {
        printfdbg("Could not open the audio file: %s\n", filePath);
        return;
    }

    //Checking that the format is correct
    //Will check sample rate, bit depth, audio format, and channels
    //Expects: 22050, 16, PCM (1), 1
    char wavHeader[44];
    this->audioFile->read(wavHeader, 44);
    //Checking that the file is indeed a wav file
    if (wavHeader[0] != 'R' || wavHeader[1] != 'I' || wavHeader[2] != 'F' || wavHeader[3] != 'F' ||
        wavHeader[8] != 'W' || wavHeader[9] != 'A' || wavHeader[10] != 'V' || wavHeader[11] != 'E') {
        
        //Invalid file, not a WAV file
        printfdbg("The provided audio file is not a WAV file.\n");
        return;
    }

    if (wavHeader[20] != 1) {
        //Number of channels is not 1
        printfdbg("The provided audio file is of type %d, should be 1 (PCM).\n", wavHeader[20]);
        return;
    }

    if (wavHeader[22] != 1) {
        //Number of channels is not 1
        printfdbg("The provided audio file has %d channels, should be 1.\n", wavHeader[22]);
        return;
    }

    int sampleRate = *((int*)(&wavHeader[24]));
    if (sampleRate != 22050) {
        //Sample rate is not 22050
        printfdbg("The provided audio file has a sample rate of %d, should be 22050.\n", sampleRate);
        return;
    }

    if (wavHeader[34] != 16) {
        //Number of channels is not 1
        printfdbg("The provided audio file has %d bits, should be 16.\n", wavHeader[34]);
        return;
    }

    //File is valid, now checking the number of samples contained in the data
    this->samples = (*((int*)(&wavHeader[40]))) >> 1;
}

AudioPlayer::~AudioPlayer() {
    this->audioFile->close();

    if (this->playing) {
        _audioManager->removeAudioPlayer(this);
    }

    delete(this->audioFile);
}

void AudioPlayer::play(int volume) {
    this->sampleCounter = 0;
    this->volume = volume;
    this->playing = true;
    _audioManager->registerAudioPlayer(this);
}

void AudioPlayer::stop() {
    if (playing) {
        _audioManager->removeAudioPlayer(this);
        this->playing = false;
    }
}

//Called by the audio manager to let the player refresh its buffer if it needs to
//It exists because SD functions can not take place from an interrupt context.
void AudioPlayer::bufferUpdate(int16_t* buffer) {
    if (!this->playing) {
        return;
    }

    int samplesToPlay = this->samples - SAMPLES_PER_BUFFER > this->sampleCounter ? SAMPLES_PER_BUFFER : this->samples - this->sampleCounter;
    this->sampleCounter += samplesToPlay;

    if (samplesToPlay <= 0) {
        this->playing = false;
        return;
    }

    this->audioFile->read(buffer, AUDIO_BUFFER_SIZE);

    for (int i = 0; SAMPLES_PER_BUFFER > i; i++) {
        //Making all values unsigned
        if (i <= samplesToPlay) {
            //buffer[i] = buffer[i];
        } else {
            buffer[i] = 0;
        }
    }
}