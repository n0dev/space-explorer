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
    double OrbitalSpeed = 0.0;
};

class Planet {
public:
    Planet(std::string json_file);

    // Configure the display
    void displayOrbit(bool);

    // Display of the planet
    void draw();

    void lookAt();

private:
    std::string m_name = "";
    double m_radius = 0.0;
    double m_positionX = 0.0;
    double m_positionY = 0.0;
    double m_positionZ = 0.0;
    GLfloat m_axialTilt = 0.0;

    // Ground attributes
    GLuint list;
    GLuint m_ground_texture;

    // Orbital attributes
    bool m_displayOrbit = true;

    void createOrbit();

    EllipticalOrbit m_orbit;
    GLuint m_orbit_list;

    // Rings
    void createRings();

    bool m_is_ring = false;
    double m_inner_radius = 0;
    double m_outer_radius = 0;
    GLuint m_ring_list = 0;
    GLuint m_ring_texture = 0;

    // Misc
    FTPoint *loc;

    // Lights
    GLShader *sunLight;
    GLint idGroundMap;
    GLint idSunPosition;
};

// TODO: Singleton pool
extern Planet *mercury;
extern Planet *venus;
extern Planet *earth;
extern Planet *mars;
extern Planet *jupiter;
extern Planet *saturn;
extern Planet *uranus;
extern Planet *neptune;
extern Planet *pluto;

static void InitPlanet() {
    mercury = new Planet("mercury.json");
    venus = new Planet("venus.json");
    earth = new Planet("earth.json");
    mars = new Planet("mars.json");
    jupiter = new Planet("jupiter.json");
    saturn = new Planet("saturn.json");
    uranus = new Planet("uranus.json");
    neptune = new Planet("neptune.json");
    pluto = new Planet("pluto.json");
};

#endif /* PLANET_H_ */
