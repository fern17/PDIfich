//Funciones para trabajar con LUT
namespace utils {

#include "getValue.cpp"
#include <CImg.h>
#include <vector>
using namespace cimg_library;   //Necesario


//@ Devuelve una imagen que representa el mapeo de LUT
CImg<unsigned char> imagenMapeo(std::vector<unsigned char> &LUT) {
    unsigned int ancho = LUT.size();
    CImg<unsigned char> mapeo(ancho, ancho,1,1,0);
    for (unsigned int i = 0; i < ancho; i++) {
        unsigned char valor = LUT[i];
        mapeo(i, ancho - valor - 1) = 255;
    }
    return mapeo;
}

//@ Aplica a img la LUT pasada por parametro
CImg<unsigned char> aplicarLUT(CImg<unsigned char> img, std::vector<unsigned char> LUT) {
    CImg<unsigned char> resultado(img.width(), img.height());
    cimg_forXY(img, x,y) {
        resultado(x,y) = LUT[img(x,y)];
    }
    return resultado;
}

//@ Crea una LUT con la transformada logaritmo
std::vector<unsigned char> logarithmicLUT(float c) {
    std::vector<unsigned char> LUT;
    LUT.resize(256,0);
    for (unsigned int i = 0; i < LUT.size(); i++) {
        LUT[i] = utils::getLogarithmicValue(i, c);
    }
    return LUT;
}

//@ Crea una LUT con la transformada potencia
std::vector<unsigned char> potLUT(float c, float gamma) {
    std::vector<unsigned char> LUT;
    LUT.resize(256,0);
    for (unsigned int i = 0; i < LUT.size(); i++) {
        LUT[i] = utils::getPotValue(i, c, gamma);
    }
    return LUT;
}
}
