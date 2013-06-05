#ifndef _MYPDI_FUNCTIONS_
#define _MYPDI_FUNCTIONS_

#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <string>
#include <queue>
#include "PDI_functions.h"   

using namespace cimg_library;   //Necesario
using namespace std;   //Necesario


template<typename T>
bool esta_adentro( T valor,  T min,  T max) {
	if (min > max) {
		T tempay = min;
		min = max;
		max = tempay;
	}	
	return (valor < max) && (valor > min || fabs(valor - min) < 0.00001 );
}


template<typename T>
CImg<T> filtro_mediana(CImg<T> & imagen, unsigned int talla = 3) {
	assert(imagen.spectrum() == 1);
	CImg<T> retval = imagen;

	cimg_forXY(imagen, x, y) {
		CImg<T> vecindad = imagen.get_crop(x-talla/2, y-talla/2, x+talla/2, y+talla/2);
		vecindad.sort();
		retval(x,y) = vecindad(talla/2, talla/2);
	}
	return retval;
}

template<typename T>
bool esColor(CImg<T> pixel, T c1min, T c1max, T c2min, T c2max, T c3min, T c3max) {
	assert(pixel.spectrum() == 3);
	if (
		 esta_adentro(pixel(0,0,0,0), c1min, c1max) &&
		 esta_adentro(pixel(0,0,0,1), c2min, c2max) &&
		 esta_adentro(pixel(0,0,0,2), c3min, c3max)
		)
		return true;
	else
		return false;
}

//***** bool es_deseada *****
// Dado un píxel de referencia y uno a evaluar
// los compara independientemente de la cantidad de canales que tenga
// en base a un umbral delta_local

// ---------Entradas------
// v_evaluar		: pixel a evaluar. Se puede obtener con imagen.get_crop(x,x,y,y)
// v_referencia    :  pixel de referencia
// delta_local : umbral de variacion admisible pixel a pixel canal a canal
// ---------Salidas------
// verdadero si cumple con la condición
template<typename T>
bool es_deseada(CImg<T> v_evaluar, CImg<T> v_referencia, T delta_local) {
	assert( v_referencia.size() == v_evaluar.size() && v_referencia.spectrum() == v_evaluar.spectrum() );
	bool deseada = true;
	cimg_forC(v_evaluar, c) {
		T rango_superior = v_referencia(0,0,0,c) + delta_local;
		if (rango_superior > 255)
			rango_superior = 255; 
		
		T rango_inferior = v_referencia(0,0,0,c) - delta_local;
		if (rango_inferior < 0)
			rango_inferior = 0; 

		deseada = deseada && esta_adentro(v_evaluar(0,0,0,c), rango_inferior , rango_superior + 1 );
	}
	return deseada;
}


//***** Crecimiento de regiones para color *****
// Se fija en los 3 canales para decidir si un pixel pertenece 
// a la region inicializada por la semilla.

// ---------Entradas------
// imagen
// seed_x		: coordenada semilla
// seed_y	    : coordenada semilla
// delta_local : umbral de variacion admisible pixel a pixel canal a canal
// ---------Salidas------
// Devuelve una imagen con 3 valores
//	 0: pixel no pertenece a la region
//	 1: pixel que pertenece a la region
//  -1: pixel no procesado

template<typename T>
CImg<int> crecimientoRegiones(CImg<T> imagen,  int seed_x,  int seed_y, T delta_local , unsigned int & pixeles_region) {
	pixeles_region = 0;
	//Imagen temporal para saber que pixeles han sido procesados
	CImg<int> retval(imagen.width(), imagen.height(), 1, 1, -1 );

	//Cola a procesar
	std::queue< std::vector< int> > a_procesar;
	std::vector<int> coordtemp; coordtemp.push_back(seed_x); coordtemp.push_back(seed_y);
	a_procesar.push( coordtemp );

	//El primer pixel, pertenece a la region
	retval(seed_x,seed_y) = 1;
	pixeles_region++;

	while(! a_procesar.empty() ) {
		//Guarda el primero de la cola
		std::vector< int > procesando = a_procesar.front();
		//Borra el primero de la cola
		a_procesar.pop();
		//Recorremos el vecindario de 8 vecinos a la cola
		for ( int i = -1; i < 2; i++) {
			for ( int j = -1; j < 2; j++) {
				if (
					esta_adentro(procesando[0] + i, 0 , imagen.width() ) && //en rango x
					esta_adentro(procesando[1] + j, 0 , imagen.height() ) &&  //en rango y
					retval(procesando[0] + i, procesando[1] + j) == -1 )  {  //no procesado
						//caso esta en rango y No ha sido procesada
						if ( es_deseada(
								imagen.get_crop(
									procesando[0] + i, procesando[1] + j, procesando[0] + i, procesando[1] + j
								), 
								imagen.get_crop(
									procesando[0], procesando[1], procesando[0], procesando[1]
									),
									delta_local
								) 
							) {

							retval(procesando[0] + i, procesando[1] + j) = 1;
							pixeles_region++;
							coordtemp.clear(); coordtemp.push_back(procesando[0] + i); coordtemp.push_back( procesando[1] + j);
							a_procesar.push(coordtemp);
						} else {
							retval(procesando[0] + i, procesando[1] + j) = 0;
						}
					}
			}
		}
	}
	return retval;
}

template <typename T>
unsigned int contarValor(CImg<T> imagen, T valor) {
	assert(imagen.spectrum() == 1);
	unsigned int contador = 0;
	cimg_forXY(imagen, x, y) {
		if (fabs(valor-imagen(x,y)) < 0.00001) {
			contador++;
		}
	}
	return contador;
}


template<typename T>
std::vector<unsigned int> coordHoughToImg(CImg<T> imagen, unsigned int _tita, unsigned int _rho) {
    unsigned int M = imagen.width();
    unsigned int N = imagen.height();

    double step_tita = M_PI/double(M-1.0);
    double diagonal = std::sqrt(std::pow(M-1,2) + std::pow(N-1,2));
    double step_rho = 2.0*diagonal/double(N-1);
    //Actualiza tita y rho a valores reales
    double tita = double(_tita)*step_tita - M_PI/2.0; 
    double rho  = double(_rho)*step_rho - diagonal;

    unsigned int x0;
    unsigned int x1;
    unsigned int y0;
    unsigned int y1;
    
    if (tita>-M_PI/2 && tita<M_PI/2){
        y0 = 0; 
        y1 = M-1;
        x0 = rho/cos(tita);      // calcula y para y=0
        x1 = rho/cos(tita)-(M-1)*tan(tita); // calcula y para y=M-1	  
    }else{
        x0 = 0; 
        x1 = N-1;
        y0 = rho/sin(tita);      // calcula y para x=0
        y1 = rho/sin(tita)-(N-1)/tan(tita); // calcula y para x=M-1
    }
    unsigned int ret_val[4] = {y0,x0,y1,x1};
    std::vector<unsigned int> retval;
    retval.assign(&ret_val[0], &ret_val[0]+4);
    return retval;
}



CImg<bool> getMascaraMayores(CImg<double> & imagen, double valor_min ) {

	CImg<double> retval(imagen.width(), imagen.height(), 1, 1, 0);

	cimg_forXY(imagen, x, y) {
		if (imagen(x,y) > valor_min) {
			retval(x,y) = 1;
		}
	}

	return retval;

}

CImg<double> aplicarMascara(CImg<double> & imagen, CImg<bool> &mascara) {
	CImg<bool> tmascara = mascara.get_resize(imagen.width(), imagen.height() );
	CImg<double> retval(imagen.width(), imagen.height(), 1, 1, 0);
	cimg_forXY(imagen, x ,y) {
		if (mascara(x,y))
			retval(x,y) = imagen(x,y);
	}
	return retval;
}

void getPuntosCardinales(CImg<double> img, std::vector<unsigned int> & cardinalidades, double umbral = 0.1 ) {
	cardinalidades.clear();
	img.normalize(0,1);

	unsigned int W = img.width();
	unsigned int H = img.height();

	int i = 0;
	int j = 0;

	//Encontramos el Norte
	for (j = 0; j < H; ++j) {
		for (i = 0; i < W; ++i) {
			if (img(i,j) > umbral) {
					if (img(i,j) > umbral); cout<<img(i,j)<<"\n";
				break;
		}
		}
	}
	cardinalidades.push_back(i);
	cardinalidades.push_back(j);

	//Encontramos el Sur
	for (j = H; j >= 0; --j) {
		for (i = 0; i < W; ++i) {
			if (img(i,j) > umbral) {
					if (img(i,j) > umbral); cout<<img(i,j)<<"\n";
				break;
		}
		}
	}
	cardinalidades.push_back(i);
	cardinalidades.push_back(j);

	//Encontramos el Este
	for (i = W; i >= 0; --i) {
		for (j = 0; j < H; ++j) {
			if (img(i,j) > umbral) {
					if (img(i,j) > umbral); cout<<img(i,j)<<"\n";
				break;
		}
		}
	}
	cardinalidades.push_back(i);
	cardinalidades.push_back(j);

	//Encontramos el OEste
	for (i = 0; i < W; ++i) {
		for (j = 0; j < H; ++j) {
			if (img(i,j) > umbral) {
					if (img(i,j) > umbral); cout<<img(i,j)<<"\n";
				break;
		}
		}
	}
	cardinalidades.push_back(i);
	cardinalidades.push_back(j);

}



template<typename T> 
struct maximo {
  unsigned int x;
  unsigned int y;
  T value;  
};

template<typename T> 
bool comparar_maximo(maximo<T> a, maximo<T> b) {
	if (a.value > b.value) //porque quiero ordenar de mayor a menor
		return true;
	else
		return false;
}




template<typename T> 
std::vector<maximo<T> > getNMax(CImg<T> imagen, unsigned int cantidad_maxima ) {

	std::vector<maximo<T> > tempy;

	cimg_forXY(imagen, x, y) {
		maximo<T> m;
		m.x = x;
		m.y = y;
		m.value = imagen(x,y);
		tempy.push_back(m);
	}

	sort( tempy.begin(), tempy.end() , comparar_maximo<T>);
	tempy.erase(tempy.begin() + cantidad_maxima); //borra desde begin + cantidad_maxima hasta el final

	return tempy;
}





#endif
