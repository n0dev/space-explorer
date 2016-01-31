#ifndef PLANET_H_
#define PLANET_H_

#include <iostream>

#include <FTGL/ftgl.h>

#include "../../include/glsl.h"

struct EllipticalOrbit {
    double Aphelion;       // farthest point from the sun
    double Perihelion;     // nearest point from the sun
	double SemiMajorAxis;
	double Eccentricity;
	double Inclination = 0.0;
    double ArgumentOfPeriapsis = 0.0;
	double LongitudeOfAscendingNode = 0.0;
	double MeanAnomaly;
    double OrbitalPeriod;
    double OrbitalSpeed;
};

class Planet
{
public:
    Planet(std::string json_file);

    /// Configuration of the planet
	void setRings(int inner_radius, int outer_radius, std::string t);

    /// Display of the planet
    void draw();
	void drawOrbit();

private:
	std::string m_name = "";
	double m_radius = 0.0;
	double m_positionX = 0.0;
    double m_positionY = 0.0;
    double m_positionZ = 0.0;
	GLfloat m_axialTilt = 0.0;

    // Ground attributes
	GLuint list;
	GLuint m_groundTexture;

    // Orbital attributes
    void createOrbit();
    EllipticalOrbit m_orbit;
	GLuint m_orbit_list;

	// Rings
	bool is_ring = false;
	int outer_radius = 0, inner_radius = 0;
	GLuint list_ring = 0;
	GLuint texture_ring = 0;

	// Misc
	FTPoint *loc;

	// Lights
	GLShader *sunLight;
	GLint idGroundMap;
	GLint idSunPosition;
};

#endif /* PLANET_H_ */
