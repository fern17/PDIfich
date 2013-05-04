#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/genArchivoMascara.cpp"

using namespace cimg_library;   //Necesario
using namespace std;

int main(int argc, char *argv[]) {
    //@ Leer filtro, aplicar filtro, convolve con filtro de distinta medida

    const char* _input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* _filterp = cimg_option("-m", "filtro_ej4p.txt", "Input filter File");
    const char* _filterg = cimg_option("-n", "filtro_ej4g.txt", "Input filter File");
    const unsigned int _lado = cimg_option("-l", 5, "Input filter File");
    const float _sigma = cimg_option("-s", 2, "Input filter File");

    CImg<double> img(_input), output_prom, output_gaussiano;

    //Creamos el filtro de promediado
    utils::genArchivoMascara(_filterp, _lado, _lado);
    CImg<double> filtro_prom = utils::get_filtro(_filterp);
    
    //Creamos el filtro de Gaussiana
    utils::genArchivoMascaraGaussiana(_filterg, _lado, _lado, _sigma);
    CImg<double> filtro_gaussiano = utils::get_filtro(_filterg);

    //Aplicamos los filtros
    output_prom = img.get_convolve(filtro_prom);
    output_gaussiano = img.get_convolve(filtro_gaussiano);

    CImgList<double> lista;
    lista.assign(img, output_prom, output_gaussiano);
    lista.display();
}
