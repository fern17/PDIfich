#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
using namespace cimg_library;   //Necesario


//Según nuestra interpretacion:

// histograma 1 -> imagen C
// histograma 2 -> imagen A
// histograma 3 -> imagen E
// histograma 4 -> imagen B
// histograma 5 -> imagen D

//Estan correctos!!!


int main(int argc, char *argv[]) {
    //@ muestra histogramas de distintas imágenes (A,B C D E)

    const char* _inputA = "../images/imagenA.tif";
    const char* _inputB = "../images/imagenB.tif";
    const char* _inputC = "../images/imagenC.tif";
    const char* _inputD = "../images/imagenD.tif";
    const char* _inputE = "../images/imagenE.tif";

    CImg<unsigned char> imgA(_inputA),
                        imgB(_inputB),
                        imgC(_inputC),
                        imgD(_inputD),
                        imgE(_inputE);

    imgA.get_histogram(256).display_graph(0,3);
    imgB.get_histogram(256).display_graph(0,3);
    imgC.get_histogram(256).display_graph(0,3);
    imgD.get_histogram(256).display_graph(0,3);
    imgE.get_histogram(256).display_graph(0,3);
}