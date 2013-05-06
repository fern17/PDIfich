#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include "../utils/genArchivoMascara.cpp"
using namespace cimg_library;   //Necesario

int main(int argc, char *argv[]) {
    const char* _input  = cimg_option("-i", "../images/esqueleto.tif", "Input Image File");
    const char* _filtro_acentuado = cimg_option("-m", "filtro_laplaciano.txt", "Filtro de acentuado");
    const float exponent = cimg_option("-e", 0.5, "Exponente de Exponenciacion");
    const float ganancia = cimg_option("-g", 1.4, "Ganancia en el filtro de alta potencia");

    CImg<unsigned char> img(_input);
    CImg<unsigned char> img_laplaciano;

    //Aplicamos un filtro de acentuado
    CImg<double> filtro_laplaciano = utils::get_filtro(_filtro_acentuado);
    img_laplaciano = img.get_convolve(filtro_laplaciano).get_normalize(0,255);

    //Aplicamos una mascara difusa
    CImg<double> img_suma_laplaciano(img.width(), img.height(), 1, 1, 0);
    cimg_forXY(img,x,y) {
        img_suma_laplaciano(x,y) = ((ganancia-1)*img(x,y) + img_laplaciano(x,y)) / 2;
    }
    img_suma_laplaciano.normalize(0,255);
    

    //Aplicamos una potenciacion
    CImg<double> img_pot(img.width(), img.height(), 1, 1, 0);
    cimg_forXY(img_pot,x,y) {
        double value = img_suma_laplaciano(x,y);
        img_pot(x,y) = std::min(std::pow(value,exponent),255.0);
    }
    img_pot.normalize(0,255);

    (img, img_laplaciano, img_suma_laplaciano, img_pot).display();

    return 0;

}
