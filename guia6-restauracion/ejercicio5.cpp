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
    //std::cout<<"a = "<<a<<"\tb = "<<b<<'\n'; 
    cimg_forXY(img[0],x,y) {
        double tempy;
        if (x == 0 and y == 0) {
            tempy = 1.0;
        }
        else { 
            tempy = 3.14159 * (double(x)*a + double(y)*b);
        }

        double val1 = T / tempy;
        double val2 = sin(tempy);
        
        //std::cout<<x<<' '<<y<<' '<<tempy<<' '<<val1<<' '<<val2<<' ';

        if (!fabs(val2) < EPS) {

            complex<double> val3 = exp(-I * tempy);
            if (fabs(cos(tempy)) < EPS) {
                img[0](x,y) = val1;
                img[1](x,y) = val1;
            }
            img[0](x,y) = real(val3) * val1 * val2;
            img[1](x,y) = imag(val3) * val1 * val2;
        } 
        //std::cerr<<img[0](x,y)<<' '<<img[1](x,y);
        //std::getchar();
    }
    return img;
}

int main(int argc, char *argv[]) {
    //@ 
    const char* input = cimg_option("-i", "../images/huang3_movida.tif", "Input Image File");
    const char* input2 = cimg_option("-i", "../images/huang3.jpg", "Input Image File");
    const float _a = cimg_option("-a", 1.0, "Input Image File");
    const float _b = cimg_option("-b", 1.0, "Input Image File");
	const float _t = cimg_option("-t", 1.0, "Input Image File");

    std::cout<<"a = "<<_a<<"\tb = "<<_b<<"\tt = "<<_t<<"\n";

    CImg<double> img(input);
    CImg<double> img2(input2);

    CImgList<double> filtro = filtroMovimiento(img.width(), img.height(), _a, _b, _t);

    CImg<double> filtrada = utils::filtradoFrecuencia(img, filtro);
    (filtro[0], filtro[1], utils::get_magnitud(filtro), img, filtrada, img2).display();

    return 0;
}
