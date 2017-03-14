#ifndef MUSIC_H_
#define MUSIC_H_

#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>

bool InitOpenAL(const char *DeviceName = NULL);

ALuint LoadSound(const std::string &Filename);

void PlaySound();

void CloseAudio();

#endif /* MUSIC_H_ */