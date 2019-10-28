#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;

class Audio {
public:
    Music musicBackground;
    Sound coinSound;
    Sound stageClearSound;
    Sound jumpSound;
    Sound walljumpSound;
    Sound stompSound;
    Sound killSound;
    Sound dashSound;
    SoundBuffer coinSoundB;
    SoundBuffer stageClearSoundB;
    SoundBuffer bumpSoundB;
    SoundBuffer jumpSoundB;
    SoundBuffer walljumpSoundB;
    SoundBuffer stompSoundB;
    SoundBuffer killSoundB;
    SoundBuffer dashSoundB;

    Audio();

    void playJumpSound();

    void playWalljumpSound();

    void playBackgroundMusic();

    void playCoinSound();

    void playStageClearSound();

    void playStompSound();

    void playKillSound();

    void playDashSound();
};

#endif