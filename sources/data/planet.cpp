/*
 * planet.c
 *
 *  Created on: 15 avr. 2013
 *      Author: n0dev
 */

#include <GL/glew.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <FTGL/ftgl.h>

#include "planet.h"
#include "../include/gui.h"
#include "../textures/loadpng.h"
#include "../gameplay/observer.h"

Planet::Planet(string name, int radius, string t)
{
	// Store all values
	this->radius = radius;
	this->name = name;

	// Texture
	if (!(this->texture = loadPNGTexture(("planets/textures/" + t).c_str()))) {
		fprintf(stderr, "failed to load %s! aborting\n", t.c_str());
		exit(1);
	}

	// Create the list
	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricTexture(sphere, 1);

	//Making a display list
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	gluSphere(sphere, this->radius, 1000, 1000);
	glEndList();
	gluDeleteQuadric(sphere);

	// Misc
	loc = new FTPoint(0.0, 0.0);

	// Ground
	//glGenBuffers(1, &buf);
}

inline float deg2rad(float deg) {
	return deg * 3.1415 / 180.0;
}

void draw_disk(int inner_radius, int outter_radius, int slices) {
	int inc;
	float theta;

	glBegin(GL_QUAD_STRIP);
	for (inc = 0; inc <= slices; inc++) {
		theta = inc * 2.0 * M_PI / slices;
		glTexCoord2f(1.0, 0.0);
		glVertex3f(outter_radius * cos(theta), outter_radius * sin(theta), 0.0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(inner_radius * cos(theta), inner_radius * sin(theta), 0.0);
	}
	glEnd();
}

void Planet::add_rings(int inner_radius, int outer_radius, string t)
{
	is_ring = true;

	this->inner_radius = inner_radius;
	this->outer_radius = outer_radius;

	if (!(this->texture_ring = loadPNGTexture(t.c_str()))) {
		fprintf(stderr, "failed to load saturn! aborting\n");
		exit(1);
	}

	GLUquadricObj *ring = NULL;
	ring = gluNewQuadric();
	gluQuadricDrawStyle(ring, GLU_FILL);
	gluQuadricNormals(ring, GLU_SMOOTH);
	gluQuadricTexture(ring, 1);
	this->list_ring = glGenLists(2);
	glNewList(this->list_ring, GL_COMPILE);
	draw_disk(this->inner_radius, this->outer_radius, 800);
	glEndList();
	gluDeleteQuadric(ring);
}

void Planet::draw(void)
{
	glPushMatrix();
	glTranslated(x-spaceship->pos.x, y-spaceship->pos.y, z-spaceship->pos.z);

	glPushMatrix();
	glTranslated(0.0, 0.0, 1.5 * this->radius);
	glRasterPos2f(0, 0);
	font->Render((this->name).c_str(), -1, *loc);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glCallList(this->list);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	if (is_ring) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->texture_ring);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glCallList(this->list_ring);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

	glPopMatrix();
}

void Planet::draw_atm()
{
	if(this->Atm.Height > 0) {

		GLUquadricObj *sphere = NULL;
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricNormals(sphere, GLU_SMOOTH);
		gluQuadricTexture(sphere, 1);

		glPushMatrix();
		glTranslated(x-spaceship->pos.x, y-spaceship->pos.y, z-spaceship->pos.z);

		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		gluSphere(sphere, this->radius + this->Atm.Height, 1000, 1000);
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	}
}

void Planet::setOrbit(const EllipticalOrbit& orbit) {
	this->Orbit = orbit;

	/* Build the orbit */
	float x, y, z, r;
	int t;
	float num = 5400;
	GLUquadricObj *o = NULL;
	o = gluNewQuadric();
	gluQuadricNormals(o, GLU_SMOOTH);
	this->orb = glGenLists(2);
	glNewList(this->orb, GL_COMPILE);
	glBegin(GL_LINE_LOOP);
	for (t = 0; t <= int(num); t += 1) {
		r = deg2rad(360.0*t/num);
		x = this->Orbit.SemiMajorAxis * sin(r);
		y = this->Orbit.SemiMajorAxis / (sqrt(1.0 - this->Orbit.Eccentricity* this->Orbit.Eccentricity)) * cos(r);
		z = 0;
		glVertex3f(x, y, z);
	}
	glEnd();
	glEndList();
	gluDeleteQuadric(o);

	// Set the new position of the planet here
	this->x = this->Orbit.SemiMajorAxis;
}

void Planet::draw_orbit(void)
{
	glPushMatrix();
	glTranslated(-spaceship->pos.x, -spaceship->pos.y, -spaceship->pos.z);
	glCallList(this->orb);
	glPopMatrix();
}

void Planet::light(void)
{
	//GLfloat light_diffuse[] = {x-spaceship->pos.x+15e9, y-spaceship->pos.y, z-spaceship->pos.z, 1.0 };
	//glLightfv(GL_LIGHT0, GL_POSITION, light_diffuse);
}

void Planet::add_moon(void)
{

}

void Planet::set_atm_height(int height)
{
	this->Atm.Height = height;
}

void Planet::add_atmosphere(Atmosphere atm)
{
	this->Atm = atm;
}

void Planet::add_rotation(UniformRotation rot)
{
	Rotation = rot;
}
