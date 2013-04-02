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
    
    const char* tipo1 = cimg_option("-r", "../images/a7v600-SE.gif", "Input Image File");
    const char* tipo2 = cimg_option("-s", "../images/a7v600-X.gif", "Input Image File");
    const char* test = cimg_option("-t", "../images/a7v600-SE(RImpulsivo).gif", "Input Image File");
    const char* maskfile = cimg_option("-m", "72mother.txt", "Input Image File");

    //Leemos las imagenes
    CImg<unsigned char> img_tipo1(tipo1);
    CImg<unsigned char> img_tipo2(tipo2);
    CImg<unsigned char> img_test(test);

    //Leemos las mascaras y las guardamos en un vector<CImg<bool> >
    std::vector<CImg<bool> > mascaras_bool;
    mascaras_bool = generarMascara(maskfile, img_tipo1.width(), img_tipo1.height() );

    unsigned int n = mascaras_bool.size();


    //Realizamos la votación en base al Error cuadratico medio de las máscaras definidas
    unsigned int votos1 = 0, votos2 = 0;

    for (unsigned int i = 0 ; i < n ; i ++ ) {
        //Para cada mascara
        CImg<unsigned char> mascara_tipo1 = aplicarMascara(img_tipo1, mascaras_bool[i]);
        CImg<unsigned char> mascara_tipo2 = aplicarMascara(img_tipo2, mascaras_bool[i]);
        CImg<unsigned char> mascara_test = aplicarMascara(img_test, mascaras_bool[i]);

        double ECM1 = mascara_tipo1.MSE(mascara_test);
        double ECM2 = mascara_tipo2.MSE(mascara_test);

        //Si son similares, no se vota
        if (fabs(ECM1-ECM2) < epsilon)
            continue;

        if (ECM1 < ECM2)
            votos1++;
        else
            votos2++;
    }
    std::cout<<"\nResultado de la Votacion\n\n";
    std::cout<<"Comparacion con: "<<test;
    std::cout<<"\nTipo 1:\t"<<votos1<<" ("<<tipo1<<")";
    std::cout<<"\nTipo 2:\t"<<votos2<<" ("<<tipo2<<")"<<std::endl;

}