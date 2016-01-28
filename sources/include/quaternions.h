#ifndef QUATERNIONS_H
#define QUATERNIONS_H

typedef struct {
	double qw;
	double qx;
	double qy;
	double qz;
} Quat;

typedef struct {
	double x;
	double y;
	double z;
} POSITION;

typedef struct {
	double x;
	double y;
	double z;
} VEC3;

typedef double Mat9[9];
typedef double Mat16[16];

void quatConjugate(Quat& qr, const Quat& qa);

double quatMagnitude(const Quat& q);

void quatMultiply(Quat& qr, const Quat& qa, const Quat& qb);

void quatNormalise(Quat& q);

void quatToMatrix(Quat *q, Mat16 Mat);

#endif
