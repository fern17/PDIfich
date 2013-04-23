#include <CImg.h>               //include basico de CImg
#include <iostream>
using namespace cimg_library;   //Necesario


int main(int argc, char *argv[]) {
     //@ Lee una paleta de colores y convierte una imagen en gris en una imagen en falso color usando la paleta
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");
    
    const char* input = cimg_option("-i", "../images/patron.tif", "Input Image File");
    
    CImg<double> img(input), generada, img_hsi, generada_rgb, r, g, b, h, s, i, rr, gg, bb;

    img.get_FFT().display();
    return 0;
}
