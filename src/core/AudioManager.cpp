#include "AudioManager.h"

#include <string.h>
#include <assert.h>

AudioManager* AudioManager::_instance = nullptr;

void AudioManager::Init() {
    InitAudioDevice();
    assert(IsAudioDeviceReady());
    SetGlobalVolume(1.0f);
}

void AudioManager::Release() {
    CloseAudioDevice();
    if (_instance != nullptr) delete _instance;

    _instance = nullptr;
}

AudioManager* AudioManager::Instance() {
    if (_instance == nullptr) _instance = new AudioManager;
    return _instance;
}

void AudioManager::PlayMusic(Music music, float volume) {
    if (IsMusicStreamPlaying(music)) return;
    //StopMusicStream(music);

    SetVolumeMusic(music, volume);
    PlayMusicStream(music);
}

void AudioManager::StopMusic(Music music) {
    PauseMusicStream(music);
}

void AudioManager::UpdateMusic(Music music) {
    if (IsMusicStreamPlaying(music)) UpdateMusicStream(music);
}

void AudioManager::ResumeMusic(Music music) {
    ResumeMusicStream(music);
}

void AudioManager::ResetMusic(Music music) {
    StopMusicStream(music);
}

void AudioManager::SetVolumeMusic(Music music, float volume) {
    SetMusicVolume(music, volume);
}

void AudioManager::PlaySound(Sound sound, float volume) {
    if (IsSoundPlaying(sound))
        ::StopSound(sound);

    SetVolumeSound(sound, volume);
    ::PlaySound(sound);
}

void AudioManager::StopSound(Sound sound) {
    ::StopSound(sound);
}

void AudioManager::ResumeSound(Sound sound) {
    ::ResumeSound(sound);
}

void AudioManager::ResetSound(Sound sound) {
    StopSound(sound);
}

bool AudioManager::IsPlayingSound(Sound sound) {
    return IsSoundPlaying(sound);
}

bool AudioManager::IsPlayingMusic(Music music) {
    return IsMusicStreamPlaying(music);
}

void AudioManager::SetVolumeSound(Sound sound, float volume) {
    SetSoundVolume(sound, volume);
}

void AudioManager::SetPanSound(Sound sound, float pan) {
    SetSoundPan(sound, pan);
}

void AudioManager::SetGlobalVolume(float volume) {
    SetMasterVolume(volume);
}

float AudioManager::GetGlobalVolume() {
    return GetMasterVolume();
}
