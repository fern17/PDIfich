#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <utility>
#include "../utils/PDI_functions.h"
using namespace cimg_library;   //Necesario
using namespace std;   //Necesario



// 1) No son adecuados ya que no cumplen con la propiedad de aplicabilidad tanto para escalares como para vectores.
//@ Funcion (aux) que dado un vecindario nos devuelve la menor distancia entre c/u de los puntos
pair<unsigned int, unsigned int> get_index_menor_distancia(CImg<double> &vecindario) {

	CImg<double> tempy = vecindario;


	double acumulador;
	cimg_forXY(vecindario, x, y) {
		acumulador = 0.0;
		cimg_forXY(vecindario, i, j) {
			cimg_forC(vecindario, c ) {
				//Manhattan
				acumulador += fabs(vecindario(x,y,c) - vecindario(i,j,c));
				
			}
		}
		tempy(x,y) = acumulador;

	}

	unsigned int retx = 0;
	unsigned int rety = 0;
	acumulador = tempy(retx,rety);

	cimg_forXY(tempy,x,y) {
		if (tempy(x,y) < acumulador) {
			retx = x;
			rety = y;
			acumulador = tempy(retx,rety);
			
		}
	}
	return std::make_pair(retx, rety);
}

//@ Función que calcula como un filtro de mediana para espacios de mas de 1D por ejemplo para aplicar sobre imágenes a color.
CImg<double> filtroMedianaGeneralizado(CImg<double> &img, unsigned int w_vecindario = 3, unsigned int h_vecindario = 3) {
	
	CImg<double> ret_val = img;

	cimg_forXY(img,x,y) {
		//Obtener vecindario
		CImg<double> vecindario = img.get_crop(x-w_vecindario/2, y-h_vecindario/2, x+w_vecindario/2, y+h_vecindario/2);

		//Obtener la menor distancia
		pair<unsigned int, unsigned int> coordenadas_distancia = get_index_menor_distancia(vecindario);

		ret_val(x,y) = vecindario(coordenadas_distancia.first, coordenadas_distancia.second);

	}

	return ret_val;	

}

