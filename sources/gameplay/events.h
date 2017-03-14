#ifndef EVENTS_H_
#define EVENTS_H_

#include <SDL/SDL.h>

typedef struct {
    char key[SDLK_LAST];
    int mousex, mousey;
    int mousexrel, mouseyrel;
    int mousebuttons[8];

    Sint16 x_move, y_move, x1_move, speed;
    int button[20];

    int quit;
} Input;

extern Input globalInput;

void process_events();

void handleKeyPress(SDL_keysym *keysym);

void evolve();

void computeSpaceshipSpeed();

#endif /* EVENTS_H_ */