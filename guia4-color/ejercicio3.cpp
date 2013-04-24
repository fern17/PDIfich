#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/PDI_functions.h"

using namespace cimg_library;   //Necesario


int main(int argc, char *argv[]) {
    //@ Tomar una imagen, realizar histograma, ver el rango de grises deseado y cambiar estos grises por colores para resaltado

    const char* _input = cimg_option("-i", "../images/rio.jpg", "Input Image File");
    const unsigned int _umbral = cimg_option("-u", 47, "Rango de gris max 0...umbral");


    //Declaramos imagenes a trabajar
    CImg<unsigned char> input(_input) ;
    CImg<double> output(input.width(), input.height(), input.depth(), 3, 0);

    
    //Creamos, Calculamos y mostramos el histograma
    CImg<unsigned char> histograma = input.get_histogram(256, 0 , 255);
    histograma.display_graph("Histograma de la Entrada", 3);

    //Vimos en el histograma que los negros del rio predominan en el rango 0..47 (ver imagen)
    cimg_forXY(output, x, y) {
        if (input(x,y) < _umbral) {
            output(x,y,0,0) = 255; //R
            output(x,y,0,1) = 255; //G
            output(x,y,0,2) = 0; //B
        } else {
            output(x,y,0,0) = input(x,y); //R
            output(x,y,0,1) = input(x,y); //G
            output(x,y,0,2) = input(x,y); //B

        }

    }

    //Display! 
    CImgList<double> lista;

    lista.assign(input, output );

    lista.display();



    return 0;
}
