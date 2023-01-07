#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "tgaimage.h"
using namespace std;

// dimenzije slike
const int width  = 512;
const int height = 512;

// definiramo boje
const TGAColor green = TGAColor(0, 255, 0, 1);
const TGAColor red   = TGAColor(255, 0, 0, 1);
const TGAColor blue  = TGAColor(0, 0, 255, 1);

void draw_triangle_2d(TGAImage& image, int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color) {
    // Poredaj vrhove uzlaznim redoslijedom y koordinate
    if (y0 > y1) {
        swap(y0, y1);
        swap(x0, x1);
    }
    if (y1 > y2) {
        swap(y2, y1);
        swap(x2, x1);
    }
    if (y0 > y1) {
        swap(y0, y1);
        swap(x0, x1);
    }

    // racuna nagibe dviju stranica trokuta
    float slope1 = (x1 - x0) / (float)(y1 - y0);
    float slope2 = (x2 - x0) / (float)(y2 - y0);

    // iteriramo kroz redove
    for (int y = y0; y <= y1; y++) {
        // racuna x koordinate dvaju rubova trokuta za ovaj redak
        int xl = x0 + slope1 * (y - y0);
        int xr = x0 + slope2 * (y - y0);

        // popunjavamo piksele između dva ruba bojom
        for (int x = xl; x <= xr; x++) {
            image.set(x, y, color);
        }
    }

    // racuna nagibe dviju stranica trokuta
    slope1 = (x2 - x1) / (float)(y2 - y1);
    slope2 = (x2 - x0) / (float)(y2 - y0);

    // iteriramo kroz redove
    for (int y = y1; y <= y2; y++) {
        // racuna x koordinate dvaju rubova trokuta za ovaj redak
        int xl = x1 + slope1 * (y - y1);
        int xr = x0 + slope2 * (y - y0);

        // popunjavamo piksele između dva ruba bojom
        for (int x = xl; x <= xr; x++) {
            image.set(x, y, color);
        }
    }
}

void draw_triangle_2d_gouraud(TGAImage& image, int x0, int y0, int x1, int y1, int x2, int y2,
                              TGAColor c0, TGAColor c1, TGAColor c2) {
    // poredaj vrhove uzlaznim redoslijedom y koordinate
    if (y0 > y1) {
        swap(y0, y1);
        swap(x0, x1);
        swap(c0, c1);
    }
    if (y1 > y2) {
        swap(y2, y1);
        swap(x2, x1);
        swap(c2, c1);
    }
    if (y0 > y1) {
        swap(y0, y1);
        swap(x0, x1);
        swap(c0, c1);
    }

    // racuna nagibe dviju stranica trokuta
    float slope1 = (x1 - x0) / (float)(y1 - y0);
    float slope2 = (x2 - x0) / (float)(y2 - y0);

    // interpoliramo boje duzinom prve polovice trokuta
    for (int y = y0; y <= y1; y++) {
        // racuna x koordinate dvaju rubova trokuta za ovaj redak
        int xl = x0 + slope1 * (y - y0);
        int xr = x0 + slope2 * (y - y0);

        // interpoliramo boje između dva ruba
        float s = (y - y0) / (float)(y1 - y0);
        TGAColor c = c0 * (1 - s) + c1 * s;

        // ispunjava piksele između dva ruba interpoliranom bojom
        for (int x = xl; x <= xr; x++) {
            image.set(x, y, c);
        }
    }

    // racuna nagibe dviju stranica trokuta
    slope1 = (x2 - x1) / (float)(y2 - y1);
    slope2 = (x2 - x0) / (float)(y2 - y0);

    // interpoliramo boje duzinom druge polovice trokuta
    for (int y = y1; y <= y2; y++) {
        // racuna x koordinate dvaju rubova trokuta za ovaj redak
        int xl = x1 + slope1 * (y - y1);
        int xr = x0 + slope2 * (y - y0);

        // interpoliramo boje između dva ruba
        float s = (y - y1) / (float)(y2 - y1);
        TGAColor c = c1 * (1 - s) + c2 * s;

        // ispunjava piksele između dva ruba interpoliranom bojom
        for (int x = xl; x <= xr; x++) {
            image.set(x, y, c);
        }
    }
}

int main() {
    // Napravi 512x512 sliku
    TGAImage image(width, height, TGAImage::RGB);

    // Nacrtaj zeleni trokut
    draw_triangle_2d(image, 10, 10, 50, 50, 100, 10, green);

    // Nacrtaj crveni trokut
    draw_triangle_2d(image, 200, 200, 250, 250, 300, 200, red);

    // nacrtaj crveni trokut s Gouraurd sjenčanjem
    draw_triangle_2d_gouraud(image, 200, 200, 250, 250, 300, 200, red, blue, green);

    // spremi sliku
    image.flip_vertically();
    image.write_tga_file("triangle.tga");
}
