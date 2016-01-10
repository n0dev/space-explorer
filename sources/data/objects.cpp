#include <SDL/SDL.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>

#include "objects.h"
#include "../include/glsl.h"
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

float pos[N_VERTS*P_SIZE] =
{
		-0.8*1e8, -0.8*1e8, 1e8,
		0.8*1e8, -0.8*1e8, 1000,
		0.0*1e8, 0.8*1e8, 1000
};

float colors[N_VERTS*C_SIZE] =
{
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
};

void initEarthGd(double radius, double px, double py, double pz, GLuint texture)
{
	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, 1);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	//Making a display list
	EarthGd= glGenLists(1);
	glNewList(EarthGd, GL_COMPILE);
	gluSphere(sphere, radius, 400, 1000);
	glEndList();
	gluDeleteQuadric(sphere);
	glPopMatrix();
}


void initEarthAtm(double radius, double px, double py, double pz)
{
	glPushMatrix();
	glTranslated(px, py, pz);
	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, 1);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	//Making a display list
	EarthAtm= glGenLists(1);
	glNewList(EarthAtm, GL_COMPILE);
	gluSphere(sphere, radius, 1000, 1000);
	glEndList();
	gluDeleteQuadric(sphere);
	glPopMatrix();

	//-----------------
	//and whenever you want to render, call glCallList(mysphereID)
	//to kill the display list, glDeleteLists(mysphereID, 1);

}

double rand_a_b(int a, int b)
{
	return rand()/((double)RAND_MAX + 1)*(b-a) + a; 
}

void init_sky()
{
	int i;
	
    /* on cree un objet et on recupere son identifiant */
    Sky = glGenLists(1);
    if(Sky == 0) {
        fprintf(stderr, "erreur lors de la creation de la liste\n");
        exit(-1);
    }
    
	glNewList(Sky, GL_COMPILE);
	glBegin(GL_POINTS);
	glColor3f(1.0f,1.0f,1.0f);
	for(i = 0; i < nb_stars; i++) {
		glColor4f((float)Stars[i].color[0], (float)Stars[i].color[1], (float)Stars[i].color[2], (float)Stars[i].alpha);
		glVertex3d(Stars[i].x, Stars[i].y, Stars[i].z);
	}
	glEnd();
	glEndList();
}

void init_galaxies()
{
	int i;

    /* on cree un objet et on recupere son identifiant */
	Galaxy_Sky = glGenLists(1);
    if(Galaxy_Sky == 0) {
        fprintf(stderr, "[*] Erreur lors de la creation de la liste galaxies\n");
        exit(-1);
    }

	glNewList(Galaxy_Sky, GL_COMPILE);
	glBegin(GL_POINTS);
	glColor3f(1.0f,1.0f,1.0f);
	for(i = 0; i < nb_galaxies; i++) {
		glVertex4d(Galaxies[i].x, Galaxies[i].y, Galaxies[i].z, Galaxies[i].alpha);
	}
	glEnd();
	glEndList();

}

void finalize()
{
	glDeleteBuffers(1, &buf);
}

