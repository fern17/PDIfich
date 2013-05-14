//Funciones utilizadas para manejar fourier
namespace utils {
#include <CImg.h>
#include <cmath>
using namespace cimg_library;   //Necesario

//Devuelve la magnitud de una transformada de Fourier    
CImg<double> get_magnitud(CImgList<double> tdf, bool centrado = false) {
    CImg<double> output = tdf[0];

    cimg_forXY(output, x ,y ) {
        output(x,y) = sqrt( pow(tdf[0](x,y),2) + pow(tdf[1](x,y),2) );
    }

    if (centrado) {
        unsigned int w = output.width();
        unsigned int h = output.height();
        output.shift(w/2, h/2,0,0,2);
    }
    return output;
}

//Devuelve la fase de una transformada de Fourier
CImg<double> get_fase(CImgList<double> & tdf) {
    CImg<double> output = tdf[0];

    cimg_forXY(output, x ,y ) {
        output(x,y) = atan2( tdf[1](x,y) , tdf[0](x,y) );
    }
    return output;
}

}
