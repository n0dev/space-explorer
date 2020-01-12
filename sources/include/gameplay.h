#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#include <SDL/SDL.h>

extern SDL_Joystick *joy;

// Detect available joystick
void joydetect(void);

// Detect available keyboard
void keyboarddetect(void);

// Detect available mouse
void mousedetect(void);

#endif /* GAMEPLAY_H_ */
