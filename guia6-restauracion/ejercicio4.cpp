#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include "../utils/fourier.cpp"
#include "../utils/PDI_functions.h"
using namespace cimg_library;   //Necesario

bool dentroCirculo(int x, int y, int x0, int y0, unsigned int radio) {
    return std::pow(x - x0, 2) + std::pow(y - y0, 2) - radio*radio < 0;
}

//@ Realiza unna mascara notch para un vector de centros y radios
CImg<double> mascaraNotch(CImg<double> &img, std::vector<std::pair<unsigned int, unsigned int> > centros, std::vector<unsigned int> radios) {
    CImg<double> ret_val(img.width(), img.height(), 1, 1, true);
    cimg_forXY(img,x,y) {
        for (unsigned int i = 0; i < centros.size(); i++) {
            if (dentroCirculo(int(x),int(y), int(centros[i].first), int(centros[i].second), radios[i])) {
                ret_val(x,y) = false;
            }
        }
    }
    return ret_val;
}


//@ Genera una máscara con un filtro rechazabanda entre las frecuencias f0 y ff
CImg<double> mascaraRechazaBanda(CImg<double> &img, unsigned int f0, unsigned int ff ) {
    CImg<double> ret_val(img.width(), img.height(), 1,1,1);

	cimg_forXY(img, x, y) {
		bool dentro_grande = dentroCirculo((int) x, (int) y, img.width()/2, img.height()/2, ff );
		bool dentro_chico = !dentroCirculo((int) x, (int) y, img.width()/2, img.height()/2, f0 );

		if (dentro_grande && dentro_chico)
			ret_val(x,y) = 0;
	}
	return ret_val;
}

//@ Lee los centros de circulos desde un archivo
std::vector<std::pair<unsigned int, unsigned int> > leerCentros(std::string file) {
    ifstream f (file.c_str());
    if (!f.is_open()) {
        std::cout<<"No pudo abrir el archivo "<<file<<"\n";
        exit(-1);
    }
    unsigned int cantidad;
    f>>cantidad;
    std::vector<std::pair<unsigned int, unsigned int> > ret_val;
    for (unsigned int i = 0; i < cantidad; i++) {
        unsigned int x;
        unsigned int y;
        f>>x>>y;
        ret_val.push_back(std::make_pair<unsigned int, unsigned int>(x,y));
    }
    f.close();
    return ret_val;
}

//@ Lee los radios de los circulos desde un archivo
std::vector<unsigned int> leerRadios(std::string file) {
    ifstream f (file.c_str());
    if (!f.is_open()) {
        std::cout<<"No pudo abrir el archivo "<<file<<"\n";
        exit(-1);
    }
    unsigned int cantidad;
    f>>cantidad;
    std::vector<unsigned int> ret_val;
    for (unsigned int i = 0; i < cantidad; i++) {
        unsigned int r;
        f>>r;
        ret_val.push_back(r);
    }
    f.close();
    return ret_val;
}

int main(int argc, char *argv[]) {
    //@ Aplica filtro notch pero no anda 
	const char* input = cimg_option("-i", "../images/img_degradada.tif", "Input Image File");
	const char* input2 = cimg_option("-i", "../images/img.tif", "Input Image File");
	const char* f_centros = cimg_option("-c", "centros_ej4_1.txt", "Input Image File");
	const char* f_radios = cimg_option("-r", "radios_ej4_1.txt", "Input Image File");

    CImg<double> img(input);
    CImg<double> img_original(input2);
    unsigned int w = img.width();
    unsigned int h = img.height();
    CImgList<double> f_img = img.get_FFT();

    CImg<double> rbanda;

    rbanda = mascaraRechazaBanda(img, 10, 20);
    rbanda.display();
    
    CImg<double> magn = magn_tdf(img);
    CImg<double> magn2 = utils::get_magnitud(f_img, false).get_log().get_normalize(0,255);

    std::vector<unsigned int> radios = leerRadios(f_radios);
    std::vector<std::pair<unsigned int, unsigned int> > centros = leerCentros(f_centros);

    CImg<double> mascara_notch = mascaraNotch(img, centros, radios);
    mascara_notch.shift(w/2, h/2, 0, 0, 2);
  
    CImgList<double> f_resultado(2,w,h,1,1,0.0);
    
    cimg_forXY(f_img[0],x,y) {
        if (mascara_notch(x,y))
            f_resultado[0](x,y) = f_img[0](x,y);
    }

    CImg<double> resultado = f_resultado.get_FFT(true)[0];
    //resultado(0,0) = 0.0; //fix para display
    
    //CImg<double> magn3 = utils::get_magnitud(f_resultado, false).get_normalize(0,255);
    //magn3(w/2,h/2) = 0.0;
    
    std::cerr<<"Error cuadratico medio = "<<resultado.MSE(img_original)<<"\n";
    (img, mascara_notch, resultado, img_original).display();
    return 0;
}
