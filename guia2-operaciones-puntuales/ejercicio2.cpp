#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/getValue.cpp"
#include "../utils/LUT.cpp"
using namespace cimg_library;   //Necesario

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

//@ Lee una imagen desde input y le aplica la transformada logaritmo
void inciso1a(const char *input) {
    CImg<unsigned char> img(input);
  
    std::cout<<"Transformacion Logaritmo: s = c log (1+r)\n";
    //Leemos el parametro de la funcion logaritmo
    float c;
    std::cout<<"Ingrese c: ";
    std::cin>>c;
    
    //Creamos la LUT
    std::vector<unsigned char> LUT = logarithmicLUT(c);

    //Aplicamos la LUT
    CImg<unsigned char> resultado = utils::aplicarLUT(img, LUT);
    
    //Generamos una imagen del mapeo para graficar
    CImg<unsigned char> mapeo = utils::imagenMapeo(LUT);

    //Graficamos
    CImgList<unsigned char> lista;
    lista.push_back(img);
    lista.push_back(mapeo);
    lista.push_back(resultado);
    lista.display();
}

//@ Lee una imagen desde input y le aplica la transformada potencia
void inciso1b(const char *input){
    CImg<unsigned char> img(input);
   
    std::cout<<"Transformacion potencia: s = c r^gamma\n";
    //Leemos los parametros de la funcion potencia
    float c, gamma;
    std::cout<<"Ingrese c: ";
    std::cin>>c;
    std::cout<<"Ingrese gamma: ";
    std::cin>>gamma;

    //Creamos la LUT
    std::vector<unsigned char> LUT = potLUT(c,gamma);
    
    //Aplicamos la LUT
    CImg<unsigned char> resultado = utils::aplicarLUT(img, LUT);
    //Realizamos la imagen mapeo para graficar
    CImg<unsigned char> mapeo = utils::imagenMapeo(LUT);

    //Graficacion
    CImgList<unsigned char> lista;
    lista.push_back(img);
    lista.push_back(mapeo);
    lista.push_back(resultado);
    lista.display();
}

int main(int argc, char *argv[]) {
    //@ Aplica las transformaciones logaritmo y potencia a una imagen
    cimg_usage("Aplicacion de transformaciones logaritmo y potencia");
    
    const char* input = cimg_option("-i", "../images/rmn.jpg", "Input Image File");
    const char* output = cimg_option("-o", "output1.bmp", "Output Image File");
    
    inciso1a(input);
    inciso1b(input);

    return 0;
}
