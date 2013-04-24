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

    CImg<double> img(input);

    CImgList<double> tdf = img.get_FFT();

    CImg<double> magnitud = get_magnitud(tdf);
    CImg<double> fase = get_fase(tdf);


    // CImgList<double> solo_fase;
    // solo_fase.assign(magnitud, ceros);
    //El primer 2 del constructor indica el size de la lista (2 imagenes)
    CImgList<double> solo_magnitud(2, img.width(), img.height(), img.depth(), img.spectrum(), 0 );
    CImgList<double> solo_fase(2, img.width(), img.height(), img.depth(), img.spectrum(), 0 );
    
    complex<double> I(0,1);

    cimg_forXY(img, x, y) {
        complex<double> fase_cero = magnitud(x,y);
        complex<double> magnitud_uno = exp(I* fase(x,y) );

        solo_magnitud[0](x,y) = real(fase_cero);
        solo_magnitud[1](x,y) = imag(fase_cero);

        solo_fase[0](x,y) = real(magnitud_uno);
        solo_fase[1](x,y) = imag(magnitud_uno);
    }

    CImg<double> solo_fase_i = solo_fase.get_FFT(true)[0];
    CImg<double> solo_magnitud_i = solo_magnitud.get_FFT(true)[0];

    CImgList<double> lista;

    lista.assign(img, magn_tdf(img) , solo_fase_i, solo_magnitud_i);

    lista.display();

    return 0;
}
