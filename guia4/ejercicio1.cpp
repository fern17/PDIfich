#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/getValue.cpp"
using namespace cimg_library;   //Necesario


int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");
    
    const char* input = cimg_option("-i", "../images/patron.tif", "Input Image File");
    
    CImg<double> img(input), generada, img_hsi, generada_rgb, r, g, b, h, s, i, rr, gg, bb;

    	
    r = img.get_channel(0);
    g = img.get_channel(1);
    b = img.get_channel(2);
    img_hsi = img.get_RGBtoHSI();

    h = img_hsi.get_channel(0);
    s = img_hsi.get_channel(1);
    i = img_hsi.get_channel(2);

    CImgList<float> lista;
    lista.assign(img,img_hsi, r,g,b, h, s ,i );
    //lista.display();


    //ejercicio b
	h.mirror('x');
    generada = img_hsi;	
	cimg_forXY(generada, x  , y) {
        generada(x,y,0,0) = h(x,y);
		generada(x,y,0,1) = 1;
		generada(x,y,0,2) = 1;
	}

    h = generada.get_channel(0);
    s = generada.get_channel(1);
    i = generada.get_channel(2);

    generada_rgb = generada.get_HSItoRGB();
    rr = generada_rgb.get_channel(0);
    gg = generada_rgb.get_channel(1);
    bb = generada_rgb.get_channel(2);

    CImgList<double> listab;
    listab.assign(generada, rr, gg, bb, h , s , i);

    listab.display();

    return 0;
}
