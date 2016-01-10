typedef struct {
	double qw;
	double qx;
	double qy;
	double qz;
} QUAT;

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

typedef double MAT9[9];
typedef double MAT16[16];

void quatConjugate(QUAT *qr, QUAT *qa);

double quatMagnitude(QUAT *qa);

void quatMultiply(QUAT *qr, QUAT *qa, QUAT *qb);

void quatNormalise(QUAT *q);

void quatToMatrix(QUAT *q, MAT16 Mat);
