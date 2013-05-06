#include <CImg.h>               //include basico de CImg
#include <iostream>
#include "../utils/genArchivoMascara.cpp"

using namespace cimg_library;   //Necesario
using namespace std;

int main(int argc, char *argv[]) {
    //@ Leer filtro, aplicar filtro, convolve con filtro de distinta medida. Filtra segun un umbral

    const char* _input = cimg_option("-i", "../images/hubble.tif", "Input Image File");
    const char* _filter = cimg_option("-m", "filtro_ej3.txt", "Input filter File");
    const unsigned int _lado = cimg_option("-l", 5, "Input filter File");
    const unsigned int _umbral = cimg_option("-u", 150, "Input filter File");

    CImg<unsigned char> img(_input), output, output_grises(img.width(), img.height(), 1 , 1 , 0), 
                            img_binaria(img.width(), img.height(), 1 , 1 , 0);

    //Creamos el filtro de promediado
    utils::genArchivoMascara(_filter, _lado, _lado);
    CImg<double> filtro = utils::get_filtro(_filter);

    //Convolucionamos
    output = img.get_convolve(filtro);

    //Binarizamos la imagen a partir del umbral definido
    cimg_forXY(output, x , y) {
        if (output(x,y) > _umbral ) {
            img_binaria(x,y) = 255;
            output_grises(x,y) = img(x,y);
        }
    }

    //Dibujamos
    CImgList<double> lista;
    lista.assign(img, output, img_binaria, output_grises );
    lista.display();
}
