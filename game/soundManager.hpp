

#ifndef SOUND_MANAGER_INCLUDED
#define SOUND_MANAGER_INCLUDED

#include "audio/audioPlayer.hpp"
#include <string>
#include <list>

class SoundManager {
    private:
    AudioPlayer* music;
    AudioPlayer* effects;

    int musicIndex;
    std::list<std::string> musicChoices;

    public:

    SoundManager();

    ~SoundManager();

    void clearMusic();

    void addMusic(std::string path);

    void playSoundEffect(std::string path);

    void checkMusicChange();
};

extern SoundManager _soundManager;

#endif //SOUND_MANAGER_INCLUDED