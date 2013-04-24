#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/getValue.cpp"
using namespace cimg_library;   //Necesario


CImg<unsigned char> imagenMapeo(std::vector<unsigned int> &LUT) {
    unsigned int ancho = LUT.size();
    CImg<unsigned char> mapeo(ancho, ancho,1,1,0);
    for (unsigned int i = 0; i < ancho; i++) {
        unsigned int valor = LUT[i];
        mapeo(i, ancho - valor - 1) = 255;
    }
    return mapeo;
}

CImg<unsigned char> aplicarTabla(CImg<unsigned char> img, std::vector<unsigned int> LUT) {
    CImg<unsigned char> resultado(img.width(), img.height());
    cimg_forXY(img, x,y) {
        resultado(x,y) = LUT[img(x,y)];
    }
    return resultado;
}


std::vector<unsigned int> logarithmicLUT(float c) {
    std::vector<unsigned int> LUT;
    LUT.resize(256,0);
    for (unsigned int i = 0; i < LUT.size(); i++) {
        LUT[i] = utils::getLogarithmicValue(i, c);
    }
    return LUT;
}

std::vector<unsigned int> potLUT(float c, float gamma) {
    std::vector<unsigned int> LUT;
    LUT.resize(256,0);
    for (unsigned int i = 0; i < LUT.size(); i++) {
        LUT[i] = utils::getPotValue(i, c, gamma);
    }
    return LUT;
}
void inciso1a(const char *input) {
    CImg<unsigned char> img(input);
    float c;
    std::cout<<"Ingrese c: ";
    std::cin>>c;
    std::vector<unsigned int> LUT = logarithmicLUT(c);
    CImg<unsigned char> resultado = aplicarTabla(img, LUT);
    CImg<unsigned char> mapeo = imagenMapeo(LUT);
    CImgList<unsigned char> lista;
    lista.push_back(img);
    lista.push_back(mapeo);
    lista.push_back(resultado);
    lista.display();
}

void inciso1b(const char *input){
    CImg<unsigned char> img(input);
    float c, gamma;
    std::cout<<"Ingrese c: ";
    std::cin>>c;
    std::cout<<"Ingrese gamma: ";
    std::cin>>gamma;
    std::vector<unsigned int> LUT = potLUT(c,gamma);
    CImg<unsigned char> resultado = aplicarTabla(img, LUT);
    CImg<unsigned char> mapeo = imagenMapeo(LUT);
    CImgList<unsigned char> lista;
    lista.push_back(img);
    lista.push_back(mapeo);
    lista.push_back(resultado);
    lista.display();
}

int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");
    
    const char* input = cimg_option("-i", "../images/rmn.jpg", "Input Image File");
    const char* output = cimg_option("-o", "output1.bmp", "Output Image File");
    
    //inciso1a(input);
    inciso1b(input);

    return 0;
}
