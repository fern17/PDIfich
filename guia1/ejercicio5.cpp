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



int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");

    //Cargamos la imagen RMN

    // const int ejercicio = cimg_option("-e", 1, "Exercise Number");
    CImg<unsigned char> img_original("../images/rmn.jpg");
    unsigned int h = img_original.height();
    unsigned int w = img_original.width();

    std::cout<<w<<" "<<h<<std::endl;

    CImg<unsigned char> img_resultado1 = img_original.get_resize( w/2, h/2 ).get_resize(w,h);
    CImg<unsigned char> img_resultado2 = img_original.get_resize( w/4, h/4 ).get_resize(w,h);
    CImg<unsigned char> img_resultado3 = img_original.get_resize( w/8, h/8 ).get_resize(w,h);

    CImgList<unsigned char> lista;
    lista.assign(img_original, img_resultado1, img_resultado2, img_resultado3);
    CImgDisplay ventana(lista);
      while ( not ventana.is_closed() && not ventana.is_keyQ()) {
        //bucle infinito
    }

    // lista.display();

    return 0;
}
