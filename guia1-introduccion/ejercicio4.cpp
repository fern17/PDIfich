#include <CImg.h>               //include basico de CImg
#include <iostream>
using namespace cimg_library;   //Necesario

//@ Genera una imagen de degradado de 256x256
void inciso1() {
    CImg<unsigned char> img(256,256);

    cimg_forXY(img,x,y) {
        img(x,y) = x;
    }
    img.display("Degradado");
}

//@ Genera una imagen de degradado de 0 a 100 y luego la normaliza antes de dibujar
void inciso2() {
    CImg<unsigned char> img(100,100);

    cimg_forXY(img,x,y) {
        img(x,y) = x;
    }

    img.normalize(0,255);
    img.display("Degradado normalizada");
}

//@ Genera una imagen binaria de rayas verticales alternadas
void inciso3() {
    CImg<bool> img(100,100);

    cimg_forXY(img,x,y) {
        if (x % 2 == 0)
            img(x,y) = false;
        else
            img(x,y) = true;
    }
    img.display("Imagen binaria de rayas");
}

//@ Genera una imagen binaria de ladoxlado con un circulo centrado del radio pasado
void drawcircle(unsigned int lado, unsigned int radio) {
    CImg<bool> img(lado,lado);

    unsigned int centro = lado/2;

    cimg_forXY(img,x,y) {
        if (  (x-centro)*(x-centro) + (y-centro)*(y-centro) <= radio*radio  ) //si esta dentro del circulo
            img(x,y) = true;
        else
            img(x,y) = false;
    }
    img.display("Imagen binaria con circulo en el centro");
}


int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Generacion de imagenes varias usando cimg_forXY");

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
