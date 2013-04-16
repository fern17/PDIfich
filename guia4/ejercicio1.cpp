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
    
    CImg<float> img(input), generada, img_hsi, r, g, b, h, s, i;

    	
    r = img.get_channel(0);
    g = img.get_channel(1);
    b = img.get_channel(2);
    img_hsi = img.get_RGBtoHSI();

    h = img_hsi.get_channel(0);
    i = img_hsi.get_channel(1);
    s = img_hsi.get_channel(2);

    CImgList<float> lista;
    lista.assign(img,img_hsi, r,g,b, h, s ,i );
    lista.display();


    //ejercicio b
	h.mirror('x');
	
	cimg_forXY(i, x  , y) {
		i(x,y) = 255;
		s(x,y) = 255;
	}
	i.display();

    generada.channel(0) = h;
    generada.channel(1) = s;
    generada.channel(2) = i;

    CImgList<float> listab;
    listab.assign(generada, h , s , i);

    listab.display();

    return 0;
}
