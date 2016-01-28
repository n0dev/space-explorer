#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>

bool InitOpenAL(const char* DeviceName = NULL);

ALuint LoadSound(const std::string& Filename);

void PlaySound();

void CloseAudio();
