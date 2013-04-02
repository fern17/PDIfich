#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/getValue.cpp"
using namespace cimg_library;   //Necesario


CImg<bool> getBitPlane(CImg<unsigned char> & img1, int bit){
	CImg<bool> img(img1);
	img.fill(0);
	img = img1 & ( pow(2,bit) );
	return img;
}

float getECM(CImg<unsigned char> &img1, CImg<unsigned char> &img2) {
    assert(img1.is_sameXY(img2));

    float salida = 0;
	cimg_forXY(img1, x,y) {
        salida += pow(img1(x,y) - img2(x,y),2);
    }
    salida = salida / ( img1.width() * img1.height() );
    return salida;
}


int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");
    
    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* input2 = cimg_option("-j", "../images/clown.jpg", "Input2 Image File");
    const char* operation = cimg_option("-o", "and", "Operation Type");
    
    CImg<unsigned char> imagen(input);

    // std::vector<CImg<bool> > vector_bool;
    CImgDisplay ventana1, ventana2;


    CImgList<unsigned char> lista;

    for (unsigned int i = 0; i < 8; i++) {
    	CImg<unsigned char> plano;
    	plano = getBitPlane(imagen, i) * pow(2,i);
    	lista.push_back(plano);
    	std::cout<<"Bit plane #"<<i<<"\t ECM: "<<getECM(imagen, plano)<<"\t MSE: "<<imagen.MSE(plano)<<std::endl;
    }

    lista.display(ventana1, false);

    CImg<unsigned char> temporal = lista.at(7);

    CImgList<unsigned char> acumulado;
    acumulado.push_back(temporal);
    for (int i = 6; i >= 0; i--) {
    	temporal = temporal + lista.at(i);
    	acumulado.push_back(temporal);
    	std::cout<<"Acumulado bits #7-"<<i<<"\t ECM: "<<getECM(imagen, temporal)<<"\t MSE: "<<imagen.MSE(temporal)<<std::endl;

    }

    acumulado.display(ventana2, false);
}