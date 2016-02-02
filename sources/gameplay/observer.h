#ifndef OBSERVER_H_
#define OBSERVER_H_

#include "../include/quaternions.h"

class Observer
{
public:
	Observer();

	/* Position of the this is the position of the camera*/
	POSITION pos;
	Quat quater;

	/* Controls */

	/*void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);*/

	/* Speed
	 * Spaceship's speed can either be positive or negative and in m by sec.
	 * It is handle by the gameplay and technical char of the spaceship
	 */

	int rangespeed;
	double speed;

	// Display an object relative to the eye
	void lookAt(double x, double y, double z);

    //
	double get_distance();

	// Actions
	void update();

//private:
	Mat16 matrix;
};


extern Observer *spaceship;

#endif /* OBSERVER_H_ */