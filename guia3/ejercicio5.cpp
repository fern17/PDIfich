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
            std::cout<<salida(j,i)<<' ';
        }
        std::cout<<'\n';
    }
    f.close();
    return salida;
}

using namespace std;

int main(int argc, char *argv[]) {
    //@ Leer filtro acentuado, aplicar filtro acentuado, convolve con filtro de distinta medida

    const char* _input = cimg_option("-i", "../images/lenna.gif", "Input Image File");
    const char* _filter = cimg_option("-m", "filtro_ej5.txt", "Input filter File");


    CImg<unsigned char> img(_input), output(img.width(), img.height(), 1, 1, 0);

    //Leemos el filtro de acentuado
    CImg<float> filtro = get_filtro(_filter);

    output = img.get_convolve(filtro);


    CImgList<unsigned char> lista;
    lista.assign(img, output.normalize(0,255) );
    lista.display();
}