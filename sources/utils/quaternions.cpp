#include <math.h>

#include "../include/quaternions.h"

void quatConjugate(QUAT *qr, QUAT *qa)
{
    qr->qw =  qa->qw;
    qr->qx = -qa->qx;
    qr->qy = -qa->qy;
    qr->qz = -qa->qz;
}

double quatMagnitude(QUAT *qa) {
	return sqrt(qa->qw * qa->qw + qa->qx * qa->qx + qa->qy * qa->qy + qa->qz * qa->qz);
}

void quatMultiply(QUAT *qr, QUAT *qa, QUAT *qb) {
	qr->qw = qa->qw*qb->qw - qa->qx*qb->qx - qa->qy*qb->qy - qa->qz*qb->qz; /* (Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2) */
	qr->qx = qa->qw*qb->qx + qa->qx*qb->qw + qa->qy*qb->qz - qa->qz*qb->qy; /* (Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2) */
	qr->qy = qa->qw*qb->qy - qa->qx*qb->qz + qa->qy*qb->qw + qa->qz*qb->qx; /* (Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2) */
	qr->qz = qa->qw*qb->qz + qa->qx*qb->qy - qa->qy*qb->qx + qa->qz*qb->qw; /* (Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2) */
}

void quatNormalise(QUAT *q) {
	double norm = quatMagnitude(q);
	q->qw = q->qw/norm;
	q->qx = q->qx/norm;
	q->qy = q->qy/norm;
	q->qz = q->qz/norm;
}

void quatToMatrix(QUAT *q, MAT16 Mat) {

	static double xx;
	static double xy;
	static double xz;
	static double xw;
	static double yy;
	static double yz;
	static double yw;
	static double zz;
	static double zw;

	xx = q->qx * q->qx;
	xy = q->qx * q->qy;
	xz = q->qx * q->qz;
	xw = q->qx * q->qw;
	yy = q->qy * q->qy;
	yz = q->qy * q->qz;
	yw = q->qy * q->qw;
	zz = q->qz * q->qz;
	zw = q->qz * q->qw;

	Mat[0] = 1.0 - 2.0 * (yy + zz);
	Mat[1] = 2.0 * (xy - zw);
	Mat[2] = 2.0 * (xz + yw);
	Mat[4] = 2.0 * (xy + zw);
	Mat[5] = 1.0 - 2.0 * (xx + zz);
	Mat[6] = 2.0 * (yz - xw);
	Mat[8] = 2.0 * (xz - yw);
	Mat[9] = 2.0 * (yz + xw);
	Mat[10] = 1.0 - 2.0 * (xx + yy);
}
