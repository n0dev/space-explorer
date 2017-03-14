#include <math.h>
#include <iostream>
#include <GL/glu.h>

#include "observer.h"

Observer *spaceship;

Observer::Observer() {
    std::cout << "[+] Initialize spaceship...";

    quater.qw = 0.98;
    quater.qx = 0.00;
    quater.qy = 0.06;
    quater.qz = 0.15;
    pos.x = 1.433 * 1e12;
    pos.y = -1.58e8;
    pos.z = 1e8;
    rangespeed = 3;
    speed = 0;

    std::cout << " done!" << std::endl;
}

void Observer::lookAt(double x, double y, double z) {
    double angle = 0.05;
    double sin_a = sin(angle / 2);
    double cos_a = cos(angle / 2);

    quater.qx = 0 * sin_a;
    quater.qy = 0 * sin_a;
    quater.qz = 1.0 * sin_a;
    quater.qw = cos_a;

    quaternion_normalise(quater);
    quaternion_to_matrix(&quater, matrix);
    /*double norm = sqrt(x*x + y*y + z*z);
    matrix[0] = x / norm;
    matrix[4] = y / norm;
    matrix[8] = z / norm;
    matrixToQuaternion(matrix, quater);
    quatNormalise(quater);
    quatToMatrix(&quater, matrix);*/
}

double Observer::get_distance() {
    return sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
}

void Observer::update() {
    pos.x += speed * matrix[0];
    pos.y += speed * matrix[4];
    pos.z += speed * matrix[8];

    /*euler_from_m4(euler, matrix);

    std::cout << "Euler: " << euler.x << "; " << euler.y << "; " << euler.z << std::endl;*/

    gluLookAt(0.0, 0.0, 0.0,
              matrix[0], matrix[4], matrix[8],
              matrix[2], matrix[6], matrix[10]);
}
