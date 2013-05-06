#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/genArchivoMascara.cpp"

using namespace cimg_library;   //Necesario


//@ Lee un filtro desde archivo    
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


int main(int argc, char *argv[]) {
    //@ Leer filtro, aplicar filtro, convolve con filtro de distinta medida

    const char* _input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* _filter = cimg_option("-m", "filtro_ej3.txt", "Input filter File");
    const unsigned int _lado = cimg_option("-l", 5, "Input filter File");
    
    CImg<double> img(_input), output;

    //Genera el filtro de promediado
    utils::genArchivoMascara(_filter, _lado, _lado);
    CImg<double> filtro = get_filtro(_filter);

    //Aplica el filtro
    output = img.get_convolve(filtro);

    (img, output).display();
}
