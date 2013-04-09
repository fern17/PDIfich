#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
using namespace cimg_library;   //Necesario


int main(int argc, char *argv[]) {
    double epsilon = 0.1;
    cimg_usage("Utilizacion de la libreria CImg");
    
    const char* input = cimg_option("-r", "../images/blister_completo.jpg", "Input Image File");
    CImg<unsigned char> img(input);
    const unsigned char red[] = { 165,200,100 };

    CImg<unsigned char> histograma, visu(img.width(),img.height(),1,1,0);
    
    histograma = img.get_histogram(256);

	CImgList<unsigned char> lista;

	lista.push_back(img);
	lista.push_back(histograma.display_graph(0, 3));
	
	lista.push_back(img.get_equalize(256,0,255));
	lista.push_back(img.get_equalize(256,0,255).get_histogram(256,0,255).display_graph(0,3) );


	lista.display();


}