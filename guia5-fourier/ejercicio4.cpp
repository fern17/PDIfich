#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <complex>
#include <ctime>
#include "../utils/PDI_functions.h"
#include "../utils/fourier.cpp"
using namespace cimg_library;   //Necesario

//@ Genera un filtro ideal con el radio especificado (frecuencia de corte)
CImgList<double> filtroIdeal(unsigned int w, unsigned int h, unsigned int radio, bool pasaaltos = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    //Si es pasa altos, tenemos que iniciarla con 1, no con 0
    if (pasaaltos) {
        img[0].fill(1);
        img[1].fill(1);
    }

    unsigned int centro_x = w/2;
    unsigned int centro_y = h/2;
    
    cimg_forXY(img[0],x,y) {
        if (  (x - centro_x)*(x - centro_x) + (y - centro_y)*(y - centro_y) <= radio*radio  ) {//si esta dentro del circulo
            if (pasaaltos) img[0](x,y) = 0.0;
            else img[0](x,y) = 1.0;
        }
    }

    img[0].shift(-w/2, -h/2,0,0,2);
    return img;
}


//@ Genera un filtro de Butterworth del orden y frecuencia de corte dados
CImgList<double> filtroButterworth(unsigned int w, unsigned int h, unsigned int frec_corte, unsigned int orden, bool pasaaltos = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    unsigned int centro_x = w/2;
    unsigned int centro_y = h/2;
    
    cimg_forXY(img[0],x,y) {
        double distancia = sqrt( (x - centro_x)*(x - centro_x) + (y - centro_y)*(y - centro_y)); 
        if (pasaaltos) img[0](x,y) = (1)/(1 + pow((float(frec_corte)/distancia), 2*orden));
        else img[0](x,y) =           (1)/(1 + pow((distancia/float(frec_corte)), 2*orden));
    }

    img[0].shift(-w/2, -h/2,0,0,2);
    return img;
}

//@ Genera un filtro Gaussiano
CImgList<double> filtroGaussiano(unsigned int w, unsigned int h, unsigned int frec_corte, bool pasaaltos = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    unsigned int centro_x = w/2;
    unsigned int centro_y = h/2;
    
    cimg_forXY(img[0],x,y) {
        double distancia = sqrt( (x - centro_x)*(x - centro_x) + (y - centro_y)*(y - centro_y)); 
        if (pasaaltos) img[0](x,y) = 1 - exp(-pow(distancia,2)/(2*(pow(frec_corte,2))));
        else           img[0](x,y) = exp(-pow(distancia,2)/(2*(pow(frec_corte,2))));
    }

    img[0].shift(-w/2, -h/2,0,0,2);
    return img;
}

CImg<double> filtradoFrecuencia(CImg<double> img, CImgList<double> filtro_frec) {
    
    //Obtenemos las transformadas de Fourier de la imagen y el filtro
    CImgList<double> fft_img = img.get_FFT(); 
    
    //Multiplicamos en frecuencia
    CImgList<double> tempy(2, img.width(), img.height(), img.depth(), img.spectrum(), 0);
   
    cimg_forXY(img,x,y) {
        //Capturamos los valores
        complex<double> factor1 (fft_img[0](x,y), fft_img[1](x,y)),
                        factor2 (filtro_frec[0](x,y), filtro_frec[1](x,y));

        //Realizamos el producto de binomios
        complex<double> prod = factor1*factor2;
        //Asignamos en real e imaginario
        tempy[0](x,y) = real(prod);
        tempy[1](x,y) = imag(prod);
    }
    //Calculamos la inversa
    return tempy.get_FFT(true)[0];
}


int main(int argc, char *argv[]) {
    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const unsigned int _frec_corte = cimg_option("-f", 50, "Input Image File");
    const unsigned int _orden_butterworth = cimg_option("-b", 2, "Input Image File");

    CImg<double> img(input), resultado_ideal_pb, resultado_butterworth_pb, resultado_gaussiano_pb;
    CImg<double> resultado_ideal_pa, resultado_butterworth_pa, resultado_gaussiano_pa;

    unsigned int w = img.width();
    unsigned int h = img.height();
    CImgList<double> filtro_ideal_pb = filtroIdeal(w, h, _frec_corte);
    CImgList<double> filtro_butterworth_pb = filtroButterworth(w, h, _frec_corte, _orden_butterworth);
    CImgList<double> filtro_gaussiano_pb = filtroGaussiano(w, h, _frec_corte);

    //Calculamos todos los resultados
    resultado_ideal_pb = filtradoFrecuencia(img, filtro_ideal_pb);
    resultado_butterworth_pb = filtradoFrecuencia(img, filtro_butterworth_pb);
    resultado_gaussiano_pb = filtradoFrecuencia(img, filtro_gaussiano_pb);

    //Dibuja
    CImgList<double> lista_pb;
    lista_pb.assign(img, utils::get_magnitud(filtro_ideal_pb), resultado_ideal_pb,
                      utils::get_magnitud(filtro_butterworth_pb), resultado_butterworth_pb,
                      utils::get_magnitud(filtro_gaussiano_pb), resultado_gaussiano_pb);
    lista_pb.display();


    //Filtrado pasa altos
    CImgList<double> filtro_ideal_pa = filtroIdeal(w, h, _frec_corte, true);
    CImgList<double> filtro_butterworth_pa = filtroButterworth(w, h, _frec_corte, _orden_butterworth, true);
    CImgList<double> filtro_gaussiano_pa = filtroGaussiano(w, h, _frec_corte, true);

    //Calculamos todos los resultados
    resultado_ideal_pa = filtradoFrecuencia(img, filtro_ideal_pa);
    resultado_butterworth_pa = filtradoFrecuencia(img, filtro_butterworth_pa);
    resultado_gaussiano_pa = filtradoFrecuencia(img, filtro_gaussiano_pa);

    //Dibuja
    CImgList<double> lista_pa;
    lista_pa.assign(img, utils::get_magnitud(filtro_ideal_pa), resultado_ideal_pa,
                      utils::get_magnitud(filtro_butterworth_pa), resultado_butterworth_pa,
                      utils::get_magnitud(filtro_gaussiano_pa), resultado_gaussiano_pa);
    lista_pa.display();

    return 0;
}
