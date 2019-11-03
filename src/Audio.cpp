#ifndef AUDIO_CPP
#define AUDIO_CPP

#include "Audio.hpp"

Audio::Audio() {
    if (!musicBackground.openFromFile("audio/backgroundmusic.ogg"))
        std::cerr << L"Music Background n'a pas pu être chargée" << std::endl;
    else
    {
        musicBackground.setLoop(true);
        musicBackground.setVolume(30);
    }

    if (!jumpSoundB.loadFromFile("audio/jump.wav"))
        std::cerr << L"Le son pour Jump na pas pu être chargé" << std::endl;
    else
    {
        jumpSound.setBuffer(jumpSoundB);
        jumpSound.setVolume(30);   
    }

    if (!walljumpSoundB.loadFromFile("audio/wall_jump.wav"))
        std::cerr << L"Le son pour walljump na pas pu être chargé" << std::endl;
    else
    {
        walljumpSound.setBuffer(walljumpSoundB);
        walljumpSound.setVolume(30);
    }

    if(!stompSoundB.loadFromFile("audio/stomp.ogg"))
        std::cerr << L"Le son de Stomp n'a pas pu être chargé" << std::endl;
    else
    {   
        stompSound.setBuffer(stompSoundB);
    }
        

    if(!killSoundB.loadFromFile("audio/kill.ogg"))
        std::cerr << L"Le son de Kill n'a pas pu être chargé" << std::endl;
    else
    {        
        killSound.setVolume(30);
        killSound.setBuffer(killSoundB);
    }
      
    if(!dashSoundB.loadFromFile("audio/dash.wav"))
        std::cerr << L"Le son de dash n'a pas pu être chargé" << std::endl;
    else
    {        
        dashSound.setVolume(30);
        dashSound.setBuffer(dashSoundB);
    }     

    if(!diamondSoundB.loadFromFile("audio/diamond.wav"))
        std::cerr << L"Le son de diamond n'a pas pu être chargé" << std::endl;
    else
    {        
        diamondSound.setVolume(30);
        diamondSound.setBuffer(diamondSoundB);
    }        

    if(!dashStoneSoundB.loadFromFile("audio/dash_stone.wav"))
        std::cerr << L"Le son de dash n'a pas pu être chargé" << std::endl;
    else
    {        
        dashStoneSound.setVolume(30);
        dashStoneSound.setBuffer(dashStoneSoundB);
    }  
}

void Audio::playJumpSound() {
    jumpSound.play();
}

void Audio::playWalljumpSound() {
    walljumpSound.play();
}

void Audio::playBackgroundMusic() {
    musicBackground.play();
}

void Audio::playCoinSound() {
    coinSound.setVolume(50);
    coinSound.setBuffer(coinSoundB);
    coinSound.play();
}

void Audio::playStageClearSound() {
    stageClearSound.setBuffer(stageClearSoundB);
    stageClearSound.play();
}

void Audio::playStompSound() {
    stompSound.play();
}

void Audio::playKillSound() {
    killSound.play();
}

void Audio::playDashSound() {
    dashSound.play();
}

void Audio::playDiamondSound() {
    diamondSound.play();
}

void Audio::playDashStoneSound() {
    dashStoneSound.play();
}
#endif