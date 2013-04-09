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

std::vector< CImg<bool> > generarMascara(std::string nombre, unsigned int w, unsigned int h) {
    std::ifstream f(nombre.c_str());
    if (!f.is_open()) {
        std::cout<<"No se pudo abrir el archivo "<<nombre<<"\n";
        exit(-1);
    }
    unsigned int x0;
    unsigned int y0;
    unsigned int x1;
    unsigned int y1;

    std::vector<CImg<bool> > salida;

    while(f>>x0>>y0>>x1>>y1) {
        CImg<bool> mask = mascaraRectangular(w,h,x0,y0,x1,y1);
        salida.push_back(mask);
    }
    f.close();
    return salida;
}

int main(int argc, char *argv[]) {
    double epsilon = 0.1;
    cimg_usage("Utilizacion de la libreria CImg");
    
    const char* completo = cimg_option("-r", "../images/blister_completo.jpg", "Input Image File");
    const char* test = cimg_option("-t", "../images/blister_incompleto.jpg", "Input Image File");
    const char* maskfile = cimg_option("-m", "blister.txt", "Input Image File");

    //Leemos las imagenes
    CImg<unsigned char> img_completo(completo);
    CImg<unsigned char> img_test(test);

    img_test.resize(img_completo);

    //Leemos las mascaras y las guardamos en un vector<CImg<bool> >
    std::vector<CImg<bool> > mascaras_bool;
    mascaras_bool = generarMascara(maskfile, img_completo.width(), img_completo.height() );

    unsigned int n = mascaras_bool.size();

    for (unsigned int i = 0 ; i < n ; i ++ ) {
        //Para cada mascara
        CImg<unsigned char> mascara_completo = aplicarMascara(img_completo, mascaras_bool[i]);
        CImg<unsigned char> mascara_test = aplicarMascara(img_test, mascaras_bool[i]);

        double ECM1 = mascara_completo.MSE(mascara_test);
        std::cout<<"\n"<<"El ECM en "<<i+1<<" es:"<<ECM1;

        if (ECM1 > 150) {
            std::cout<<"\t*** Falta la pastilla "<<i+1;
        }
    }
    std::cout<<"\n\n";

}