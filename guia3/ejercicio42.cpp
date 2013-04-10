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

    const char* _input = cimg_option("-i", "../images/hubble.tif", "Input Image File");
    const char* _filter = cimg_option("-m", "filtro_ej3.txt", "Input filter File");
    const unsigned int _lado = cimg_option("-l", 5, "Input filter File");
    const unsigned int _umbral = cimg_option("-u", 150, "Input filter File");

    CImg<unsigned char> img(_input), output, output_grises(img.width(), img.height(), 1 , 1 , 0), 
                            img_binaria(img.width(), img.height(), 1 , 1 , 0);

    //Creamos el filtro de promediado
    utils::genArchivoMascara(_filter, _lado, _lado);
    CImg<float> filtro = get_filtro(_filter);

    output = img.get_convolve(filtro);

    cimg_forXY(output, x , y) {
        if (output(x,y) > _umbral ) {
            img_binaria(x,y) = 255;
            output_grises(x,y) = img(x,y);
        }
    }


    CImgList<unsigned char> lista;
    lista.assign(img, output, img_binaria, output_grises );
    lista.display();
}