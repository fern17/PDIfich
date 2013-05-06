#include <CImg.h>               //include basico de CImg
#include <iostream>
#include "../utils/genArchivoMascara.cpp"

using namespace cimg_library;   //Necesario

int main(int argc, char *argv[]) {
    //@ Leer filtro acentuado, aplicar filtro acentuado, convolve con filtro de distinta medida

    const char* _input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* _filter = cimg_option("-m", "filtro_ej5.txt", "Input filter File");

    CImg<double> img(_input), output(img.width(), img.height(), 1, 1, 0);

    //Leemos el filtro de acentuado
    CImg<double> filtro = utils::get_filtro(_filter);

    //aplicamos el filtro
    output = img.get_convolve(filtro);

    CImgList<unsigned char> lista;
    lista.assign(img, output.normalize(0,255) );
    lista.display();
}
