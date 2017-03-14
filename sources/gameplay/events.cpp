#include <SDL/SDL.h>
#include <math.h>

#include "events.h"
#include "../include/data/planet.h"
#include "../include/music.h"
#include "../include/gui.h"
#include "../include/gameplay.h"
#include "observer.h"

Input globalInput;

void process_events() {
    SDL_Event event;

    globalInput.mousebuttons[SDL_BUTTON_WHEELUP] = 0;
    globalInput.mousebuttons[SDL_BUTTON_WHEELDOWN] = 0;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_ACTIVEEVENT:
                /*if(event.active.gain == 0)
                    isActive = 0;
                else
                    isActive = 1;*/
                break;

            case SDL_VIDEORESIZE:
                pScreen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32, videoFlags);
                if (!pScreen) {
                    fprintf(stderr, "Could not get a surface after resize: %s\n", SDL_GetError());
                    gui_close();
                }
                printf("Resized %d %d\n", event.resize.w, event.resize.h);
                gui_resize(event.resize.w, event.resize.h);
                break;

            case SDL_KEYDOWN:
                globalInput.key[event.key.keysym.sym] = 1;
                handleKeyPress(&event.key.keysym);
                break;

            case SDL_KEYUP:
                globalInput.key[event.key.keysym.sym] = 0;
                break;

            case SDL_MOUSEMOTION:
                globalInput.mousex = event.motion.x;
                globalInput.mousey = event.motion.y;
                globalInput.mousexrel = event.motion.xrel;
                globalInput.mouseyrel = event.motion.yrel;
                break;

            case SDL_MOUSEBUTTONDOWN:
                globalInput.mousebuttons[event.button.button] = 1;
                break;

            case SDL_MOUSEBUTTONUP:
                globalInput.mousebuttons[event.button.button] = 0;
                break;

            case SDL_JOYAXISMOTION:
                globalInput.x_move = SDL_JoystickGetAxis(joy, 0);
                globalInput.y_move = SDL_JoystickGetAxis(joy, 1);
                globalInput.x1_move = SDL_JoystickGetAxis(joy, 2);
                globalInput.speed = SDL_JoystickGetAxis(joy, 4);
                break;

            case SDL_JOYHATMOTION:
                //printf("Chapeau %d en position %d", event.jhat.hat, event.jhat.which);
                switch (event.jhat.value) {
                    case SDL_HAT_CENTERED:
                        printf("centrée\n");
                        break;
                    case SDL_HAT_UP:
                        printf("haute\n");
                        break;
                    case SDL_HAT_RIGHT:
                        printf("droite\n");
                        break;
                    case SDL_HAT_DOWN:
                        printf("basse\n");
                        break;
                    case SDL_HAT_LEFT:
                        printf("gauche\n");
                        break;
                    case SDL_HAT_RIGHTUP:
                        printf("haute/droite\n");
                        break;
                    case SDL_HAT_RIGHTDOWN:
                        printf("basse/droite\n");
                        break;
                    case SDL_HAT_LEFTUP:
                        printf("haute/gauche\n");
                        break;
                    case SDL_HAT_LEFTDOWN:
                        printf("basse/gauche\n");
                        break;
                    default:
                        break;
                }
                break;

                /* Déplacement trackball joystick */
            case SDL_JOYBALLMOTION:
                /*spritepos.x += event.jball.xrel;
                 spritepos.y += event.jball.yrel;*/
                printf("Trackball %d a bougé de (%d %d %d)\n", event.jball.ball,
                       event.jball.which, event.jball.xrel, event.jball.yrel);
                break;

            case SDL_JOYBUTTONDOWN:
                globalInput.button[event.jbutton.button] = 1;
                if (event.jbutton.button == 3) {
                    spaceship->rangespeed += 1;
                }
                if (event.jbutton.button == 2) {
                    spaceship->rangespeed -= 1;
                }
                if (event.jbutton.button == 7) {
                    globalInput.quit = 1;
                    gui_close();
                }
                break;

            case SDL_JOYBUTTONUP:
                globalInput.button[event.jbutton.button] = 0;
                break;

            case SDL_QUIT:
                globalInput.quit = 1;
                gui_close();
                break;

            default:
                break;
        }
    }
}

void handleKeyPress(SDL_keysym *keysym) {
    switch (keysym->sym) {
        case SDLK_ESCAPE:
            gui_close();
            break;

        case SDLK_F11:

            /*if (fullscreen) {
                SDL_WM_ToggleFullScreen(pScreen);
                //pScreen = SDL_SetVideoMode(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, SCREEN_BPP, videoFlags);
            }
            else {
                //
            }
            printf("Current Video Info: %d x %d pixels\n", videoInfo->current_w,videoInfo->current_h);*/
            break;

        case SDLK_KP1:
            mercury->lookAt();
            break;

        case SDLK_KP2:
            venus->lookAt();
            break;

        case SDLK_KP3:
            earth->lookAt();
            break;

        case SDLK_KP4:
            mars->lookAt();
            break;

        case SDLK_KP5:
            earth->lookAt();
            break;

        case SDLK_KP6:
            jupiter->lookAt();
            break;

        case SDLK_KP7:
            saturn->lookAt();
            break;

        case SDLK_KP8:
            uranus->lookAt();
            break;

        case SDLK_KP9:
            neptune->lookAt();
            break;

        case SDLK_KP_PLUS:
            spaceship->rangespeed += 1;
            break;

        case SDLK_KP_MINUS:
            spaceship->rangespeed -= 1;
            break;

        case SDLK_KP_MULTIPLY:
            globalInput.speed = 5.0d;
            break;

        case SDLK_KP_DIVIDE:
            globalInput.speed = -5.0d;
            break;

        default:
            break;
    }
}

void computeSpaceshipSpeed() {
    if (globalInput.speed < 0) {
        spaceship->speed = pow(abs(globalInput.speed), 1.7) * pow(10, spaceship->rangespeed - 6);
    } else {
        spaceship->speed = -pow(abs(globalInput.speed), 1.7) * pow(10, spaceship->rangespeed - 6);
    }
}


void evolve() {

    static Quat CamQuaternionLocalRotation;
    static double rotation;

    if (abs(globalInput.x_move) > 1800.0) {
        rotation = (double) globalInput.x_move / 1500000.0;
        CamQuaternionLocalRotation.qw = cos(rotation / 2);
        CamQuaternionLocalRotation.qx = spaceship->matrix[0] * sin(rotation / 2);
        CamQuaternionLocalRotation.qy = spaceship->matrix[4] * sin(rotation / 2);
        CamQuaternionLocalRotation.qz = spaceship->matrix[8] * sin(rotation / 2);
        quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
        quaternion_normalise(spaceship->quater);
        quaternion_to_matrix(&spaceship->quater, spaceship->matrix);
    }

    if (abs(globalInput.y_move) > 1800) {
        rotation = -(double) globalInput.y_move / 1500000.0;
        CamQuaternionLocalRotation.qw = cos(rotation / 2);
        CamQuaternionLocalRotation.qx = spaceship->matrix[1] * sin(rotation / 2);
        CamQuaternionLocalRotation.qy = spaceship->matrix[5] * sin(rotation / 2);
        CamQuaternionLocalRotation.qz = spaceship->matrix[9] * sin(rotation / 2);
        quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
        quaternion_normalise(spaceship->quater);
        quaternion_to_matrix(&spaceship->quater, spaceship->matrix);
    }

    if (abs(globalInput.x1_move) > 1600) {
        rotation = -(double) globalInput.x1_move / 1000000.0;
        CamQuaternionLocalRotation.qw = cos(rotation / 2);
        CamQuaternionLocalRotation.qx = spaceship->matrix[2] * sin(rotation / 2);
        CamQuaternionLocalRotation.qy = spaceship->matrix[6] * sin(rotation / 2);
        CamQuaternionLocalRotation.qz = spaceship->matrix[10] * sin(rotation / 2);
        quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
        quaternion_normalise(spaceship->quater);
        quaternion_to_matrix(&spaceship->quater, spaceship->matrix);
    }

    computeSpaceshipSpeed();

    if (globalInput.button[7]) {
        //Quit(0);
    }
    if (globalInput.button[0] == 1) {
        PlaySound();
    }
    if (globalInput.key[SDLK_q] == 1 || globalInput.key[SDLK_a] == 1) {
        rotation = -0.03;
        CamQuaternionLocalRotation.qw = cos(rotation / 2);
        CamQuaternionLocalRotation.qx = spaceship->matrix[0] * sin(rotation / 2);
        CamQuaternionLocalRotation.qy = spaceship->matrix[4] * sin(rotation / 2);
        CamQuaternionLocalRotation.qz = spaceship->matrix[8] * sin(rotation / 2);
        quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
        quaternion_normalise(spaceship->quater);
        quaternion_to_matrix(&spaceship->quater, spaceship->matrix);
    }
    if (globalInput.key[SDLK_d] == 1) {
        rotation = 0.03;
        CamQuaternionLocalRotation.qw = cos(rotation / 2);
        CamQuaternionLocalRotation.qx = spaceship->matrix[0] * sin(rotation / 2);
        CamQuaternionLocalRotation.qy = spaceship->matrix[4] * sin(rotation / 2);
        CamQuaternionLocalRotation.qz = spaceship->matrix[8] * sin(rotation / 2);
        quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
        quaternion_normalise(spaceship->quater);
        quaternion_to_matrix(&spaceship->quater, spaceship->matrix);
    }
    if (globalInput.key[SDLK_z] == 1 || globalInput.key[SDLK_w] == 1) {
        rotation = 0.03;
        CamQuaternionLocalRotation.qw = cos(rotation / 2);
        CamQuaternionLocalRotation.qx = spaceship->matrix[1] * sin(rotation / 2);
        CamQuaternionLocalRotation.qy = spaceship->matrix[5] * sin(rotation / 2);
        CamQuaternionLocalRotation.qz = spaceship->matrix[9] * sin(rotation / 2);
        quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
        quaternion_normalise(spaceship->quater);
        quaternion_to_matrix(&spaceship->quater, spaceship->matrix);
    }
    if (globalInput.key[SDLK_s] == 1) {
        rotation = -0.03;
        CamQuaternionLocalRotation.qw = cos(rotation / 2);
        CamQuaternionLocalRotation.qx = spaceship->matrix[1] * sin(rotation / 2);
        CamQuaternionLocalRotation.qy = spaceship->matrix[5] * sin(rotation / 2);
        CamQuaternionLocalRotation.qz = spaceship->matrix[9] * sin(rotation / 2);
        quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
        quaternion_normalise(spaceship->quater);
        quaternion_to_matrix(&spaceship->quater, spaceship->matrix);
    }

    if (globalInput.key[SDLK_RIGHT] == 1) {
        spaceship->pos.x -= 0.5 * (spaceship->matrix[1]);
        spaceship->pos.y -= 0.5 * (spaceship->matrix[5]);
        spaceship->pos.z -= 0.5 * (spaceship->matrix[9]);
    }
    if (globalInput.key[SDLK_LEFT] == 1) {
        spaceship->pos.x += 0.5 * (spaceship->matrix[1]);
        spaceship->pos.y += 0.5 * (spaceship->matrix[5]);
        spaceship->pos.z += 0.5 * (spaceship->matrix[9]);
    }
    if (globalInput.key[SDLK_UP] == 1) {
        if (globalInput.key[SDLK_a] == 1) {
            spaceship->pos.x += 10000000000000.0 * (spaceship->matrix[0]);
            spaceship->pos.y += 10000000000000.0 * (spaceship->matrix[4]);
            spaceship->pos.z += 10000000000000.0 * (spaceship->matrix[8]);
        }
        if (globalInput.key[SDLK_e] == 1) {
            spaceship->pos.x += 40.0 * (spaceship->matrix[0]);
            spaceship->pos.y += 40.0 * (spaceship->matrix[4]);
            spaceship->pos.z += 40.0 * (spaceship->matrix[8]);
        }
        spaceship->pos.x += 0.5 * (spaceship->matrix[0]);
        spaceship->pos.y += 0.5 * (spaceship->matrix[4]);
        spaceship->pos.z += 0.5 * (spaceship->matrix[8]);
    }
    if (globalInput.key[SDLK_DOWN] == 1) {
        spaceship->pos.x -= 0.5 * (spaceship->matrix[0]);
        spaceship->pos.y -= 0.5 * (spaceship->matrix[4]);
        spaceship->pos.z -= 0.5 * (spaceship->matrix[8]);
    }
}
