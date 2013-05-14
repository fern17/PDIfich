#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
#include <utility>
#include "../utils/fourier.cpp"
#include "../utils/PDI_functions.h"
#include "../utils/filtradoFrecuencia.cpp"
using namespace cimg_library;   //Necesario

#define EPS 0.00001

//@ Devuelve un filtro de movimiento (pagina 260)
CImgList<double> filtroMovimiento(unsigned int w, unsigned int h, double a, double b, double T = 1) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    complex<double> I(0,1);
    
    cimg_forXY(img[0],x,y) {
        double tempy = M_PI*(x*a+y*b);

        double val1 = T / tempy;
        double val2 = sin(tempy);


        if (!fabs(val2) < EPS) {

            complex<double> val3 = exp(-I * tempy);
            if (abs(cos(tempy)) < EPS) {
                img[0](x,y) = val1;
                img[1](x,y) = val1;
            }
                img[0](x,y) = real(val3) * val1 * val2;
                img[1](x,y) = imag(val3) * val1 * val2;
        } 


    }
    return img;
}

int main(int argc, char *argv[]) {
    //@ 
    const char* input = cimg_option("-i", "../images/huang3_movida.tif", "Input Image File");
    const double _a = cimg_option("-a", 1, "Input Image File");
    const double _b = cimg_option("-b", 1, "Input Image File");
	const double _t = cimg_option("-t", 1, "Input Image File");

    CImg<double> img(input);

    CImgList<double> filtro = filtroMovimiento(img.width(), img.height(), _a, _b, _t);


    CImg<double> filtrada = utils::filtradoFrecuencia(img, filtro);
    (img, filtro[0], filtrada).display();


    return 0;
}
