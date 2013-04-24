#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <complex>
#include "../utils/PDI_functions.h"
using namespace cimg_library;   //Necesario


CImg<double> get_magnitud(CImgList<double> & tdf) {
    CImg<double> output = tdf[0];

    cimg_forXY(output, x ,y ) {
        output(x,y) = sqrt( pow(tdf[0](x,y),2) + pow(tdf[1](x,y),2) );
    }
    return output;
}

CImg<double> get_fase(CImgList<double> & tdf) {
    CImg<double> output = tdf[0];

    cimg_forXY(output, x ,y ) {
        output(x,y) = atan2( tdf[1](x,y) , tdf[0](x,y) );
    }
    return output;
}

int main(int argc, char *argv[]) {
    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
	const char* input2 = cimg_option("-j", "../images/coins.tif", "Input Image File");

    CImg<double> img(input);
    CImg<double> img2(input2);

    CImgList<double> tdf = img.get_FFT();
    CImgList<double> tdf2 = img2.get_FFT();

    CImg<double> magnitud = get_magnitud(tdf);
    CImg<double> fase = get_fase(tdf2);

    CImgList<double> resultado(2, img.width(), img.height(), img.depth(), img.spectrum(), 0 );

    
    complex<double> I(0,1);

    cimg_forXY(img, x, y) {
        complex<double> valor = magnitud(x,y) * exp(I * fase(x,y) );

        resultado[0](x,y) = real(valor);
        resultado[1](x,y) = imag(valor);
    }

    CImg<double> resultado_i = resultado.get_FFT(true)[0];

    CImgList<double> lista;

    lista.assign(img, img2, resultado_i);

    lista.display();

    return 0;
}
