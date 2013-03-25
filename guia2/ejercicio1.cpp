#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
using namespace cimg_library;   //Necesario


CImg<unsigned char> obtenerMapeo(std::vector<unsigned int> &LUT) {
    unsigned int ancho = LUT.size();
    CImg<unsigned char> mapeo(ancho, ancho);
    for (unsigned int i = 0; i < ancho; i++) {
        unsigned int valor = LUT[i];
        mapeo(ancho - i, valor) = 255;
    }
    return mapeo;
}


//Funcion que toma una imagen cualquiera, y un titulo de ventana, y la muestra
template<typename T>
void disp(CImg<T> img, std::string title = "titulo") {
    CImgDisplay ventana(img, title.c_str());   //Crea una ventana y dibuja la imagen...
    while ( not ventana.is_closed() && not ventana.is_keyQ()) {
        //bucle infinito
    }
}

void tablaLineal(std::vector<unsigned int> &LUT, float a, float c, unsigned int inicio=0, unsigned int final=256) {
    if (LUT.size() != 256){
        std::cout<<LUT.size()<<" Resize\n";
        LUT.resize(256, 0); //resizea y rellena con ceros
    }
    for (unsigned int i = inicio; i < final; i++) {
        float var = a*i+c;
        if (var > 255) 
            LUT[i] = 255;
        else if (var < 0) 
            LUT[i] = 0;
        else 
            LUT[i] = (unsigned int) var; //truncado
    }
}

void tablaNegativo(std::vector<unsigned int> &LUT) {
    return tablaLineal(LUT,-1,255);
}

CImg<unsigned char> aplicarTabla(CImg<unsigned char> img, std::vector<unsigned int> LUT) {
    CImg<unsigned char> resultado(img.width(), img.height());
    cimg_forXY(img, x,y) {
        resultado(x,y) = LUT[img(x,y)];
    }
    return resultado;
}

int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");
    
    float a = cimg_option("-a", 1.0, "Escalado");
    float c = cimg_option("-c", 0.0, "Offset");
    unsigned int inicio;
    unsigned int final;
    std::cout<<"Inicio del intervalo: ";
    std::cin>>inicio;
    std::cout<<"\nFin del intervalo: ";
    std::cin>>final;


    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* output = cimg_option("-o", "output1.bmp", "Output Image File");

    CImg<unsigned char> img(input);
    CImg<unsigned char> resultado_lineal;
    CImg<unsigned char> resultado_negativo;
    
    std::vector<unsigned int> LUT_lineal;
    std::vector<unsigned int> LUT_negativo;
    tablaLineal(LUT_lineal, 1, 0);
    std::cout<<LUT_lineal.size()<<'\n';
    tablaLineal(LUT_lineal, a, c, inicio, final);
    std::cout<<LUT_lineal.size()<<'\n';
    
    tablaNegativo(LUT_negativo);

    resultado_lineal = aplicarTabla(img, LUT_lineal);
    resultado_negativo = aplicarTabla(img, LUT_negativo);
    
    CImgList<unsigned char> lista;
    lista.push_back(img);
    lista.push_back(resultado_lineal);
    lista.push_back(resultado_negativo);
    lista.display();


    return 0;
}
