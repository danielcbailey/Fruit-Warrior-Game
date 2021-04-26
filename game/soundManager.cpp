

#include "soundManager.hpp"
#include "audio/audioManager.hpp"

SoundManager _soundManager;

SoundManager::SoundManager() {
    this->music = nullptr;
    this->effects = nullptr;
    this->musicIndex = 0;
}

SoundManager::~SoundManager() {
    if (this->music) {
        delete(this->music);
    }
    if (this->effects) {
        delete(this->effects);
    }
}

void SoundManager::clearMusic() {
    this->musicChoices.clear();
    this->musicIndex = 0;
    if (this->music) {
        this->music->stop();
        delete(this->music);
        this->music = nullptr;
    }
}

void SoundManager::addMusic(std::string path) {
    this->musicChoices.push_back(path);

    if (this->music == nullptr) {
        this->music = new AudioPlayer(path.c_str());
        this->music->play(125);
    }
}

void SoundManager::playSoundEffect(std::string path) {
    if (this->effects) {
        this->effects->stop();
        _audioManager->removeAudioPlayer(this->effects);

        delete(this->effects);
        this->effects = nullptr;
    }

    this->effects = new AudioPlayer(path.c_str());
    this->effects->play(75);
}

void SoundManager::checkMusicChange() {
    if (this->music) {
        if (!this->music->isPlaying()) {
            //swap time
            this->music->stop();
            _audioManager->removeAudioPlayer(this->music);

            delete(this->music);
            this->music = nullptr;

            //getting next song
            this->musicIndex = (this->musicIndex + 1) % this->musicChoices.size();
            int i = 0;
            for (auto it = this->musicChoices.begin(); it != this->musicChoices.end(); ++it) {
                if (i == this->musicIndex) {
                    this->music = new AudioPlayer((*it).c_str());
                    this->music->play(125);
                    return;
                }
                i++;
            }
        }
    }
}