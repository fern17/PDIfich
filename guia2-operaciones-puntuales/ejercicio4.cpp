#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/getValue.cpp"
using namespace cimg_library;   //Necesario


CImg<unsigned char> sumarImg(CImg<unsigned char> & img1, CImg<unsigned char> & img2) {
    assert( img1.is_sameXY(img2) );

    CImg<unsigned char> salida(img1.width(), img1.height(), 1, 1, 0);

    cimg_forXY(img1,x,y) {
        salida(x,y) = ( img1(x,y) + img2(x,y) ) / 2;
    }

    return salida;
}

//@ Aplica el filtro emboss a la imagen con el corrimiento pasado por parametros
CImg<unsigned char> emboss(CImg<unsigned char> img, int corrimientox = 1, int corrimientoy = 1) {

    CImg<unsigned char> salida;
    CImg<unsigned char> negativa(img.width(), img.height() , 1 , 1 , 0);

    //Genero la negativa
    cimg_forXY(img,x,y) {
        negativa(x,y) = utils::getLinearValue(img(x,y) , 0 , -1 , 255);
    }

    //Corremos la imagen
    negativa.shift(corrimientox, corrimientoy);

    //Sumamos ambas imagenes
    salida = sumarImg(img, negativa);

    return salida;
}

int main(int argc, char *argv[]) {
    //@ Aplica el filtro emboss
    cimg_usage("Filtro emboss");

    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const int corrimientox = cimg_option("-x", 1, "Corrimiento en X"); // (Pueden ser negativos los corrimientos: ferdi lo dijo)
    const int corrimientoy = cimg_option("-y", 1, "Corrimiento en Y");

    CImg<unsigned char> imagen(input);
   
    CImg<unsigned char> salida = emboss(imagen, corrimientox, corrimientoy);

    //Graficamos
    CImgList<unsigned char> lista;
    lista.assign(imagen, salida);

    lista.display("Filtro Emboss");

    return 0;
}
