#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "tgaimage.h"
using namespace std;


typedef float Vec3[3];
typedef float Vec2[2];

// dimenzije slike
const int width  = 512;
const int height = 512;

// definiramo boje
const TGAColor green = TGAColor(0, 255, 0, 1);
const TGAColor red   = TGAColor(255, 0, 0, 1);
const TGAColor blue  = TGAColor(0, 0, 255, 1);


float rubnafunk(const Vec3 &a, const Vec3 &b, const Vec3 &c) {
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}

//funkcija iscrtava trokut kroz zadane pixele
void draw_triangle(float x0, float y0,float z0, float x1, float y1,float z1, float x2, float y2,float z2 ,TGAImage &image,TGAColor color)
{
    Vec3 a = {x0,y0,z0};
    Vec3 b = {x1,y1,z1};
    Vec3 c = {x2, y2,z2};

    // dijeli koordinate sa z
    a[0]/=a[2]; a[1]/=a[2];
    b[0]/=b[2]; b[1]/=b[2];
    c[0]/=c[2]; c[1]/=c[2];

    //tranformacija
    a[0] = (1 + a[0]) * 0.5 * width; a[1]= (1 + a[1]) * 0.5 * height;
    b[0] = (1 + b[0]) * 0.5 * width; b[1]= (1 + b[1]) * 0.5 * height;
    c[0] = (1 + c[0]) * 0.5 * width; c[1]= (1 + c[1]) * 0.5 * height;

    // 1/z
    a[2] = 1 /a[2];  b[2]= 1 /b[2]; c[2] = 1 /c[2];

    for (int j =0; j<height ; j++){
        for (int i= 0;i<width;i++){
            Vec3 p = {((float)i+0.5f),(height-(float)j+0.5f),0};
            float alfa = rubnafunk(c,b,p);
            float beta =rubnafunk(a,c,p);
            float gama = rubnafunk(b,a,p);
            if((alfa >= 0 && beta >= 0 && gama >= 0) ) {
                image.set(j,i,color);

            }
        }
    }
}

void draw_triangle_tex(float x0, float y0, float z0, float u0, float v0, float x1, float y1, float z1, float u1, float v1, float x2, float y2, float z2, 
                       float u2, float v2, const char* file_path, TGAImage& image) {
    // Učitava teksturu s file patha
    TGAImage texture;
    texture.read_tga_file(file_path);

    Vec3 a = {x0, y0, z0};
    Vec3 b = {x1, y1, z1};
    Vec3 c = {x2, y2, z2};
    Vec2 uv_a = {u0, v0};
    Vec2 uv_b = {u1, v1};
    Vec2 uv_c = {u2, v2};

    // Dijeli koordinate sa z
    a[0] /= a[2]; a[1] /= a[2];
    b[0] /= b[2]; b[1] /= b[2];
    c[0] /= c[2]; c[1] /= c[2];

    // transformacija koordinata
    a[0] = (1 + a[0]) * 0.5 * width; a[1] = (1 + a[1]) * 0.5 * height;
    b[0] = (1 + b[0]) * 0.5 * width; b[1] = (1 + b[1]) * 0.5 * height;
    c[0] = (1 + c[0]) * 0.5 * width; c[1] = (1 + c[1]) * 0.5 * height;

    // inverz od 1/z
    a[2] = 1 / a[2]; b[2] = 1 / b[2]; c[2] = 1 / c[2];

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Vec3 p = {(float)i + 0.5f, height - (float)j + 0.5f, 0};
            float alpha = rubnafunk(c, b, p);
            float beta = rubnafunk(a, c, p);
            float gama = rubnafunk(b, a, p);
            if (alpha >= 0 && beta >= 0 && gama >= 0) {
                // Interpolira UV koordinate pomoću baricentričnih koordinata
                float w = alpha / (alpha + beta + gama);
                float u = u0 * (1 - w) + u1 * w;
                float v = v0 * (1 - w) + v1 * w;
                // Trazi boju u teksturi koristeći interpolirane UV koordinate
                TGAColor color = texture.get(u, v);
                // Postavlja boju pixela na slici
                image.set(i, j, color);
            }
        }
    }
}

void draw_triangle_tex_corrected(float x0, float y0, float z0, float u0, float v0,
                                 float x1, float y1, float z1, float u1, float v1,
                                 float x2, float y2, float z2, float u2, float v2,
                                 const char* file_path, TGAImage& image) {
    // Učitava teksturu s file patha
    TGAImage texture;
    texture.read_tga_file(file_path);

    Vec3 a = {x0, y0, z0};
    Vec3 b = {x1, y1, z1};
    Vec3 c = {x2, y2, z2};
    Vec2 uv_a = {u0, v0};
    Vec2 uv_b = {u1, v1};
    Vec2 uv_c = {u2, v2};

    // Dijeli koordinate sa z
    a[0] /= a[2];
    a[1] /= a[2];
    b[0] /= b[2];
    b[1] /= b[2];
    c[0] /= c[2];
    c[1] /= c[2];

    // transformacija koordinata
    a[0] = (1 + a[0]) * 0.5 * width;
    a[1] = (1 + a[1]) * 0.5 * height;
    b[0] = (1 + b[0]) * 0.5 * width;
    b[1] = (1 + b[1]) * 0.5 * height;
    c[0] = (1 + c[0]) * 0.5 * width;
    c[1] = (1 + c[1]) * 0.5 * height;

    // inverz 1/z
    a[2] = 1 / a[2];
    b[2] = 1 / b[2];
    c[2] = 1 / c[2];

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Vec3 p = {(float) i + 0.5f, height - (float) j + 0.5f, 0};
            float alpha = rubnafunk(c, b, p);
            float beta = rubnafunk(a, c, p);
            float gama = rubnafunk(b, a, p);
            if (alpha >= 0 && beta >= 0 && gama >= 0) {
                // Interpolira UV koordinate pomoću baricentričnih koordinata
                float w = alpha / (alpha + beta + gama);
                float u = u0 * (1 - w) + u1 * w;
                float v = v0 * (1 - w) + v1 * w;
                // Trazi boju u teksturi koristeći interpolirane UV koordinate
                TGAColor color = texture.get(u, v);
                // Postavlja boju pixela na slici
                image.set(i, j, color);
            }
        }
    }
}

int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);

    // nacrtaj trokute
    draw_triangle(-48, -10,  82, 29, -15,  44, 13,  34, 114,image, blue);
    //raw_triangle_tex(-48, -10,  82, 29, -15,  44, 13,  34, 114,0,0,0,1,1,0,image,"image1.tga");
    //draw_triangle_tex_corrected(-48, -10,  82, 29, -15,  44, 13,  34, 114,0,0,0,1,1,0,image,"image1.tga");
    //raw_triangle_tex(-48, -10,  82, 29, -15,  44, 13,  34, 114,0,0,0,1,1,0,image,"image2.tga");
    //draw_triangle_tex_corrected(-48, -10,  82, 29, -15,  44, 13,  34, 114,0,0,0,1,1,0,image,"image2.tga");
    // spremi sliku
    image.flip_vertically();
    image.write_tga_file("triangle.tga");
}
