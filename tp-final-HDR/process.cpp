#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <complex>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include "getSeeds.cpp"

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
	std::ofstream salida("output.txt", std::ios::app);

	const clock_t begin_time = clock();
	
	//Leer sobre que ejemplo se va a trabajar
	const char* _ejemplo = cimg_option("-e", "cave", "Carpeta de Ejemplos");
	//Leer sobre que formato se va a trabajar
	const char* _format = cimg_option("-f", "jpg", "Formato de Ejemplos");

	salida<<_ejemplo<<",";
	//Obtener las imágenes de la carpeta
	std::vector<std::string> imagenes_a_leer = getSeeds(_ejemplo, _format);

	//Recorremos las imágenes leidas y las agregamos a una lista
	//En caso de no existir alguna (faltante), la generaremos
	unsigned int _n = imagenes_a_leer.size();

	for(unsigned int i = 0; i < _n; i++) {
		if (imagenes_a_leer[i].compare("NOT_FOUND") == 0) { //Bandera para identificar que no existe la imagen y hay que generarla
			// COMING SOON
		} else {
			CImg<double> img_tmp(imagenes_a_leer[i].c_str());
			
			if (leer_primera) {
				W = img_tmp.width();
				H = img_tmp.height();
				C = img_tmp.spectrum();
				salida<<W<<","<<H<<","<<C<<",";
				leer_primera = false;
			}

			lista.push_back( img_tmp.resize(W, H) );
		}
	}

	//Declaramos la imagen compuesta
	CImg<double> img_compuesta(2*W, 2*H, 1, C, 0 );


	for (unsigned int x = 0; x < W; x++) {
		for (unsigned int y = 0; y < H; y++ ) {
			for (unsigned int c = 0; c < C; c++ ) {
				img_compuesta(x*2      ,  y*2      ,0,c) = lista[3](x,y,0,c);
				img_compuesta(x*2      ,  y*2 + 1  ,0,c) = lista[2](x,y,0,c);
				img_compuesta(x*2 + 1  ,  y*2 + 1  ,0,c) = lista[1](x,y,0,c);
				img_compuesta(x*2 + 1  ,  y*2      ,0,c) = lista[0](x,y,0,c);
			}
		}

	}
	
	img_compuesta.save("test.bmp");

    salida<<float( clock () - begin_time ) /  CLOCKS_PER_SEC<<"\n";
}
