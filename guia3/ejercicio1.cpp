#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
using namespace cimg_library;   //Necesario


int main(int argc, char *argv[]) {
    //@ histograma, ecualización, crear y graficar histograma

    const char* _input = cimg_option("-r", "../images/blister_completo.jpg", "Input Image File");
    CImg<unsigned char> input(_input) ,output, hinput, houtput;

    //Histograma de la entrada
    hinput = input.get_histogram(256);
    //Ecualizamos la imágen
    output = input.get_equalize(256,0,255);
    //Histograma de la salida
    houtput = output.get_histogram(256);

    CImgList<unsigned char> lista;
    lista.push_back(input);
    lista.push_back(output);

    lista.display();

    hinput.display_graph("Histograma de la Entrada",3);
    houtput.display_graph("Histograma de la Salida",3);


}