#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/PDI_functions.h"

using namespace cimg_library;   //Necesario


int main(int argc, char *argv[]) {

	const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
	const char* paleta = cimg_option("-p", "../paletas/gray.pal", "Input Image File");

	CImg<unsigned char> img_gris(256,256,1,1,0);

	//Creamos el degradé de grises
	for (unsigned int i = 0; i < 256; i++) {
		for (unsigned int j = 0; j < 256; j++) {
			img_gris(i,j) = i;
		}
	}

	CImg<double> mapeo, output(256, 256, 1, 3, 0);
	mapeo = cargar_paleta(paleta);


	CImg<double> R, G, B;
	R = mapeo.get_channel(0);
	G = mapeo.get_channel(1);
	B = mapeo.get_channel(2);

	//Recorre x y, y c CANALES
   cimg_forXYC(output, x, y, c) {
        output(x, y, c) = mapeo(0, img_gris(x, y), c);
    }


    //Parte B! (pobre union)
    CImg<unsigned char> img_prueba(input);
    CImg<double> output2(img_prueba.width(), img_prueba.height(), img_prueba.depth(), 3);

    cimg_forXYC(output2,x,y,c) {
        output2(x, y, c) = mapeo(0, img_prueba(x, y), c);

    }



	CImgList<double> lista;

	lista.assign(img_gris, output, img_prueba, output2);

	lista.display("Imágen orginal, Imagen el falso color", 0);



    return 0;
}
