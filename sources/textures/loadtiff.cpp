#include <tiffio.h>

unsigned char *loadTIFF(char *name) {
    unsigned char *tex = NULL;
    tstrip_t strip = 0;
    tsize_t off = 0;
    tsize_t n = 0;
    TIFF *tf = TIFFOpen(name, "r");
    while ((n = TIFFReadEncodedStrip(tf, strip, tex + off, (tsize_t) -1)) > 0) {
        strip += 1;
        off += n;
    };
    TIFFClose(tf);
    return (tex);
}
