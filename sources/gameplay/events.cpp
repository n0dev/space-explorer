#include <SDL/SDL.h>
#include <math.h>

#include "events.h"
#include "../include/music.h"
#include "../include/gui.h"
#include "../include/gameplay.h"
#include "observer.h"

Input globalInput;

void shoot() {
	printf("test");
}

/* Gestion des évènements d'entrée */
void process_events() {
	SDL_Event event;

	globalInput.mousebuttons[SDL_BUTTON_WHEELUP] = 0;
	globalInput.mousebuttons[SDL_BUTTON_WHEELDOWN] = 0;

	/* Un évènement attend d'être traité? */
	while (SDL_PollEvent(&event)) {

		/* Si oui, quel type? */
		switch (event.type) {

		/* Something's happen with our focus. If we lost focus or we are
		 * iconified, we shouldn't draw the screen */
		case SDL_ACTIVEEVENT:
			/*if(event.active.gain == 0)
				isActive = 0;
			else
				isActive = 1;*/
			break;

			/* handle resize event */
		case SDL_VIDEORESIZE:
			pScreen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32, videoFlags);
			if (!pScreen) {
				fprintf(stderr, "Could not get a surface after resize: %s\n", SDL_GetError());
				gui_close();
			}
			printf("Resized %d %d\n", event.resize.w, event.resize.h);
			gui_resize(event.resize.w, event.resize.h);
			break;

			/* Appui sur une touche */
		case SDL_KEYDOWN:
			globalInput.key[event.key.keysym.sym] = 1;
			handleKeyPress(&event.key.keysym);
			break;

			/* Relâchement d'une touche */
		case SDL_KEYUP:
			globalInput.key[event.key.keysym.sym] = 0;
			break;

			/* Déplacement souris */
		case SDL_MOUSEMOTION:
			globalInput.mousex = event.motion.x;
			globalInput.mousey = event.motion.y;
			globalInput.mousexrel = event.motion.xrel;
			globalInput.mouseyrel = event.motion.yrel;
			break;

			/* Enfoncement bouton souris */
		case SDL_MOUSEBUTTONDOWN:
			globalInput.mousebuttons[event.button.button] = 1;
			break;

			/* Relâchement bouton souris */
		case SDL_MOUSEBUTTONUP:
			globalInput.mousebuttons[event.button.button] = 0;
			break;

			/* Déplacement axe joystick */
		case SDL_JOYAXISMOTION:
			globalInput.x_move = SDL_JoystickGetAxis(joy, 0);
			globalInput.y_move = SDL_JoystickGetAxis(joy, 1);
			globalInput.x1_move = SDL_JoystickGetAxis(joy, 2);
			globalInput.speed = SDL_JoystickGetAxis(joy, 4);
			break;

			/* Déplacement chapeau joystick */
		case SDL_JOYHATMOTION:
			printf("Chapeau %d en position %d", event.jhat.hat, event.jhat.which);
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
			}
			break;

			/* Déplacement trackball joystick */
		case SDL_JOYBALLMOTION:
			/*spritepos.x += event.jball.xrel;
			 spritepos.y += event.jball.yrel;*/
			printf("Trackball %d a bougé de (%d %d %d)\n", event.jball.ball,
					event.jball.which, event.jball.xrel, event.jball.yrel);
			break;

			/* Appui bouton joystick */
		case SDL_JOYBUTTONDOWN:
			globalInput.button[event.jbutton.button] = 1;
			if(event.jbutton.button == 3) {
				spaceship->rangespeed += 1;
			}
			if(event.jbutton.button == 2) {
				spaceship->rangespeed -= 1;
			}
			if(event.jbutton.button == 7) {
				globalInput.quit = 1;
				gui_close();
			}
			break;

			/* Relâchement bouton joystick */
		case SDL_JOYBUTTONUP:
			globalInput.button[event.jbutton.button] = 0;
			break;

			/* handle quit requests */
		case SDL_QUIT:
			globalInput.quit = 1;
			gui_close();
			break;
		default:
			break;
		}
	}
}

bool fullscreen = false;

/* function to handle key press events */
void handleKeyPress(SDL_keysym *keysym) {
	switch (keysym->sym) {
	case SDLK_ESCAPE:
		/* ESC key was pressed */
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
        std::cout << "DONE" << std::endl;
        break;
	default:
		break;
	}
}

void computeSpaceshipSpeed() {
	if (globalInput.speed < 0) {
		spaceship->speed = (double) pow(abs(globalInput.speed), 1.7) * pow(10, spaceship->rangespeed-6);
	} else {
		spaceship->speed = -(double) pow(abs(globalInput.speed), 1.7) * pow(10, spaceship->rangespeed-6);
	}
}


void evolve() {

	static Quat CamQuaternionLocalRotation;
	static double rotation;

	if (abs(globalInput.x_move) > 1800.0) {
		rotation = (double) globalInput.x_move / 1500000.0;
		CamQuaternionLocalRotation.qw = cos(rotation/2);
		CamQuaternionLocalRotation.qx = spaceship->matrix[0] * sin(rotation/2);
		CamQuaternionLocalRotation.qy = spaceship->matrix[4] * sin(rotation/2);
		CamQuaternionLocalRotation.qz = spaceship->matrix[8] * sin(rotation/2);
		quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
		quatNormalise(spaceship->quater);
		quatToMatrix(&spaceship->quater, spaceship->matrix);
	}

	if (abs(globalInput.y_move) > 1800) {
		rotation = -(double) globalInput.y_move / 1500000.0;
		CamQuaternionLocalRotation.qw = cos(rotation/2);
		CamQuaternionLocalRotation.qx = spaceship->matrix[1] * sin(rotation/2);
		CamQuaternionLocalRotation.qy = spaceship->matrix[5] * sin(rotation/2);
		CamQuaternionLocalRotation.qz = spaceship->matrix[9] * sin(rotation/2);
		quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
		quatNormalise(spaceship->quater);
		quatToMatrix(&spaceship->quater, spaceship->matrix);
	}

	if (abs(globalInput.x1_move) > 1600) {
		rotation = -(double) globalInput.x1_move / 1000000.0;
		CamQuaternionLocalRotation.qw = cos(rotation/2);
		CamQuaternionLocalRotation.qx = spaceship->matrix[2] * sin(rotation/2);
		CamQuaternionLocalRotation.qy = spaceship->matrix[6] * sin(rotation/2);
		CamQuaternionLocalRotation.qz = spaceship->matrix[10]* sin(rotation/2);
		quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
		quatNormalise(spaceship->quater);
		quatToMatrix(&spaceship->quater, spaceship->matrix);
	}

	computeSpaceshipSpeed();

	if (globalInput.button[7]) {
		//Quit(0);
	}
	if (globalInput.button[0] == 1) {
		PlaySound();
	}
	if(globalInput.key[SDLK_q] == 1) {
		rotation = -0.03;
		CamQuaternionLocalRotation.qw = cos(rotation / 2);
		CamQuaternionLocalRotation.qx = spaceship->matrix[0] * sin(rotation / 2);
		CamQuaternionLocalRotation.qy = spaceship->matrix[4] * sin(rotation / 2);
		CamQuaternionLocalRotation.qz = spaceship->matrix[8] * sin(rotation / 2);
		quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
		quatNormalise(spaceship->quater);
		quatToMatrix(&spaceship->quater, spaceship->matrix);
	}
	if(globalInput.key[SDLK_d] == 1) {
		rotation = 0.03;
		CamQuaternionLocalRotation.qw = cos(rotation / 2);
		CamQuaternionLocalRotation.qx = spaceship->matrix[0] * sin(rotation / 2);
		CamQuaternionLocalRotation.qy = spaceship->matrix[4] * sin(rotation / 2);
		CamQuaternionLocalRotation.qz = spaceship->matrix[8] * sin(rotation / 2);
		quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
		quatNormalise(spaceship->quater);
		quatToMatrix(&spaceship->quater, spaceship->matrix);
	}
	if(globalInput.key[SDLK_z] == 1) {
		rotation = 0.03;
		CamQuaternionLocalRotation.qw = cos(rotation / 2);
		CamQuaternionLocalRotation.qx = spaceship->matrix[1] * sin(rotation / 2);
		CamQuaternionLocalRotation.qy = spaceship->matrix[5] * sin(rotation / 2);
		CamQuaternionLocalRotation.qz = spaceship->matrix[9] * sin(rotation / 2);
		quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
		quatNormalise(spaceship->quater);
		quatToMatrix(&spaceship->quater, spaceship->matrix);
	}
	if(globalInput.key[SDLK_s] == 1) {
		rotation = -0.03;
		CamQuaternionLocalRotation.qw = cos(rotation / 2);
		CamQuaternionLocalRotation.qx = spaceship->matrix[1] * sin(rotation / 2);
		CamQuaternionLocalRotation.qy = spaceship->matrix[5] * sin(rotation / 2);
		CamQuaternionLocalRotation.qz = spaceship->matrix[9] * sin(rotation / 2);
		quatMultiply(spaceship->quater, CamQuaternionLocalRotation, spaceship->quater);
		quatNormalise(spaceship->quater);
		quatToMatrix(&spaceship->quater, spaceship->matrix);
	}
	if(globalInput.key[SDLK_RIGHT] == 1) {
		spaceship->pos.x -= 0.5 * (spaceship->matrix[1]);
		spaceship->pos.y -= 0.5 * (spaceship->matrix[5]);
		spaceship->pos.z -= 0.5 * (spaceship->matrix[9]);
	}
	if(globalInput.key[SDLK_LEFT] == 1) {
		spaceship->pos.x += 0.5 * (spaceship->matrix[1]);
		spaceship->pos.y += 0.5 * (spaceship->matrix[5]);
		spaceship->pos.z += 0.5 * (spaceship->matrix[9]);
	}
	if(globalInput.key[SDLK_UP] == 1) {
		if(globalInput.key[SDLK_a] == 1) {
			spaceship->pos.x += 10000000000000.0 * (spaceship->matrix[0]);
			spaceship->pos.y += 10000000000000.0 * (spaceship->matrix[4]);
			spaceship->pos.z += 10000000000000.0 * (spaceship->matrix[8]);
		}
		if(globalInput.key[SDLK_e] == 1) {
			spaceship->pos.x += 40.0 * (spaceship->matrix[0]);
			spaceship->pos.y += 40.0 * (spaceship->matrix[4]);
			spaceship->pos.z += 40.0 * (spaceship->matrix[8]);
		}
		spaceship->pos.x += 0.5 * (spaceship->matrix[0]);
		spaceship->pos.y += 0.5 * (spaceship->matrix[4]);
		spaceship->pos.z += 0.5 * (spaceship->matrix[8]);
	}
	if(globalInput.key[SDLK_DOWN] == 1) {
		spaceship->pos.x -= 0.5 * (spaceship->matrix[0]);
		spaceship->pos.y -= 0.5 * (spaceship->matrix[4]);
		spaceship->pos.z -= 0.5 * (spaceship->matrix[8]);
	}
}
