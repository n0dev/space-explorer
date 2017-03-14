#ifndef QUATERNIONS_H
#define QUATERNIONS_H

struct Vec3 {
    double x;
    double y;
    double z;
};

struct Quat {
    double qw;
    double qx;
    double qy;
    double qz;
};

typedef double Mat3[9];
typedef double Mat4[16];

// Convert euler angles to a 3x3 matrix
void m3_from_euler(Mat3 &, const Vec3 &);

// Convert euler angles to a 4x4 matrix
void m4_from_euler(Mat4 &, const Vec3 &);

// Convert a 3x3 matrix to euler angles
void euler_from_m3(Vec3 &, const Mat3 &);

// Convert a 4x4 matrix to euler angles
void euler_from_m4(Vec3 &, const Mat4 &);

//----------

// Calculate the conjugate (or inverse) of the quaternion
void quaternion_conjugate(Quat &qr, const Quat &qa);

// Normalize a quaternion
void quaternion_normalise(Quat &q);

// Calculate the magnitude of a quaternion
double quaternion_magnitude(const Quat &q);

// Create a quaternion from an axis and angle
void quaternion_from_axis_angle(Quat &qr, const Vec3 &, const double &);

// Create a quaternion from spherical coordinates
void quaternion_from_spherical(Quat &qr, const double &latitude, const double &longitude, const double &angle);

// Create an axis and angle from a quaternion
void quaternion_to_axis_angle(Vec3 &, double &, Quat &);

// Create a quaternion to spherical coordinates
void quaternion_to_spherical(double &latitude, double &longitude, double &angle, Quat &q);


void quatMultiply(Quat &qr, const Quat &qa, const Quat &qb);


void matrixToQuaternion(Mat4 &mr, Quat &q);

void quaternion_to_matrix(Quat *q, Mat4 mr);

#endif
