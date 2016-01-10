/*
 ============================================================================
 Name        : spacesimu.c
 Author      : Nicolas Hess
 Version     :
 Copyright   : Your copyright notice
 Description : Space Simulator, drive your own spaceship throught space
 ============================================================================
 */

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <FTGL/ftgl.h>

#include "include/main.h"
#include "include/glsl.h"
#include "include/lights.h"
#include "gameplay/events.h"
#include "include/gui.h"
#include "include/gameplay.h"
#include "gameplay/observer.h"
#include "data/objects.h"
#include "data/importdb.h"
#include "data/planet.h"
#include "include/utils.h"
#include "include/music.h"
#include "textures/loadpng.h"

using namespace std;

int main(int argc, char *argv[])
{

	cout << endl << "-- " << PROGRAM_TITLE << endl << "-- " << PROGRAM_AUTHOR;
	cout << endl << "---------------------" << endl << endl;

	GLfloat fps = 0.0;

	char speed[80];
	char distance[80];
	char coord[80];
	double Fdistance;

	/* Import the Star Database */
	//importStarsDB();
	importStarsDBHip();
	import_galaxies();

	/* Initialize OpenGL context */
	gui_init();

	/* Load star catalog and inilialize opengl background */
	init_sky();
	init_galaxies();

	/* Initialize the Gameplay */
	joydetect();

	/* Initialize everything for the spaceship. It has to be a global variable */
	spaceship = new Observer();

	GLuint program;
	program = glCreateProgram();
	GLuint vs = LoadShader(GL_VERTEX_SHADER, "shaders/SkyFromSpace2.vert");
	GLuint fs = LoadShader(GL_FRAGMENT_SHADER, "shaders/SkyFromSpace2.frag");
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	LinkShader(program);

	/*GLuint programEarth;
	programEarth = glCreateProgram();
	GLuint vsEarth = LoadShader(GL_VERTEX_SHADER, "shaders/EarthFromSpace.vert");
	GLuint fsEarth = LoadShader(GL_FRAGMENT_SHADER, "shaders/EarthFromSpace.frag");
	glAttachShader(programEarth, vsEarth);
	glAttachShader(programEarth, fsEarth);
	LinkShader(programEarth);*/

	srand((unsigned int) time(NULL));

	/* Chargement des textures */
	if (!(texEarth = loadPNGTexture("./planets/textures/earth.png"))) {
		fprintf(stderr, "failed to load earth! aborting\n");
		exit(1);
	}

	/* Chargement des textures */
	if (!(texEarthN = loadPNGTexture("./planets/textures/earth_night.png"))) {
		fprintf(stderr, "failed to load earth! aborting\n");
		exit(1);
	}

	/*GLuint programLight;
	programLight = glCreateProgram();
	GLuint ligthVert = LoadShader(GL_VERTEX_SHADER, "shaders/ligth.vert");
	GLuint lightFrag = LoadShader(GL_FRAGMENT_SHADER, "shaders/ligth.frag");
	glAttachShader(programLight, ligthVert);
	glAttachShader(programLight, lightFrag);
	LinkShader(programLight);*/

	/* Sun */
	Planet Sun("Sun", 696 * 1e6, "sun.png");

	EllipticalOrbit e;

	/* Mercury */
	Planet Mercury("Mercury", 2400000, "mercury.png");
	e.SemiMajorAxis = 57909100000.0;
	e.Eccentricity = 0.205630;
	Mercury.setOrbit(e);

	Planet Venus("Venus", 6050000, "venus.png");
	e.SemiMajorAxis = 108208000000.0;
	e.Eccentricity = 0.0067;
	Venus.setOrbit(e);

	Planet Earth("Earth", 6357000, "earth.png");
	e.SemiMajorAxis = 149598261000.0;
	e.Eccentricity = 0.01671123;
	Earth.setOrbit(e);

	//Planet Moon("Moon", 1700000, "moon.png");

	Planet Mars("Mars", 3350000, "mars.png");
	e.SemiMajorAxis = 227939100000;
	e.Eccentricity = 0.093315;
	Mars.setOrbit(e);

	Planet Jupiter("Jupiter", 71000000, "jupiter.png");
	e.SemiMajorAxis = 778547200000;
	e.Eccentricity = 0.048775;
	Jupiter.setOrbit(e);

	Planet Saturn("Saturn", 60000000, "saturn.png");
	e.SemiMajorAxis = 1433449370000;
	e.Eccentricity = 0.055723219;
	Saturn.setOrbit(e);
	Saturn.set_atm_height(5000000);
	Saturn.add_rings(74500000, 140220000, "./planets/textures/saturnrings.png");

	Planet Uranus("Uranus", 25500000, "uranus.png");
	e.SemiMajorAxis = 2876679082000;
	e.Eccentricity = 0.044405586;
	Uranus.setOrbit(e);

	Planet Neptune("Neptune", 24500000, "neptune.png");
	e.SemiMajorAxis = 4503443661000;
	e.Eccentricity = 0.011214269;
	Neptune.setOrbit(e);

	Planet Pluto("Pluto", 1150000, "pluto.png");
	e.SemiMajorAxis = 5874000000000;
	e.Eccentricity = 0.24880766;
	Pluto.setOrbit(e);

	initEarthGd(Rt, 0, 0, 2.5 * Rg, texEarth);
	initEarthAtm(Rt + 50000, 0, 0, 2.5 * Rg);

	// Initialisation d'OpenAL
	InitOpenAL();
	PlaySound();

	// Matrix
	quatToMatrix(&spaceship->quater, spaceship->matrix);
	spaceship->update();

	Uint32 last_time = SDL_GetTicks();
	Uint32 ellapsed_time;
	Uint32 start_time;

	while (!globalInput.key[SDLK_ESCAPE] && !globalInput.quit) {

		int MatSpec [4] = {1, 1, 1, 1};
		glMaterialiv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 100);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		/* These are to calculate our fps */
		static GLint T0 = 0;
		static GLint Frames = 0;

		start_time = SDL_GetTicks();

		ellapsed_time = start_time - last_time;
		last_time = start_time;

		/* Mise à jour de l'état du jeu à partir des évènements */
		process_events();
		evolve();
		spaceship->update();

		// Distance from (0,0,0)
		Fdistance = spaceship->get_distance();

		// No need to display planets if we are beyond the solar system
		if (Fdistance < 950 * 1.5e13) {

			glActiveTexture(GL_TEXTURE0);

			// ## TODO ##: Sun as a planet -> star or object
			Sun.draw();
			//Sun.light();

			Mercury.draw();
			Mercury.draw_orbit();

			Venus.draw();
			Venus.draw_orbit();

			Earth.draw();
			Earth.draw_orbit();

			Mars.draw();
			Mars.draw_orbit();

			Jupiter.draw();
			Jupiter.draw_orbit();

			Saturn.draw();
			//Saturn.draw_atm();
			Saturn.draw_orbit();

			Uranus.draw();
			Uranus.draw_orbit();

			Neptune.draw();
			Neptune.draw_orbit();

			Pluto.draw();
			Pluto.draw_orbit();

			/*//glPushMatrix();
			//glTranslated(0.0, 20 * 1e7, 0.0);

			glUseProgram(programEarth);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texEarth);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, texEarthN);
			glUniform1i(idGroundColorMap, 1);
			glUniform1i(idGroundColorMapNight, 2);
			glUniform3fv(idGroundSunPosition, 1, vecSunPosition);
			glUniform3fv(idGroundCamPosition, 1, vecCamPosition);
			glCallList(EarthGd);
			glUseProgram(0);

			// Program for the Atm
			glUseProgram(program);
			glUniform3fv(idCamPosition, 1, vecCamPosition);
			glUniform3fv(idSunPosition, 1, vecSunPosition);
			glCallList(EarthAtm);
			glUseProgram(0);

			//glPopMatrix();*/
		}

		// Galaxy example
		if (Fdistance < 150 * 1e21) {
			glPushMatrix();
			glTranslated(-spaceship->pos.x, -spaceship->pos.y, -spaceship->pos.z);
			glCallList(Sky);
			glPopMatrix();
		}

		// Edge of the universe
		else if (Fdistance < 150 * 1e28) {
			glPushMatrix();
			glTranslated(-spaceship->pos.x, -spaceship->pos.y, -spaceship->pos.z);
			glCallList(Galaxy_Sky);
			glPopMatrix();
		}

		//--------> Begin 2D

		// Clear Screen And Depth Buffer
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Reset The Current Modelview Matrix
		glLoadIdentity();

		// Move Out One Unit From The Screen
		glTranslated(0.0f,0.0f,-1.0f);

		// do any 3D stuff

		// switch to 2D mode (orthographic)
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, pScreen->w, 0.0f, pScreen->h, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
		glTranslatef (0.375, 0.375, 0.); // exact pixel precision

		// Set text color
		glColor3f(1.0f,1.0f,1.0f);

		glRasterPos2d(0.0, 0.0);

		// Speed
		display_actual_speed(speed, spaceship->speed);
		FTPoint loc1(4.0, pScreen->h - 14);
		font->Render(speed, -1, loc1);

		// Distance
		display_actual_distance(distance, Fdistance);
		loc1.Y(pScreen->h - 2*14);
		font->Render(distance, -1, loc1);

		// Matrix
		snprintf(coord, 79, "Coord: (x=%.4g, y=%.4g, z=%.4g)", spaceship->pos.x,
						spaceship->pos.y, spaceship->pos.z);
		loc1.Y(pScreen->h - 3*14);
		font->Render(coord, -1, loc1);

		snprintf(coord, 79, "Quat: (w=%.4f, x=%.4f, y=%.4f, z=%.4f)",
				spaceship->quater.qw, spaceship->quater.qx, spaceship->quater.qy,
				spaceship->quater.qz);
		loc1.Y(pScreen->h - 4*14);
		font->Render(coord, -1, loc1);

		glBegin(GL_LINES);
		glVertex2i(5, 155);
		glVertex2i(25, 155);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex2i(10, 9);
		glVertex2i(10, 301);
		glVertex2i(21, 301);
		glVertex2i(21, 9);
		glEnd();

		glBegin(GL_QUADS);
		glVertex2i(10, 155);
		glVertex2i(10, -0.00512 * (double) (int) globalInput.speed + 155.0);
		glVertex2i(20, -0.00512 * (double) (int) globalInput.speed + 155.0);
		glVertex2i(20, 155);
		glEnd();

		glBegin (GL_LINES);
		glVertex2i (pScreen->w/2+2, pScreen->h/2+2);
		glVertex2i (pScreen->w/2+2, pScreen->h/2-2);
		glVertex2i (pScreen->w/2-2, pScreen->h/2-2);
		glVertex2i (pScreen->w/2-2, pScreen->h/2+2);
		glEnd ();

		/* Gather our frames per second */
		Frames++;
		{
			GLint t = SDL_GetTicks();
			if (t - T0 >= 5000) {
				GLfloat seconds = (t - T0) / 1000.0;
				fps = Frames / seconds;
				T0 = t;
				Frames = 0;
			}
		}
		loc1.X(pScreen->w - 43);
		loc1.Y(pScreen->h - 14);
		snprintf(coord, 79, "%2.2g FPS", fps);
		font->Render(coord, -1, loc1);

		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		//--------> End 2D

		glFlush();
		SDL_GL_SwapBuffers();

		ellapsed_time = SDL_GetTicks() - start_time;
		if (ellapsed_time <= 10) {
			SDL_Delay(10 - ellapsed_time);
		}
	}
	return 0;
}
