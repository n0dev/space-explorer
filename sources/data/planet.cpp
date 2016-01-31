#include <math.h>
#include <fstream>

#include <FTGL/ftgl.h>
#include <GL/glew.h>


#include "rapidjson/document.h"
#include "../include/data/planet.h"
#include "../include/gui.h"
#include "../textures/loadpng.h"
#include "../gameplay/observer.h"

using namespace rapidjson;

const std::string g_planetPath = "planets/";
const std::string g_texturePath = "planets/textures/";

inline double deg2rad(double deg) {
    return deg * 3.1415 / 180.0;
}

Planet::Planet(std::string json_file)
{
    // Open and parse json file
    std::string json_path = g_planetPath;
    json_path += json_file;
    std::ifstream file(json_path);
    if (!file.good()) {
        std::cerr << "Cannot open " << json_file << std::endl;
        exit(1);
    }
    std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    Document document;
    document.Parse(json.c_str());

    // Store all mandatory values
    m_name = document["Name"].GetString();
    m_radius = document["Radius"].GetDouble();

    // Load the ground texture
    std::string texturePath = g_texturePath;
    texturePath += std::string(document["GroundTexture"].GetString());
    if (!(m_groundTexture = loadPNGTexture(texturePath.c_str()))) {
        std::cerr << "Failed to load " << texturePath << "! aborting" << std::endl;
        exit(1);
    }

    if (document.HasMember("AxialTilt")) {
        m_axialTilt = static_cast<GLfloat>(document["AxialTilt"].GetDouble());
    }

    // Load orbit information and creates it
    if (document.HasMember("Orbit")) {
        if (document["Orbit"].HasMember("Aphelion")) {
            m_orbit.Aphelion = document["Orbit"]["Aphelion"].GetDouble();
        }
        if (document["Orbit"].HasMember("Perihelion")) {
            m_orbit.Perihelion = document["Orbit"]["Perihelion"].GetDouble();
        }
        if (document["Orbit"].HasMember("SemiMajorAxis")) {
            m_orbit.SemiMajorAxis = document["Orbit"]["SemiMajorAxis"].GetDouble();
        }
        if (document["Orbit"].HasMember("Eccentricity")) {
            m_orbit.Eccentricity = document["Orbit"]["Eccentricity"].GetDouble();
        }
        if (document["Orbit"].HasMember("Inclination")) {
            m_orbit.Inclination = document["Orbit"]["Inclination"].GetDouble();
        }
        if (document["Orbit"].HasMember("ArgumentOfPeriapsis")) {
            m_orbit.ArgumentOfPeriapsis = document["Orbit"]["ArgumentOfPeriapsis"].GetDouble();
        }
        if (document["Orbit"].HasMember("LongitudeOfAscendingNode")) {
            m_orbit.LongitudeOfAscendingNode = document["Orbit"]["LongitudeOfAscendingNode"].GetDouble();
        }
        if (document["Orbit"].HasMember("MeanAnomaly")) {
            m_orbit.MeanAnomaly = document["Orbit"]["MeanAnomaly"].GetDouble();
        }
        if (document["Orbit"].HasMember("OrbitalPeriod")) {
            m_orbit.OrbitalPeriod = document["Orbit"]["OrbitalPeriod"].GetDouble();
        }
        if (document["Orbit"].HasMember("OrbitalSpeed")) {
            m_orbit.OrbitalSpeed = document["Orbit"]["OrbitalSpeed"].GetDouble();
        }
        createOrbit();
    }

    // Create the planet mesh
    GLUquadricObj *sphere = NULL;
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluQuadricTexture(sphere, 1);
    list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    gluSphere(sphere, m_radius, 1000, 1000);
    glEndList();
    gluDeleteQuadric(sphere);

    // Misc
    loc = new FTPoint(0.0, 0.0);

    // Lights
    sunLight = new GLShader("../sources/shaders/sunlight.vert", "../sources/shaders/sunlight.frag");
    idGroundMap = glGetUniformLocation(sunLight->program, "tex");
    idSunPosition = glGetUniformLocation(sunLight->program, "sunPosition");
}

void draw_disk(int inner_radius, int outer_radius, int slices) {
	double theta;

	glBegin(GL_QUAD_STRIP);
	for (int inc = 0; inc <= slices; inc++) {
		theta = inc * 2.0 * M_PI / slices;
		glTexCoord2f(1.0, 0.0);
		glVertex3f(static_cast<GLfloat>(outer_radius * cos(theta)), static_cast<GLfloat>(outer_radius * sin(theta)), 0.0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(static_cast<GLfloat>(inner_radius * cos(theta)), static_cast<GLfloat>(inner_radius * sin(theta)), 0.0);
	}
	glEnd();
}

void Planet::setRings(int inner_radius, int outer_radius, std::string t)
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
	glTranslated(m_positionX-spaceship->pos.x, m_positionY-spaceship->pos.y, m_positionZ-spaceship->pos.z);

    if (m_axialTilt != 0) {
        glRotatef(m_axialTilt, 1, 0, 0);
    }

	glPushMatrix();
	glTranslated(0.0, 0.0, 1.5 * m_radius);
	glRasterPos2f(0, 0);
	font->Render(m_name.c_str(), -1, *loc);
	glPopMatrix();

	sunLight->begin();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_groundTexture);
    glUniform1i(idGroundMap, 1);
	const GLfloat vecSunPosition[3] = {(GLfloat) -spaceship->pos.x, (GLfloat) -spaceship->pos.y, (GLfloat) -spaceship->pos.z};
    glUniform3fv(idSunPosition, 1, vecSunPosition);
	glCallList(this->list);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	sunLight->end();

	if (is_ring) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->texture_ring);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
		glCallList(this->list_ring);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
    }

	glPopMatrix();
}

void Planet::createOrbit()
{
    // Set information
    const int edges = 6000;

	// Build the orbit
    const GLdouble a = m_orbit.SemiMajorAxis;
    const GLdouble b = a * sqrt(1.0 - m_orbit.Eccentricity * m_orbit.Eccentricity);
	GLdouble x, y, z, r;
	GLUquadricObj *o = NULL;
	o = gluNewQuadric();
	gluQuadricNormals(o, GLU_SMOOTH);
    m_orbit_list = glGenLists(2);
	glNewList(m_orbit_list, GL_COMPILE);
	glBegin(GL_LINE_LOOP);

	for (int t = 0; t <= edges; t += 1) {
		r = deg2rad(360.0 * t / edges);
		x = a * cos(r);
		y = b * sin(r);
		z = 0;
		glVertex3d(x, y, z);
	}
	glEnd();
	glEndList();
	gluDeleteQuadric(o);

	// Set the new position of the planet here
	m_positionX = m_orbit.SemiMajorAxis;
}

void Planet::drawOrbit(void)
{
	glPushMatrix();
	glTranslated(-spaceship->pos.x, -spaceship->pos.y, -spaceship->pos.z);
    if (m_orbit.LongitudeOfAscendingNode != 0.0) {
        glRotated(m_orbit.LongitudeOfAscendingNode, 0, 0, 1);
    }
    if (m_orbit.ArgumentOfPeriapsis != 0.0) {
        glRotated(m_orbit.ArgumentOfPeriapsis, 0, 0, 1);
    }
    if (m_orbit.Inclination != 0.0) {
        glRotated(m_orbit.Inclination, 1, 0, 0);
    }
	glCallList(m_orbit_list);
	glPopMatrix();
}
