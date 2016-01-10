#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

bool InitOpenAL(const char* DeviceName = NULL);

ALuint LoadSound(const std::string& Filename);

void PlaySound();

void CloseAudio();
