#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
using namespace cimg_library;   //Necesario

template<typename T>
bool fueraDeRango(CImg<T> img, int x, int y, int w, int h) {
    if (y < 0 or y > w) 
        return true;
    if (x < 0 or x > h)
        return true;
    return false;
}


template<typename T>
CImg<T> obtenerVecindario(CImg<T> &img, unsigned int x0, unsigned int y0, unsigned int w, unsigned int h, unsigned int step_x, unsigned int step_y) {
    CImg<T> ret_val (step_x*2+1, step_y*2+1,1, 1, 0); //imagen a retornar que es un vecindario de img de tamaño ancho x alto
    unsigned int xx = 0;           //posicion x a reemplazar en ret_val
    unsigned int yy = 0;           //posicion y a reemplazar en ret_val
    for (int i = y0 - step_y; i <= y0 + step_y; i++, yy++) { 
        for (int j = x0 - step_x; j <= x0 + step_x; j++, xx++) {
            if (fueraDeRango(img, j, i, w, h)) {
                std::cout<<"fuera de rango\n";
                ret_val(xx,yy) = 0;
            }
            else 
                ret_val(xx,yy) = img(j,i);
        }
    }
    return ret_val; 
}

template<typename T>
CImg<T> promediar(CImg<T> img, unsigned int ancho = 3, unsigned int alto = 3) {
    CImg<T> ret_val = img;
    unsigned int h = img.height(); //ancho de la imagen
    unsigned int w = img.width();  //alto de la imagen
    unsigned int step_x = ancho/2; //ancho del vecindario centrado
    unsigned int step_y = alto/2;  //alto del vecindario centrado
    for (unsigned int i = 0; i < img.width(); i++) {
        for (unsigned int j = 0; j < img.height(); j++) {
            CImg<T> vecindario = obtenerVecindario(img, j, i, w, h, step_x, step_y);
            ret_val(j,i) = vecindario.mean();
        }
    }
    return ret_val;
}

template<typename T>
CImg<T> histogramaLocal(CImg<T> img, unsigned int ancho = 3, unsigned int alto = 3) {
    CImg<T> ret_val = img;
    unsigned int h = img.height(); //ancho de la imagen
    unsigned int w = img.width();  //alto de la imagen
    unsigned int step_x = ancho/2; //ancho del vecindario centrado
    unsigned int step_y = alto/2;  //alto del vecindario centrado
    for (unsigned int i = 0; i < img.width(); i++) {
        for (unsigned int j = 0; j < img.height(); j++) {
            CImg<T> vecindario = obtenerVecindario(img, j, i, w, h, step_x, step_y);
            vecindario.equalize(256, 0, 255);
            ret_val(j,i) = vecindario(step_y,step_x); //asigna el centro del vecindario
        }
    }
    return ret_val;
}

int main(int argc, char *argv[]) {
    const char* _input  = cimg_option("-i", "../images/cuadros.tif", "Input Image File");
    const unsigned int _ancho = cimg_option("-w", 3, "Ancho de la mascara");
    const unsigned int _alto = cimg_option("-h", 3, "Alto de la mascara");

    CImg<unsigned char> img(_input);
    CImg<unsigned char> hist_local = histogramaLocal(img, _ancho, _alto);
    hist_local.display();

    return 0;
 }
