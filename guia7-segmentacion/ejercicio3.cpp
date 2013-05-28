#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include "../utils/PDI_functions.h"

using namespace cimg_library;   //Necesario


//Struct que representa un click de un mouse
typedef struct click{
    int x;
    int y;
    void print(bool cr) {
        std::cout<<x<<' '<<y;
        if (cr)
            std::cout<<'\n';
    }
}click;

click click1;

//@ Registrac click y obtiene coordenadas
template<typename T>
void loopDisplayAndClick(CImg<T> img, std::string title, int delta) {
    CImgDisplay ventana(img, title.c_str());   //Crea una ventana y dibuja la imagen...
    int etiqueta = 600;
    while ( not ventana.is_closed() && not ventana.is_keyQ()) {
        ventana.wait(); //espera eventos
        if (ventana.button() && ventana.mouse_y() >= 0 && ventana.mouse_x() >= 0) { 
                click1.x = ventana.mouse_x();
                click1.y = ventana.mouse_y();
                CImg<unsigned char> segmentada = region_growing(img, click1.x, click1.y, delta, etiqueta);
                etiqueta++;
                img = segmentada;
                ventana.resize();
                ventana.render(img);
                ventana.paint();
        } 
    }
}


int main(int argc, char *argv[]) {
  const char* input = cimg_option("-i", "../images/bone.tif", "Input Image File");
  const int delta = cimg_option("-d", 20, "Delta de tolerancia");



  CImg<unsigned char> imagen(input);
  loopDisplayAndClick(imagen, "Clickee la semilla", delta);
  


  return 0;
}


