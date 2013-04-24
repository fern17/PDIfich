#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <vector>
#include <cassert>
#include "../utils/genArchivoMascara.cpp"
#include "../utils/getValue.cpp"
using namespace cimg_library;   //Necesario

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

    const char* _input  = cimg_option("-i", "../images/esqueleto.tif", "Input Image File");
    const char* _filter = cimg_option("-m", "filtro_ej5.txt", "Input filter File");
    const char* _filter2 = cimg_option("-n", "filtro_ej72.txt", "Input filter File");
    const float _c = cimg_option("-c", 1, "Input filter File");
    const float _gamma = cimg_option("-g", 0.5, "Input filter File");
    const float _scalar = cimg_option("-e", 1.2, "Input filter File");

    std::cout<<"EscalarrrrrrrrrrrrrrrrrrrrrrrrrrrrrrRRRRRR "<<_scalar<<"\n";

    CImg<unsigned char> img(_input), img_equ, img_log, img_pot,filtrada_prom(img.width(), img.height(), 1, 1, 0), 
                        filtrada_acen(img.width(), img.height(), 1, 1, 0), filtrada_hb, output, output2, tempy;

    //LUT para logaritmo
    std::vector<unsigned int> LUT = potLUT(_c, _gamma);
    img_pot = aplicarTabla(img, LUT);
    //std::vector<unsigned int> LUT = logarithmicLUT(_c);
    //img_log = aplicarTabla(img, LUT);

    //Leemos el filtro de acentuado
    //CImg<float> filtro_acen = get_filtro(_filter);

    //Leemos el filtro de promediado
    CImg<float> filtro_prom = get_filtro(_filter2);

    //Ecualiza
    //img_equ = img_log.get_equalize(256,0,255);
    //img_equ = img_pot.get_equalize(256,0,255);
   
    //img_equ = img.get_equalize(256,0,255);
    
    //Promedia
    filtrada_prom = img_pot.get_convolve(filtro_prom);
    //Acentua
    //filtrada_acen = img.get_convolve(filtro_acen);

    filtrada_hb = _scalar*img - filtrada_prom;
    //Suma
    //output = filtrada_prom+filtrada_acen;

    CImgList<unsigned char> lista;
    //lista.assign(img_log, img_equ, filtrada_prom, filtrada_acen, output );
    lista.assign(img, img_pot, filtrada_prom );
    lista.display();
}
