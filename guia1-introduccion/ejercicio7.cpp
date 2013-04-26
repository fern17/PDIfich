#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
using namespace cimg_library;   //Necesario

//@ Genera una imagen de degradado de 256x256
CImg<unsigned char> degradado() {
    CImg<unsigned char> img(256,256);

    cimg_forXY(img,x,y) {
        img(x,y) = x;
    }
    return img;
}


//@ Transforma una imagen uchar a bool con la tecnica de halftoning (dithering)
CImg<bool> halftoning(CImg<unsigned char> img_original) {
    //Extiende el rango de la imagen de entrada
    img_original.normalize(0,255);

    // img_original.quantize(10); img_original.normalize(0,9);
    unsigned int h = img_original.height();
    unsigned int w = img_original.width();

    CImg<bool> img_resultado(w*3, h*3);

    //Factor interpola los niveles de gris
    float factor = 256.0/10.0;

    //Patron de grises para el halftoning
    bool patron_grises[10][3][3] = {
                        {{1,1,1},{1,1,1},{1,1,1}},
                        {{1,0,1},{1,1,1},{1,1,1}},
                        {{1,0,1},{1,1,1},{1,1,0}},
                        {{0,0,1},{1,1,1},{1,1,0}},
                        {{0,0,1},{1,1,1},{0,1,0}},
                        {{0,0,0},{1,1,1},{0,1,0}},
                        {{0,0,0},{1,1,0},{0,1,0}},
                        {{0,0,0},{1,1,0},{0,0,0}},
                        {{0,0,0},{0,1,0},{0,0,0}},
                        {{0,0,0},{0,0,0},{0,0,0}}
                     };

    cimg_forXY(img_original,x,y) {

        //Aplicamos el factor e invertimos porque las bases estan al reves (1 significa negro y no blanco)
        unsigned int current_level = 9 - floor(img_original(x,y) / factor);
        
        //Estos son mapeos a cada submatriz de 3x3
        img_resultado(3*x, 3*y) = patron_grises[current_level][0][0];
        img_resultado(3*x+1, 3*y) = patron_grises[current_level][0][1];
        img_resultado(3*x+2, 3*y) = patron_grises[current_level][0][2];

        img_resultado(3*x, 3*y+1) = patron_grises[current_level][1][0];
        img_resultado(3*x+1, 3*y+1) = patron_grises[current_level][1][1];
        img_resultado(3*x+2, 3*y+1) = patron_grises[current_level][1][2];

        img_resultado(3*x, 3*y+2) = patron_grises[current_level][2][0];
        img_resultado(3*x+1, 3*y+2) = patron_grises[current_level][2][1];
        img_resultado(3*x+2, 3*y+2) = patron_grises[current_level][2][2];
        
    }
    return img_resultado; 
}


int main(int argc, char *argv[]) {
   
    //@ Realiza la representacion de imagenes en escala de grises con halftoning de imagenes binarias
    cimg_usage("Halftoning");

    const char* input = cimg_option("-i", "../images/huang2.jpg", "Input Image File");

    CImg<unsigned char> img_original(input);
    CImg<bool> img_resultado = halftoning(img_original);

    CImg<unsigned char> img_degradado = degradado();
    CImg<bool> resultado_degradado = halftoning(img_degradado);

    (img_original, img_resultado.get_normalize(0,255), img_degradado, resultado_degradado.get_normalize(0,255)).display("Original y procesa con halftoning");
    
    return 0;
}
