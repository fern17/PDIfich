#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/genArchivoMascara.cpp"

using namespace cimg_library;   //Necesario


    
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
            std::cerr<<"wololo"<<valor<<"\n";
            salida(j,i) =  valor;
        }
    }
    f.close();
    return salida;
}

using namespace std;

int main(int argc, char *argv[]) {
    //@ Leer filtro, aplicar filtro, convolve con filtro de distinta medida

    const char* _input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* _filter = cimg_option("-m", "filtro_examen_m1.txt", "Input filter File");
    const char* _filter2 = cimg_option("-s", "filtro_examen_m2.txt", "Input filter File");

    CImg<double> img(_input), m1, m2;
    
    CImg<double> filtro = get_filtro(_filter);
    CImg<double> filtro2 = get_filtro(_filter2);


    m1 = img.get_convolve(filtro);
    m2 = m1.get_convolve(filtro2);

    CImgList<unsigned char> lista;
    lista.assign(img, m1, m2);
    lista.display();

   
}