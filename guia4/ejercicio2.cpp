#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/getValue.cpp"
using namespace cimg_library;   //Necesario


int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");
    
    const char* input = cimg_option("-i", "../images/patron.tif", "Input Image File");
    
    CImg<unsigned char> img(input);
    img.display();

    return 0;
}
