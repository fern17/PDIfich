#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <complex>
#include <ctime>
#include <vector>
#include <string>

#include "../utils/PDI_functions.h"
#include "../utils/fourier.cpp"
#include "../utils/genArchivoMascara.cpp"
using namespace cimg_library;   //Necesario


//Lista de imágenes con la cual se trabajará
CImgList<double> lista;

//Ancho de las imagenes a trabajar
unsigned int W;

//Alto de las imagenes a trabajar
unsigned int H;

//Canales de las imagenes a trabajar
unsigned int C;

//Bandera
bool leer_primera = true;

int main(int argc, char *argv[]) {
	//Leer sobre que ejemplo se va a trabajar
	const char* _ejemplo = cimg_option("-e", "cave", "Carpeta de Ejemplos");

	//Obtener las imágenes de la carpeta
	std::vector<std::string> imagenes_a_leer = getSeeds(_ejemplo);

	//Recorremos las imágenes leidas y las agregamos a una lista
	//En caso de no existir alguna (faltante), la generaremos
	unsigned int _n = imagenes_a_leer.size();

	for(unsigned int i = 0; i < _n; i++) {
		if (imagenes_a_leer[i] == -1) { //Bandera para identificar que no existe la imagen y hay que generarla

		} else {
			CImg<double> img_tmp(imagenes_a_leer[i]);
			
			if (leer_primera) {
				W = img_tmp.width();
				H = img_tmp.height();
				C = img_tmp.spectrum();
				leer_primera = false;
			}

			lista.push_back( img_tmp.resize(W, H) );
		}
	}

	//Declaramos la imagen compuesta
	CImg<double> img_compuesta(2*W, 2*H, 1, C, 0 );


	for (unsigned int x = 0; x < W; x++) {
		for (unsigned int y = 0; y < H; y++ ) {
			img_compuesta(x*2,y*2) = lista[0](x,y);
			img_compuesta(x*2,y*2+1) = lista[1](x,y);
			img_compuesta(x*2+1,y*2+1) = lista[2](x,y);
			img_compuesta(x*2+1,y*2) = lista[3](x,y);
		}

	}
	

	// cimg_forXY(img_compuesta, x ,y ) {

	// 	unsigned int modx = x%2;
	// 	unsigned int mody = y%2;

	// 	img_compuesta(x,y) = 

	// }  

}