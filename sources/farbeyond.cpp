#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>

#include <FTGL/ftgl.h>

#include "gameplay/events.h"
#include "include/gui.h"
#include "include/gameplay.h"
#include "gameplay/observer.h"
#include "data/objects.h"
#include "data/importdb.h"
#include "include/data/planet.h"
#include "include/music.h"

int main(int argc, char *argv[]) {
    std::cout << std::endl << "-- " << PROGRAM_TITLE << std::endl << "-- " << PROGRAM_AUTHOR;
    std::cout << std::endl << "---------------------" << std::endl << std::endl;

    GLdouble fps = 0.0;

    char speed[80];
    char distance[80];
    char coord[80];
    double abs_distance;

    // Import the Star Database
    importStarsDBHip();
    import_galaxies();

    // Initialize OpenGL context
    gui_init();

    // Load star catalog and initialize OpenGL background
    init_sky();
    init_galaxies();

    // Initialize the game play
    joydetect();

    /* Initialize everything for the spaceship. It has to be a global variable */
    spaceship = new Observer();

    srand((unsigned int) time(NULL));

    // Sun
    //Planet Sun("Sun", 696 * 1e6, "sun.png");

    InitPlanet();

    // Initialization of OpenAL
    InitOpenAL();
    PlaySound();

    // Matrix
    quatToMatrix(&spaceship->quater, spaceship->matrix);
    spaceship->update();

    Uint32 elapsed_time;
    Uint32 start_time;

    while (!globalInput.key[SDLK_ESCAPE] && !globalInput.quit) {

        int MatSpec[4] = {1, 1, 1, 1};
        glMaterialiv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec);
        glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 100);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        /* These are to calculate our fps */
        static GLint T0 = 0;
        static GLint Frames = 0;
        start_time = SDL_GetTicks();

        // Update events
        process_events();
        evolve();
        spaceship->update();

        // Distance from (0,0,0)
        abs_distance = spaceship->get_distance();

        // No need to display planets if we are beyond the solar system
        if (abs_distance < 950 * 1.5e13) {

            glActiveTexture(GL_TEXTURE0);

            // ## TODO ##: Sun as a planet -> star or object
            //Sun.draw();
            //Sun.light();

            mercury->draw();
            mercury->drawOrbit();

            venus->draw();
            venus->drawOrbit();

            earth->draw();
            earth->drawOrbit();

            mars->draw();
            mars->drawOrbit();

            jupiter->draw();
            jupiter->drawOrbit();

            saturn->draw();
            saturn->drawOrbit();

            uranus->draw();
            uranus->drawOrbit();

            neptune->draw();
            neptune->drawOrbit();

            pluto->draw();
            pluto->drawOrbit();
        }

        // Galaxy example
        if (abs_distance < 150 * 1e21) {
            glPushMatrix();
            glTranslated(-spaceship->pos.x, -spaceship->pos.y, -spaceship->pos.z);
            glCallList(Sky);
            glPopMatrix();
        }

            // Edge of the universe
        else if (abs_distance < 150 * 1e28) {
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
        glTranslated(0.0f, 0.0f, -1.0f);

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
        glTranslatef(0.375, 0.375, 0.0); // exact pixel precision

        // Set text color
        glColor3f(1.0f, 1.0f, 1.0f);

        glRasterPos2d(0.0, 0.0);

        // Speed
        display_actual_speed(speed, spaceship->speed);
        FTPoint loc1(4.0, pScreen->h - 14);
        font->Render(speed, -1, loc1);

        // Distance
        display_actual_distance(distance, abs_distance);
        loc1.Y(pScreen->h - 2 * 14);
        font->Render(distance, -1, loc1);

        // Matrix
        snprintf(coord, 79, "Coord: (x=%.4g, y=%.4g, z=%.4g)", spaceship->pos.x, spaceship->pos.y, spaceship->pos.z);
        loc1.Y(pScreen->h - 3 * 14);
        font->Render(coord, -1, loc1);

        snprintf(coord, 79, "Quat: (w=%.4f, x=%.4f, y=%.4f, z=%.4f)",
                 spaceship->quater.qw, spaceship->quater.qx, spaceship->quater.qy,
                 spaceship->quater.qz);
        loc1.Y(pScreen->h - 4 * 14);
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
        glVertex2i(10, (int) (-0.00512 * (double) globalInput.speed + 155.0));
        glVertex2i(20, (int) (-0.00512 * (double) globalInput.speed + 155.0));
        glVertex2i(20, 155);
        glEnd();

        glBegin(GL_LINES);
        glVertex2i(pScreen->w / 2 + 2, pScreen->h / 2 + 2);
        glVertex2i(pScreen->w / 2 + 2, pScreen->h / 2 - 2);
        glVertex2i(pScreen->w / 2 - 2, pScreen->h / 2 - 2);
        glVertex2i(pScreen->w / 2 - 2, pScreen->h / 2 + 2);
        glEnd();

        /* Gather our frames per second */
        Frames++;
        {
            GLint t = SDL_GetTicks();
            if (t - T0 >= 5000) {
                GLdouble seconds = (t - T0) / 1000.0;
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

        elapsed_time = SDL_GetTicks() - start_time;
        if (elapsed_time <= 10) {
            SDL_Delay(10 - elapsed_time);
        }
    }
    return 0;
}
