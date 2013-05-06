#include <CImg.h>
#include <string>
#include <iostream>
#include <fstream>

namespace utils {
using namespace cimg_library;   //Necesario

//@ Genera una mascara de promediado
void genArchivoMascara(std::string archivo, unsigned int ancho, unsigned int alto) {
    std::ofstream file(archivo.c_str(), std::ios::trunc);
    file<<alto<<' '<<ancho<<'\n';
    unsigned int factor = ancho*alto;
    for (unsigned int i = 0; i < alto; i++) {
        for (unsigned int j = 0; j < ancho; j++) {
            float value = 1.0/factor;
            file<<value<<' ';
        }
        file<<'\n';
    }
    file.close();
}

//@ genera una mascara gaussiana
void genArchivoMascaraGaussiana(std::string archivo, unsigned int ancho, unsigned int alto, float sigma) {
    std::ofstream file(archivo.c_str(), std::ios::trunc);
    file<<alto<<' '<<ancho<<'\n';
    for (unsigned int i = 0; i < alto; i++) {
        for (unsigned int j = 0; j < ancho; j++) {
            int xx = j - (ancho/2.0);
            int yy = i - (alto/2.0);
            float value = (1/(2*3.14*sigma)) * exp(-(xx*xx+yy*yy)/(2*sigma*sigma));
            file<<value<<' ';
        }
        file<<'\n';
    }
    file.close();
}

//@ Lee un filtro desde archivo con el formato:
//filas columnas
//v00 v01 v02
//v10 v11 v12
//v20 v21 v22
CImg<double> get_filtro(std::string nombre) {
    std::ifstream f(nombre.c_str());
    if (!f.is_open()) {
        std::cout<<"No se pudo abrir el archivo "<<nombre<<"\n";
        exit(-1);
    }
    unsigned int filas;
    unsigned int columnas;
    double valor;

    f>>filas;
    f>>columnas;
    CImg<double> salida(columnas, filas, 1 , 1, 0);
    for (unsigned int i = 0; i < filas; i++) { 
        for (unsigned int j = 0; j < columnas; j++) {
            f>>valor;
            salida(j,i) =  valor;
        }
    }
    f.close();
    return salida;
}

}
