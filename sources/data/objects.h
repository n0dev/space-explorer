#define N_VERTS 3
#define P_SIZE 3
#define C_SIZE 3

/* texture Id */
extern GLuint texEarth;
extern GLuint texEarthN;

/* Stars and galaxies */
extern GLuint Sky;
extern GLuint Galaxy_Sky;
extern GLuint buf;

extern GLuint EarthGd;
extern GLuint EarthAtm;

void dessineRectangle(double largeur,double hauteur);

void initEarthGd(double radius, double px, double py, double pz, GLuint texture);

void initEarthAtm(double radius, double px, double py, double pz);

void Init_Cube();

void init_sky();

void init_galaxies();
