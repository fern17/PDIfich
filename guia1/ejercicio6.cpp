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

    // const int ejercicio = cimg_option("-e", 1, "Exercise Number");
    CImg<unsigned char> img_original("../images/huang2.jpg");
    unsigned int h = img_original.height();
    unsigned int w = img_original.width();

    // std::vector<CImg <unsigned char> > img_vector;

    CImgList<unsigned char> lista;
    lista.push_back(img_original);

    for (unsigned int i = 1 ; i < 9; i++) {
        CImg <unsigned char> tmp = img_original.get_quantize(9-i);
        // img_vector.push_back(tmp);
        lista.push_back(tmp);
    }

    CImgDisplay ventana(lista);
      while ( not ventana.is_closed() && not ventana.is_keyQ()) {
        //bucle infinito
    }

    return 0;
}
