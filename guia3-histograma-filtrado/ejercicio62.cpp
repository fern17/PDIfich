#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/genArchivoMascara.cpp"

using namespace cimg_library;   //Necesario

CImg<unsigned char> restarImg(CImg<unsigned char> & img1, CImg<unsigned char> & img2) {
    assert( img1.is_sameXY(img2) );

    CImg<unsigned char> salida(img1.width(), img1.height(), 1, 1, 0);

    int temporal;
    cimg_forXY(img1,x,y) {
        temporal = (255 + img1(x,y) - img2(x,y) ) / 2;
        if (temporal < 0) temporal = 0;
        else if (temporal > 255) temporal = 255;
        
        salida(x,y) = temporal;
    }

    return salida;
}
    
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
    const char* _filter = cimg_option("-m", "filtro_ej3.txt", "Input filter File");
    const float _scalar = cimg_option("-e", 1.2, "Input filter File");


    CImg<unsigned char> img(_input), filtrada(img.width(), img.height(), 1, 1, 0), output, tempy;

    //Leemos el filtro de acentuado
    CImg<float> filtro = get_filtro(_filter);

    tempy = _scalar * img;


    filtrada = img.get_convolve(filtro);

    output = restarImg(tempy,filtrada);


    CImgList<unsigned char> lista;
    lista.assign(img, tempy, output.normalize(0,255) );
    lista.display();
}