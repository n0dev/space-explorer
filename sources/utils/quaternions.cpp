#include <math.h>

#include "../include/quaternions.h"

#define RADIANS 360/6.28

void m3_from_euler(Mat3& mat, const Vec3& vec)
{
    double a = cos(vec.x);
    double b = sin(vec.x);
    double c = cos(vec.y);
    double d = sin(vec.y);
    double e = cos(vec.z);
    double f = sin(vec.z);

    double ad = a * d;
    double bd = b * d;

    mat[0] = c * e;
    mat[1] = -c * f;
    mat[2] = -d;
    mat[4] = -bd * e + a * f;
    mat[5] =  bd * f + a * e;
    mat[6] = -b * c;
    mat[8] =  ad * e + b * f;
    mat[9] = -ad * f + b * e;
    mat[10] = a * c;
}

void m4_from_euler(Mat4& mat, const Vec3& vec)
{
    double a = cos(vec.x);
    double b = sin(vec.x);
    double c = cos(vec.y);
    double d = sin(vec.y);
    double e = cos(vec.z);
    double f = sin(vec.z);

    double ad = a * d;
    double bd = b * d;

    mat[0] = c * e;
    mat[1] = -c * f;
    mat[2] = -d;
    mat[4] = -bd * e + a * f;
    mat[5] =  bd * f + a * e;
    mat[6] = -b * c;
    mat[8] =  ad * e + b * f;
    mat[9] = -ad * f + b * e;
    mat[10] = a * c;

    mat[3] = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0;
    mat[15] = 1;
}

void euler_from_m3(Vec3& vec, const Mat3& mat)
{
    vec.y = -asin(mat[2]);
    double c = cos(vec.y);

    if (fabs(c) > 0.0005) { // Gimball lock?
        vec.x = atan2(-mat[6] / c, mat[10] / c);
        vec.z = atan2(-mat[1] / c, mat[0] / c);
    }
    else { // Gimball lock has occurred
        vec.x = 0;
        vec.z  = atan2(mat[4], mat[5]);
    }
}

void euler_from_m4(Vec3& vec, const Mat4& mat)
{
    vec.y = -asin(mat[2]);
    double c = cos(vec.y);
    //vec.y *= RADIANS;

    if (fabs(c) > 0.0005) { // Gimball lock?
        vec.x = atan2(-mat[6] / c, mat[10] / c) /** RADIANS*/;
        vec.z = atan2(-mat[1] / c, mat[0] / c) /** RADIANS*/;
    }
    else { // Gimball lock has occurred
        vec.x = 0;
        vec.z  = atan2(mat[4], mat[5]) /** RADIANS*/;
    }

    /*angle_x = clamp( angle_x, 0, 360 );
    angle_y = clamp( angle_y, 0, 360 );
    angle_z = clamp( angle_z, 0, 360 );*/
}

void quaternion_conjugate(Quat& qr, const Quat& qa)
{
    qr.qw =  qa.qw;
    qr.qx = -qa.qx;
    qr.qy = -qa.qy;
    qr.qz = -qa.qz;
}

double quaternion_magnitude(const Quat& q)
{
	return sqrt(q.qw * q.qw + q.qx * q.qx + q.qy * q.qy + q.qz * q.qz);
}

void quatMultiply(Quat& qr, const Quat& qa, const Quat& qb)
{
	qr.qw = qa.qw*qb.qw - qa.qx*qb.qx - qa.qy*qb.qy - qa.qz*qb.qz; /* (Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2) */
	qr.qx = qa.qw*qb.qx + qa.qx*qb.qw + qa.qy*qb.qz - qa.qz*qb.qy; /* (Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2) */
	qr.qy = qa.qw*qb.qy - qa.qx*qb.qz + qa.qy*qb.qw + qa.qz*qb.qx; /* (Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2) */
	qr.qz = qa.qw*qb.qz + qa.qx*qb.qy - qa.qy*qb.qx + qa.qz*qb.qw; /* (Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2) */
}

void quaternion_normalise(Quat& q)
{
	double norm = quaternion_magnitude(q);
	q.qw = q.qw/norm;
	q.qx = q.qx/norm;
	q.qy = q.qy/norm;
	q.qz = q.qz/norm;
}

void matrixToQuaternion(Mat4& mr, Quat& q) {
    double trace = 1.0 + mr[0] + mr[5] + mr[10];
    double s;
    if (trace > 0) {
        s = 0.5 / sqrt(trace);
        q.qw = 0.25 / s;
        q.qx = (mr[9] - mr[6]) * s;
        q.qy = (mr[2] - mr[8]) * s;
        q.qz = (mr[4] - mr[1]) * s;
    }
    else if (q.qx >= q.qy && q.qx >= q.qz) {
        s  = sqrt( 1.0 + mr[0] - mr[5] - mr[10] ) * 2;
        q.qx = 0.5 / s;
        q.qy = (mr[1] + mr[4] ) / s;
        q.qz = (mr[2] + mr[8] ) / s;
        q.qw = (mr[6] + mr[9] ) / s;
    }
    else if (q.qy >= q.qx && q.qy >= q.qz) {
        s  = sqrt( 1.0 + mr[5] - mr[0] - mr[10] ) * 2;
        q.qx = (mr[1] + mr[4] ) / s;
        q.qy = 0.5 / s;
        q.qz = (mr[6] + mr[9] ) / s;
        q.qw = (mr[2] + mr[8] ) / s;
    }
    else {
        s  = sqrt( 1.0 + mr[10] - mr[0] - mr[5] ) * 2;
        q.qx = (mr[2] + mr[8] ) / s;
        q.qy = (mr[6] + mr[9] ) / s;
        q.qz = 0.5 / s;
        q.qw = (mr[1] + mr[4] ) / s;
    }
}


void quaternion_to_matrix(Quat *q, Mat4 mr)
{
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

    mr[0] = 1.0 - 2.0 * (yy + zz);
    mr[1] = 2.0 * (xy - zw);
    mr[2] = 2.0 * (xz + yw);
    mr[4] = 2.0 * (xy + zw);
    mr[5] = 1.0 - 2.0 * (xx + zz);
    mr[6] = 2.0 * (yz - xw);
    mr[8] = 2.0 * (xz - yw);
    mr[9] = 2.0 * (yz + xw);
    mr[10] = 1.0 - 2.0 * (xx + yy);

    mr[3]  = mr[7] = mr[11] = mr[12] = mr[13] = mr[14] = 0;
    mr[15] = 1;
}

void quaternion_from_axis_angle(Quat &q, const Vec3 &axis, const double &angle) {
    double sin_a = sin(angle / 2);
    double cos_a = cos(angle / 2);

    q.qx = axis.x * sin_a;
    q.qy = axis.y * sin_a;
    q.qz = axis.z * sin_a;
    q.qw = cos_a;

    quaternion_normalise(q);
}

void quaternion_to_axis_angle(Vec3 &axis, double &angle, Quat &q) {
    quaternion_normalise(q);

    double cos_angle = q.qw;
    angle            = acos(cos_angle) * 2 * RADIANS;
    double sin_angle = sqrt(1.0 - cos_angle * cos_angle);

    if (fabs(sin_angle) < 0.0005)
        sin_angle = 1;

    axis.x = q.qx / sin_angle;
    axis.y = q.qy / sin_angle;
    axis.z = q.qz / sin_angle;
}

void quaternion_from_spherical(Quat &qr, const double &latitude, const double &longitude, const double &angle) {
    double sin_a    = sin(angle / 2 );
    double cos_a    = cos(angle / 2 );

    double sin_lat  = sin(latitude);
    double cos_lat  = cos(latitude);

    double sin_long = sin(longitude);
    double cos_long = cos(longitude);

    qr.qx = sin_a * cos_lat * sin_long;
    qr.qy = sin_a * sin_lat;
    qr.qz = sin_a * sin_lat * cos_long;
    qr.qw = cos_a;
}

void quaternion_to_spherical(double &latitude, double &longitude, double &angle, Quat &q) {
    double cos_angle  = q.qw;
    double sin_angle  = sqrt( 1.0 - cos_angle * cos_angle );
    angle             = acos( cos_angle ) * 2 * RADIANS;

    if (fabs(sin_angle) < 0.0005)
        sin_angle = 1;

    double tx = q.qx / sin_angle;
    double ty = q.qy / sin_angle;
    double tz = q.qz / sin_angle;

    latitude = -asin(ty);

    if (tx * tx + tz * tz < 0.0005)
        longitude = 0;
    else
        longitude = atan2(tx, tz) * RADIANS;

    if (longitude < 0)
        longitude += 360.0;
}
