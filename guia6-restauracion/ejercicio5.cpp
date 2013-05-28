#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
#include <utility>
#include "../utils/fourier.cpp"
#include "../utils/PDI_functions.h"
#include "../utils/filtradoFrecuencia.cpp"
#include <cassert>
using namespace cimg_library;   //Necesario

#define EPS 0.00001

//@ Aplica el filtrado de Wiener para un filtro pasado por parametro
CImg<double> filtradoWiener(CImg<double> img, CImgList<double> filtro, double K) {
    unsigned int w = img.width();
    unsigned int h = img.height();
    CImgList<double> resultado(2, w, h, img.depth(), img.spectrum(), 0);
    CImgList<double> f_img = img.get_FFT();
    double re;
    double im;

    cimg_forXY(img, u, v) {
        //Para no hacer division por cero
        if (fabs(filtro[0](u,v)) < EPS)
            re = 0.0;
        else
            re = 1/filtro[0](u,v);
        //Para no hacer division por cero
        if (fabs(filtro[1](u,v)) < EPS)
            im = 0.0;
        else
            im = 1/filtro[1](u,v);
        //Componemos el primer factor 1/H
        std::complex<double> factor1 (re, im);

        //Componemos el segundo factor: H^2/(H^2 + K)
        double pot = std::pow(filtro[0](u,v), 2) + std::pow(filtro[1](u,v), 2);
        double fac2 = (pot) / (pot + K);
        std::complex<double> factor2 (fac2, 0.0);
        
        //Componemos el tercer factor: G(u,v)
        std::complex<double> factor3 (f_img[0](u,v), f_img[1](u,v));

        //Realizamos el producto de binomios
        std::complex<double> prod = factor1*factor2*factor3;

        //Asignamos en real e imaginario
        resultado[0](u,v) = std::real(prod);
        resultado[1](u,v) = std::imag(prod);
    }
    CImg<double> resultado_espacio = resultado.get_FFT(true)[0];
    return resultado_espacio;
}


//@ Devuelve un filtro de movimiento (pagina 260)
CImgList<double> filtroMovimiento(unsigned int w, unsigned int h, double a, double b, double T = 1.0) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    complex<double> I(0,1);
    int inicio_x = -int(w)/2;
    int inicio_y = -int(h)/2;
    int fin_x = int(w)/2;
    int fin_y = int(h)/2;

    for (int x = inicio_x; x < fin_x; x++) {
        for (int y = inicio_y; y < fin_y; y++) {
    //for (int x = 0; x < w; x++) {
    //    for (int y = 0; y < h; y++) {
            double tempy;
            if (x == 0 and y == 0) 
                tempy = 1.0;
            else 
                tempy = 3.14159f * (double(x)*a + double(y)*b);
                //tempy = 3.14159f * (double(x-w/2)*a + double(y-h/2)*b);
          
            double val1 = T / tempy;
            double val2 = sin(tempy);
            
            unsigned int pos_x = x+w/2;
            unsigned int pos_y = y+h/2;
            
            //std::cout<<x<<' '<<y<<' '<<pos_x<<' '<<pos_y<<'\n';
            if (fabs(val2) > EPS) {
                complex<double> val3 = exp(-I * tempy);
                img[0](pos_x, pos_y) = real(val3) * val1 * val2;
                img[1](pos_x, pos_y) = imag(val3) * val1 * val2;
                //img[0](x,y) = real(val3) * val1 * val2;
                //img[1](x,y) = imag(val3) * val1 * val2;
                
                //std::cout<<tempy<<' '<<val1<<' '<<val2<<' '<<real(val3)<<' '<<imag(val3)<<'\n';
                //std::cout<<img[0](pos_x, pos_y)<<' '<<img[1](pos_x, pos_y)<<'\n';
                //std::getchar();
            }
        }
    }
    img[0].shift(-w/2, -h/2, 0, 0, 2);
    img[1].shift(-w/2, -h/2, 0, 0, 2);
    return img;
}

CImg<double> filtradoInverso(CImgList<double> img, CImgList<double> filtro, double epsilon) {
    assert(img.is_sameXY(filtro));
    CImgList<double> resultado(2,img[0].width(), img[0].height(), 1, img[0].spectrum(), 0.0);
    cimg_forXY(img[0], u, v) {
        std::complex<double> parte_img(img[0](u,v), img[1](u,v));
        std::complex<double> parte_filtro(filtro[0](u,v), filtro[1](u,v));
        if (not (fabs(real(parte_filtro)) < epsilon and fabs(imag(parte_filtro)))) { //si no son cero ambos
            std::complex<double> res = parte_img/parte_filtro;
            resultado[0](u,v) = real(res);
            resultado[1](u,v) = imag(res);
        }
    }
    return resultado.get_FFT(true)[0];
}


int main(int argc, char *argv[]) {
    //@ Realiza un filtro de movimiento y le aplica el filtro de Wiener con parametro K
    const char* input = cimg_option("-i", "../images/huang3_movida.tif", "Input Image File");
    const char* input2 = cimg_option("-j", "../images/huang3.jpg", "Input Image File");
    const float _a = cimg_option("-a", 1.0, "Input Image File");
    const float _b = cimg_option("-b", 1.0, "Input Image File");
	const float _t = cimg_option("-t", 1.0, "Input Image File");
	const float _K = cimg_option("-k", 1.0, "Input Image File");
	const double epsilon = cimg_option("-e", 0.0001, "Input Image File");

    std::cout<<"Parametros:\na = "<<_a<<"\tb = "<<_b<<"\tT = "<<_t<<"\tK = "<<_K<<"\n";

    CImg<double> img(input);
    CImg<double> original(input2);

    //Realizamos el filtro
    CImgList<double> filtro = filtroMovimiento(original.width(), original.height(), _a, _b, _t);
    CImg<double> movida = utils::filtradoFrecuencia(original, filtro);

    //Filtramos con el filtro inverso
    CImg<double> filtrada = filtradoInverso(movida.get_FFT(), filtro, epsilon);

    //Filtramos con el filtro de Wiener
    //CImg<double> filtrada = filtradoWiener(img, filtro, _K); 
    
    //Dibujamos
    (filtro[0], filtro[1], movida, filtrada, original).display();

    return 0;
}
