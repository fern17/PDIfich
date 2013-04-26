#include <CImg.h>               //include basico de CImg
#include <iostream>
using namespace cimg_library;   //Necesario

int main(int argc, char *argv[]) {
    //@ Carga una imagen y la subdimensiona y luego expande, para emular el fenomeno de submuestreo
    cimg_usage("Efectos de resolucion espacial por submuestreo");

    //Cargamos la imagen RMN

    // const int ejercicio = cimg_option("-e", 1, "Exercise Number");
    CImg<unsigned char> img_original("../images/rmn.jpg");
    unsigned int h = img_original.height();
    unsigned int w = img_original.width();

    //Redimensionamos a la mitad y luego expandemos de nuevo
    CImg<unsigned char> img_resultado1 = img_original.get_resize( w/2, h/2 ).get_resize(w,h);
    CImg<unsigned char> img_resultado2 = img_original.get_resize( w/4, h/4 ).get_resize(w,h);
    CImg<unsigned char> img_resultado3 = img_original.get_resize( w/8, h/8 ).get_resize(w,h);

    CImgList<unsigned char> lista;
    lista.assign(img_original, img_resultado1, img_resultado2, img_resultado3);
    lista.display("Lista de imagenes con diferente resolucion espacial");

    return 0;
}
