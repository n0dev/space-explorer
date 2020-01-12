#include <iostream>
#include <SDL/SDL_joystick.h>

using namespace std;

int nbjoysticks;
SDL_Joystick *joy;

void joydetect(void) {
    int i;
    cout << "[+] Current number of joystick: ";
    nbjoysticks = SDL_NumJoysticks();

    for (i = 0; i < nbjoysticks; i++) {
        joy = SDL_JoystickOpen(i);
        /*printf("[+] Joystick %d: %s\n", i, SDL_JoystickName(i));
        printf("[+] Device index: %d\n", SDL_JoystickIndex(joy));
        printf("[+] Trackballs: %d\n", SDL_JoystickNumBalls(joy));
        printf("[+] POV Hats: %d\n", SDL_JoystickNumHats(joy));
        printf("[+] Axes: %d\n", SDL_JoystickNumAxes(joy));
        printf("[+] Buttons: %d\n", SDL_JoystickNumButtons(joy));*/
    }
    cout << nbjoysticks << endl;
}
