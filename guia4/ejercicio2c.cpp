#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/PDI_functions.h"

using namespace cimg_library;   //Necesario


int main(int argc, char *argv[]) {
    //@ Binariza una imagen en base a un umbral, falso color en imagen binaria, truco para mostrar imagenes binarias en el display

    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const unsigned int umbral = cimg_option("-u", 150, "Input Image File");
    const char* paleta = cimg_option("-p", "../paletas/paleta_binaria_rb.pal", "Input Image File");

    //Declaramos imagenes a trabajar
    CImg<unsigned char> img_entrada(input);
    CImg<bool> img_binaria(img_entrada.width(), img_entrada.height(), img_entrada.depth(), 1);
    CImg<double> output(img_entrada.width(), img_entrada.height(), img_entrada.depth(), 3) ;

    //Creamos la imagen binaria en base al umbral
    cimg_forXY(img_entrada, x, y) {
        if (img_entrada(x,y) > umbral)
            img_binaria(x,y) = true;
        else
            img_binaria(x,y) = false;
    }

    CImg<double> mapeo;
    mapeo = cargar_paleta(paleta);
    
    //Recorre x y, y c CANALES
   cimg_forXYC(output, x, y, c) {
        if (img_binaria(x, y))
            output(x, y, c) = mapeo(0, 1, c);
        else
            output(x, y, c) = mapeo(0, 0, c);
    }
    img_binaria.display();
    //MOstramos
    CImgList<double> lista;

    lista.assign(img_entrada, output.normalize(0,255) );

    lista.display();



    return 0;
}
