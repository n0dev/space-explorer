#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include "objects.h"
#include "importdb.h"

/* texture Id */
GLuint texEarth;
GLuint texEarthN;

/* Stars and galaxies */
GLuint Sky;
GLuint Galaxy_Sky;
GLuint buf;

GLuint EarthGd;
GLuint EarthAtm;

float pos[N_VERTS * P_SIZE] =
        {
                -0.8 * 1e8, -0.8 * 1e8, 1e8,
                0.8 * 1e8, -0.8 * 1e8, 1000,
                0.0 * 1e8, 0.8 * 1e8, 1000
        };

float colors[N_VERTS * C_SIZE] =
        {
                1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 0.0, 1.0
        };

void init_sky() {
    int i;

    /* on cree un objet et on recupere son identifiant */
    Sky = glGenLists(1);
    if (Sky == 0) {
        fprintf(stderr, "erreur lors de la creation de la liste\n");
        exit(-1);
    }

    glNewList(Sky, GL_COMPILE);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (i = 0; i < nb_stars; i++) {
        glColor4f((float) Stars[i].color[0], (float) Stars[i].color[1], (float) Stars[i].color[2],
                  (float) Stars[i].alpha);
        glVertex3d(Stars[i].x, Stars[i].y, Stars[i].z);
    }
    glEnd();
    glEndList();
}

void init_galaxies() {
    int i;

    /* on cree un objet et on recupere son identifiant */
    Galaxy_Sky = glGenLists(1);
    if (Galaxy_Sky == 0) {
        fprintf(stderr, "[*] Erreur lors de la creation de la liste galaxies\n");
        exit(-1);
    }

    glNewList(Galaxy_Sky, GL_COMPILE);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (i = 0; i < nb_galaxies; i++) {
        glVertex4d(Galaxies[i].x, Galaxies[i].y, Galaxies[i].z, Galaxies[i].alpha);
    }
    glEnd();
    glEndList();

}

void finalize() {
    glDeleteBuffers(1, &buf);
}

