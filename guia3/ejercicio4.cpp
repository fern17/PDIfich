#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/genArchivoMascara.cpp"

using namespace cimg_library;   //Necesario


    
CImg<float> get_filtro(std::string nombre) {
    std::ifstream f(nombre.c_str());
    if (!f.is_open()) {
        std::cout<<"No se pudo abrir el archivo "<<nombre<<"\n";
        exit(-1);
    }
    unsigned int filas;
    unsigned int columnas;
    float valor;

    f>>filas;
    f>>columnas;
    CImg<float> salida(columnas, filas, 1 , 1, 0);
    for (unsigned int i = 0; i < filas; i++) { 
        for (unsigned int j = 0; j < columnas; j++) {
            f>>valor;
            // std::cerr<<"wololo"<<valor<<"\n";
            salida(j,i) =  valor;
        }
    }
    f.close();
    return salida;
}

using namespace std;

int main(int argc, char *argv[]) {
    //@ Leer filtro, aplicar filtro, convolve con filtro de distinta medida

    const char* _input = cimg_option("-i", "../images/blister_completo.jpg", "Input Image File");
    const char* _filter = cimg_option("-m", "filtro_ej3.txt", "Input filter File");
    const unsigned int _lado = cimg_option("-l", 5, "Input filter File");
    const float _sigma = cimg_option("-s", 1, "Input filter File");

    CImg<unsigned char> img(_input), output, output_gaussiano;

    //Creamos el filtro de promediado
    utils::genArchivoMascara(_filter, _lado, _lado);
    CImg<float> filtro = get_filtro(_filter);
    
    //Creamos el filtro de Gaussiana
    utils::genArchivoMascaraGaussiana(_filter, _lado, _lado, _sigma);
    CImg<float> filtro_gaussiano = get_filtro(_filter);


    output = img.get_convolve(filtro);
    output_gaussiano = img.get_convolve(filtro_gaussiano);


    CImgList<unsigned char> lista;
    lista.assign(img, output, output_gaussiano);

   lista.display();
}