#ifndef _H_AUDIO_MANAGER_H_ 
#define _H_AUDIO_MANAGER_H_

#include "raylib.h"

class AudioManager {
private:
    AudioManager() = default;
    virtual ~AudioManager() = default;

    AudioManager(AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    static AudioManager* _instance;

    struct Audio {
        union {
            Sound sound;
            Music music;
        }audio;
        const char* path;
        float volume;

    };

public:

    static AudioManager* Instance();
    static void Release();

    void Init();

    void UpdateMusic(Music music);
    void PlayMusic(Music music, float volume = musicVolume);
    void StopMusic(Music music);
    void ResumeMusic(Music music);
    void ResetMusic(Music music);
    void SetVolumeMusic(Music music, float volume);
    bool IsPlayingMusic(Music music);

    void PlaySound(Sound sound, float volume = SFXVolume);
    void StopSound(Sound sound);
    void ResumeSound(Sound sound);
    void ResetSound(Sound sound);
    bool IsPlayingSound(Sound sound);
    void SetVolumeSound(Sound sound, float volume);
    void SetPanSound(Sound sound, float pan);

    void SetGlobalVolume(float volume);
    float GetGlobalVolume();

    inline static float SFXVolume = 0.5f; // Volumen global del sonido
    inline static float musicVolume = 0.5f; // Volumen global de la musica
};

#endif //_H_AUDIO_MANAGER_H_
