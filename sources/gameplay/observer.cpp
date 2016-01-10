#include <math.h>
#include <iostream>
#include <GL/glu.h>

#include "observer.h"

using namespace std;

Observer *spaceship;

Observer::Observer()
{
	cout << "[+] Initialize spaceship...";
	flush(cout);

	quater.qw = 0.98;
	quater.qx = 0.00;
	quater.qy = 0.06;
	quater.qz = 0.15;
	pos.x = 1.433*1e12;
	pos.y = -1.58e8;
	pos.z = 1e8;
	rangespeed = 3;
	speed = 0;

	cout << " done!" << endl;
}

double Observer::get_distance()
{
	return sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
}

void Observer::update()
{
	pos.x += speed * matrix[0];
	pos.y += speed * matrix[4];
	pos.z += speed * matrix[8];

	gluLookAt(0.0, 0.0, 0.0,
			matrix[0], matrix[4], matrix[8],
			matrix[2], matrix[6], matrix[10]);
}
