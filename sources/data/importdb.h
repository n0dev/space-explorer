typedef double STARCOLOR[3];

typedef struct {

    int id;
    double x;
    double y;
    double z;
    double alpha;
    double temp;
    STARCOLOR color;

} starsdb;

extern int nb_stars;
extern int nb_galaxies;

extern starsdb Stars[200000];
extern starsdb Galaxies[20000000];

void importStarsDB();

void importStarsDBHip();

void import_galaxies();
