#include <iostream>
#include <string.h>
#include <math.h>

#include "importdb.h"
#include "spectrum.h"

using namespace std;

int nb_stars;
int nb_galaxies;

starsdb Stars[200000];
starsdb Galaxies[20000000];

void importStarsDBHip() {

    /*   1-  6  I6    ---      HIP     Hipparcos identifier
     *  16- 28 F13.10 rad      RArad   Right Ascension in ICRS, Ep=1991.25
     *  30- 42 F13.10 rad      DErad   Declination in ICRS, Ep=1991.25
     *  44- 50  F7.2  mas      Plx     Parallax
     * 130-136  F7.4  mag      Hpmag   Hipparcos magnitude
     * 153-158  F6.3  mag      B-V     Colour index
     * 166-171  F6.3  mag      V-I     V-I colour index
     *
     *  364 nm for U, 442 nm for B, 540 nm for V
     *
     *  a star with (B-V) < 0 is bluer than Vega
     *  a star with (B-V) > 0 is redder than Vega
     */

    cout << "[+] Import hipparcos star catalog...";
    flush(cout);

    static const char filename[] = "./catalogs/hipparcos/hip2.dat";
    FILE *file = fopen(filename, "r");

    nb_stars = 0;

    if (file != NULL) {
        char line[450];

        char temp_id[7];
        char temp_ra[14];
        char temp_de[14];
        char temp_dist[7];
        char temp_mag[8];
        char temp_bv[7];
        char temp_vi[7];

        while (fgets(line, sizeof(line), file) != NULL) {
            memcpy(&temp_id, line, 6);
            memcpy(&temp_ra, &line[14], 14);
            memcpy(&temp_de, &line[28], 14);
            memcpy(&temp_dist, &line[42], 7);
            memcpy(&temp_mag, &line[128], 7);
            memcpy(&temp_bv, &line[151], 6);
            memcpy(&temp_vi, &line[164], 6);

            Stars[nb_stars].id = atoi(temp_id);
            Stars[nb_stars].x = 1000.0 / atof(temp_dist) * 3.0857 * 1e16 * cos(atof(temp_ra)) * cos(atof(temp_de));
            Stars[nb_stars].y = 1000.0 / atof(temp_dist) * 3.0857 * 1e16 * sin(atof(temp_ra)) * cos(atof(temp_de));
            Stars[nb_stars].z = 1000.0 / atof(temp_dist) * 3.0857 * 1e16 * sin(atof(temp_de));

            /* -4,6	Magnitude maximale de Venus1
             * -1,5	Etoile la plus brillante (Sirius)
             * ≈ +6,5	Étoile la plus faible visible à l'oeil nu
             * +12,6	Quasar le plus lumineux
             * +30	Objets les plus faibles observés (Hubble)
             */

            Stars[nb_stars].alpha = -(1.0 / 25) * atof(temp_mag) + 20.0 / 25.0;
            Stars[nb_stars].temp = 1000.0 + 5000.0 / (atof(temp_bv) + 0.5);
            computeTemp(Stars[nb_stars].temp, Stars[nb_stars].color);

            nb_stars += 1;
            /*if(nbStars > 50)
                exit(0);*/
        }

        fclose(file);
        cout << " done!" << endl;

    } else {
        perror(filename); /* why didn't the file open? */
    }
}


void import_galaxies() {

    /*   1-  6  I6    ---      HIP     Hipparcos identifier
     *  16- 28 F13.10 rad      RArad   Right Ascension in ICRS, Ep=1991.25
     *  30- 42 F13.10 rad      DErad   Declination in ICRS, Ep=1991.25
     *  44- 50  F7.2  mas      Plx     Parallax
     * 130-136  F7.4  mag      Hpmag   Hipparcos magnitude
     * 153-158  F6.3  mag      B-V     Colour index
     * 166-171  F6.3  mag      V-I     V-I colour index
     *
     *  364 nm for U, 442 nm for B, 540 nm for V
     *
     *  a star with (B-V) < 0 is bluer than Vega
     *  a star with (B-V) > 0 is redder than Vega
     */

    //static const char filename1[] = "./catalogs/galaxy/virgo/croton_etal.BVRIK.mini.binary/croton_etal.BVRIK.mini.binary";

    cout << "[+] Import croton galaxy catalog...";
    flush(cout);

    //      6.5757904     13.0860395     25.3381310

    //int fd;
    //fd = open(filename1, O_RDONLY);
    /*int f;
    if(read(fd,&f,sizeof(int))==sizeof(f))
        printf("%x\n", 6.5757904);
    else
        printf("oops\n");*/

    //fread(n, sizeof(float), 23, fp);  // read 10 ints
    //close(fd);

    //int filedesc = open("testfile.txt", O_RDONLY);
    //close(filedesc);
    //for(i = 0; i < 10; i++)
    //	printf("n[%d] == %g\n", i, n[i]);

    //exit(0);

    static const char filename[] = "./catalogs/galaxy/virgo/croton_etal.BVRIK.ascii/croton_etal.BVRIK.ascii";
    FILE *file = fopen(filename, "r");

    nb_galaxies = 0;

    if (file != NULL) {
        char line[450];
        double x, y, z;

        while (fgets(line, sizeof(line), file) != NULL && nb_galaxies < 20000000) {

            fscanf(file, "%lf %lf %lf", &x, &y, &z);
            Galaxies[nb_galaxies].x = 30856775814672000.0 * 1000000.0 * x - 5 * 1e24;
            Galaxies[nb_galaxies].y = 30856775814672000.0 * 1000000.0 * y - 5 * 1e24;
            Galaxies[nb_galaxies].z = 30856775814672000.0 * 1000000.0 * z - 5 * 1e24;
            Galaxies[nb_galaxies].alpha = 0.1;
            nb_galaxies += 1;
        }

        fclose(file);
        cout << " done!" << endl;

    } else {
        perror(filename); /* why didn't the file open? */
    }
}


void importStarsDB() {
    static const char filename[] = "./catalogs/catalog.dat";
    FILE *file = fopen(filename, "r");

    nb_stars = 0;

    if (file != NULL) {
        char line[256]; /* or other suitable maximum line size */
        char temp_id[4];
        char temp_glon[6];
        char temp_glat[6];
        char temp_dist[5];
        char temp_mag[5];

        while (fgets(line, sizeof(line), file) != NULL) {
            memcpy(&temp_id, line, 4);
            memcpy(&temp_glon, &line[87], 6);
            memcpy(&temp_glat, &line[94], 6);
            memcpy(&temp_dist, &line[154], 5);
            memcpy(&temp_mag, &line[171], 5);
            Stars[nb_stars].id = atoi(temp_id);
            Stars[nb_stars].x =
                    atof(temp_dist) * 300000000.0 * 3600.0 * 24.0 * 365.0 * cos(M_PI * atof(temp_glon) / 180) *
                    cos(M_PI * atof(temp_glat) / 180);
            Stars[nb_stars].y =
                    atof(temp_dist) * 300000000.0 * 3600.0 * 24.0 * 365.0 * sin(M_PI * atof(temp_glon) / 180) *
                    cos(M_PI * atof(temp_glat) / 180);
            Stars[nb_stars].z =
                    atof(temp_dist) * 300000000.0 * 3600.0 * 24.0 * 365.0 * sin(M_PI * atof(temp_glat) / 180);
            Stars[nb_stars].alpha = atof(temp_mag);
            nb_stars += 1;
        }
        printf("Stars DB imported!\n");
        fclose(file);
    } else {
        perror(filename); /* why didn't the file open? */
    }
}
