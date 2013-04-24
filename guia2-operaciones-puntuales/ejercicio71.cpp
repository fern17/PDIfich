#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/getValue.cpp"
using namespace cimg_library;   //Necesario


CImg<unsigned char> imagenMapeo(std::vector<unsigned int> &LUT) {
    unsigned int ancho = LUT.size();
    CImg<unsigned char> mapeo(ancho, ancho,1,1,0);
    for (unsigned int i = 0; i < ancho; i++) {
        unsigned int valor = LUT[i];
        mapeo(i, ancho - valor - 1) = 255;
    }
    return mapeo;
}

CImg<unsigned char> aplicarTabla(CImg<unsigned char> img, std::vector<unsigned int> LUT) {
    CImg<unsigned char> resultado(img.width(), img.height());
    cimg_forXY(img, x,y) {
        resultado(x,y) = LUT[img(x,y)];
    }
    return resultado;
}



std::vector<unsigned int> logarithmicLUT(float c) {
    std::vector<unsigned int> LUT;
    LUT.resize(256,0);
    for (unsigned int i = 0; i < LUT.size(); i++) {
        LUT[i] = utils::getLogarithmicValue(i, c);
    }
    return LUT;
}

CImg<bool> mascaraRectangular(unsigned int w, unsigned int h,
							unsigned int x0, unsigned int y0, 
							unsigned int x1, unsigned int y1) {
	
	CImg<bool> salida(w,h,1,1,0);


	if (x0 + y0 > x1 + y1) {
		//Punto 1 esta mas cerca del origen => intercambio
		unsigned int tmp;
		tmp = x0;
		x0 = x1;
		x1 = tmp;

		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	for (unsigned int x = x0; x < x1 && x < w; x++ ) {
		for (unsigned int y=y0; y < y1 && y < h; y++) {
			salida(x,y) = true;
		}
	}
	return salida;

}

CImg<unsigned char> aplicarMascara(CImg<unsigned char> & img1, CImg<bool> & mascara) {
    assert( img1.is_sameXY(mascara) );

    CImg<unsigned char> salida(img1.width(), img1.height(), 1, 1, 0);

    cimg_forXY(img1,x,y) {
        salida(x,y) = ( img1(x,y) * (unsigned int) mascara(x,y) );
    }

    return salida;
}


int main(int argc, char *argv[]) {

    cimg_usage("Utilizacion de la libreria CImg");
    
    const char* input = cimg_option("-i", "../images/earth.bmp", "Input Image File");
    const float c = cimg_option("-c", 3.0, "Constant of logarithmicLUT");

    CImg<unsigned char> img(input);
    CImg<unsigned char> resultado;

    CImg<bool> mascara = mascaraRectangular(img.width() , img.height() , 437, 0, img.width() , 237);
    CImg<unsigned char> mascara_imagen = aplicarMascara(img , mascara);



    resultado = aplicarTabla(mascara_imagen, logarithmicLUT(c ));

    resultado = resultado + img;

    CImgList<unsigned char> lista;
    lista.assign(img, resultado);
    lista.display();

}