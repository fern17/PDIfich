#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <complex>
#include <ctime>
#include <fstream>
#include "../utils/filtradoFrecuencia.cpp"
#include "../utils/fourier.cpp"
#include "../utils/PDI_functions.h"


using namespace cimg_library;   //Necesario

 
int main(int argc, char *argv[]) {
      CImg<double> imagen("/home/marian0/PDIfich/problemas-practica/img/cervezas/01.jpg");

    CImg<double> transformada = magn_tdf(imagen, true, "/home/marian0/PDIfich/paletas/hot.pal");

    transformada.display();
}
