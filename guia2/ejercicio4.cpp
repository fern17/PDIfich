#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/getValue.cpp"
using namespace cimg_library;   //Necesario



CImg<bool> drawcircle(unsigned int lado, unsigned int radio) {
    CImg<bool> img(lado,lado);

    unsigned int centro = lado/2;

    cimg_forXY(img,x,y) {
        if (  (x-centro)*(x-centro) + (y-centro)*(y-centro) <= radio*radio  )
            img(x,y) = true;
        else
            img(x,y) = false;
    }
    return img;
}


//Funcion que toma una imagen cualquiera, y un titulo de ventana, y la muestra
template<typename T>
void disp(CImg<T> img, std::string title = "titulo") {
    CImgDisplay ventana(img, title.c_str());   //Crea una ventana y dibuja la imagen...
    while ( not ventana.is_closed() && not ventana.is_keyQ()) {
    }
}

CImg<unsigned char> sumarImg(CImg<unsigned char> & img1, CImg<unsigned char> & img2) {
    assert( img1.is_sameXY(img2) );

    CImg<unsigned char> salida(img1.width(), img1.height(), 1, 1, 0);

    cimg_forXY(img1,x,y) {
        salida(x,y) = ( img1(x,y) + img2(x,y) ) / 2;
    }

    return salida;
}

int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");

    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const int corrimientox = cimg_option("-x", 1, "Corrimiento en X"); // (Pueden ser negativos los corrimientos: ferdi lo dijo)
    const int corrimientoy = cimg_option("-y", 1, "Corrimiento en Y");

    CImg<unsigned char> imagen(input);
    CImg<unsigned char> salida;
    CImg<unsigned char> negativa(imagen.width(), imagen.height() , 1 , 1 , 0);

    //Genero la negativa
    cimg_forXY(imagen,x,y) {
        negativa(x,y) = utils::getLinearValue(imagen(x,y) , 0 , -1 , 255);
    }

    //Corrimiento
    negativa.shift(corrimientox, corrimientoy);

    salida = sumarImg(imagen, negativa);


    CImgList<unsigned char> lista;
    lista.assign(imagen, negativa, salida);

    lista.display();

    return 0;
}
