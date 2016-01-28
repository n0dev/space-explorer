/*
 * planet.h
 *
 *  Created on: 15 avr. 2013
 *      Author: n0dev
 */

#ifndef PLANET_H_
#define PLANET_H_

#include <iostream>

#include <FTGL/ftgl.h>

#include "../include/glsl.h"

using namespace std;

typedef struct {
	float Period;
	float SemiMajorAxis;
	float Eccentricity;
	float Inclination;
	float AscendingNode;
	float LongOfPericenter;
	float MeanLongitude;
} EllipticalOrbit;

typedef struct {
	int Height; // meters 60
	int Lower[3]; // [ 0.8 0.8 0.5 ]
	float Upper[3]; //[ 0.6 0.6 0.6 ]
	float Sky[3]; //[ 0.8 0.8 0.5 ]
	GLuint CloudMap; // "venus.jpg"
	int CloudHeight; // 50
	int CloudSpeed;  //90
} Atmosphere;

typedef struct {
	float Period;
	float Inclination;
	float AscendingNode;
	float MeridianAngle;
} UniformRotation;

class Planet
{
 public:
	Planet(string name, int radius, string t);


	void add_rings(int inner_radius, int outer_radius, string t);
	void setOrbit(const EllipticalOrbit& orbit);
	void add_rotation(UniformRotation rot);
	void add_moon();
	void draw();
	void draw_orbit();

	// Atmosphere related methods
	void set_atm_height(int height);
	void add_atmosphere(Atmosphere atm);
	void draw_atm();

	// Light related methods
	void light();

	// Easy accessors
	double axial_tilt = 23.6;

 private:
	string name = "";
	int distance_from_sun = 0;
	int radius = 0;
	double x=0.0, y=0.0, z=0.0;
	GLuint list;
	GLuint texture;
	GLuint orb = 0;


	// Ground information
	//GLuint buf; // Our Vertex Buffer Object

	// Misc information
	UniformRotation Rotation;
	EllipticalOrbit Orbit;
	Atmosphere Atm;

	// Rings
	bool is_ring = false;
	int outer_radius = 0, inner_radius = 0;

	GLuint m_listAtm = 0;
	GLuint list_ring = 0;
	GLuint texture_ring = 0;

	// Misc
	FTPoint *loc;

	// Lights
	GLShader *sunLight;
	GLuint idGroundMap;
	GLuint idSunPosition;
};

#endif /* PLANET_H_ */
