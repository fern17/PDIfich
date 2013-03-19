#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
using namespace cimg_library;   //Necesario


//Funcion que toma una imagen cualquiera, y un titulo de ventana, y la muestra
template<typename T>
void disp(CImg<T> img, std::string title = "titulo") {
    CImgDisplay ventana(img, title.c_str());   //Crea una ventana y dibuja la imagen...
    while ( not ventana.is_closed() && not ventana.is_keyQ()) {
        //bucle infinito
    }
}



void inciso1() {
    CImg<unsigned char> img(256,256);

    cimg_forXY(img,x,y) {
        img(x,y) = x;
    }
    disp(img);
}

void inciso2() {
    CImg<unsigned char> img(100,100);

    cimg_forXY(img,x,y) {
        img(x,y) = x;
    }

    img.normalize(0,255);
    disp(img);
}

void inciso3() {
    CImg<bool> img(100,100);

    cimg_forXY(img,x,y) {
        if (x%2 == 0)
            img(x,y) = false;
        else
            img(x,y) = true;
    }
    disp(img);
}

void drawcircle(unsigned int lado, unsigned int radio) {
    CImg<bool> img(lado,lado);

    unsigned int centro = lado/2;

    cimg_forXY(img,x,y) {
        if (  (x-centro)*(x-centro) + (y-centro)*(y-centro) <= radio*radio  )
            img(x,y) = true;
        else
            img(x,y) = false;
    }
    disp(img);
}


int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");

    const int ejercicio = cimg_option("-e", 1, "Exercise Number");

    const char* output = cimg_option("-o", "output.bmp", "Output Image File");

    const int lado = cimg_option("-l", 500, "Lado");
    const int radio = cimg_option("-r", 50, "Radio");
    
    switch (ejercicio) {
        case 1:
            inciso1();
        break;
        case 2:
            inciso2();
        break;
        case 3:
            inciso3();
        break;
        case 4:
            drawcircle(lado,radio);
        break;
    }

    return 0;
}
