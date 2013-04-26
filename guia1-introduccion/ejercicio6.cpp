#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
using namespace cimg_library;   //Necesario

int main(int argc, char *argv[]) {
    //@ Carga una imagen y la cuantiza con niveles de 256,128,64,etc..
    cimg_usage("Efecto del cuantizado");

    CImg<unsigned char> img_original("../images/huang2.jpg");
    unsigned int h = img_original.height();
    unsigned int w = img_original.width();


    CImgList<unsigned char> lista;
    lista.push_back(img_original);

    for (unsigned int i = 1 ; i < 9; i++) {
        //Calcula la cantidad de niveles
        unsigned int cantidad_niveles = pow(2,9-i);
        //Cuantiza y normaliza
        CImg <unsigned char> tmp = img_original.get_quantize(cantidad_niveles).get_normalize(0,255);
        //Agrega a la lista
        lista.push_back(tmp);
    }
    
    lista.display("Efecto del cuantizado");

    return 0;
}
