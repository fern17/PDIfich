#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <queue>
using namespace cimg_library;   //Necesario


bool esta_adentro( int valor,  int min,  int max) {	
	return (valor < max) && (valor >= min);
}




//Función auxiliar que dado un vector de refencia compara con uno a evaluar
bool es_deseada(CImg<int> v_evaluar, CImg<int> v_referencia) {
	assert( v_referencia.size() == v_evaluar.size() && v_referencia.spectrum() == v_evaluar.spectrum() );

	 int delta_local = 200;

	bool deseada = true;

	cimg_forC(v_evaluar, c) {
		 int rango_superior = v_referencia(0,0,0,c) + delta_local;
		if (rango_superior > 255)
			rango_superior = 255; 
		
		 int rango_inferior = v_referencia(0,0,0,c) - delta_local;
		if (rango_inferior < 0)
			rango_inferior = 0; 
		deseada = deseada && esta_adentro(v_evaluar(0,0,0,c), rango_inferior , rango_superior );
		
	}
	return deseada;
}

CImg<int> crecimientoRegiones(CImg<int> & imagen,  int seed_x,  int seed_y) {
	
	//Imagen temporal para saber que pixeles han sido procesados
	CImg<int> retval(imagen.width(), imagen.height(), 1, 1, -1 );


	std::queue< std::vector< int> > a_procesar;
	//nose si se puede hacer esto
	a_procesar.push( std::vector< int>(seed_x, seed_y) );
	retval(seed_x, seed_y) = 1;

	while(! a_procesar.empty() ) {
		//Guarda el primero de la cola
		std::vector< int> procesando = a_procesar.front();
		//Borra el primero de la cola
		a_procesar.pop();
		//Recorremos el vecindario de 8 vecinos a la cola
		for ( int i = -1; i < 2; i++) {
			for ( int j = -1; j < 2; j++) {
				if (i == 0 && j == 0 )
					continue;

				std::cout<<procesando[0] + i<<" ";
				if (
					esta_adentro(procesando[0] + i, 0 , imagen.height() ) && //en rango x
					esta_adentro(procesando[1] + j, 0 , imagen.width() ) &&  //en rango y
					retval(procesando[0] + i, procesando[1] + j) == -1 )    //no procesado
					{
						if ( es_deseada(
								imagen.get_crop(
									procesando[0] + i, procesando[1] + j, procesando[0] + i, procesando[1] + j
								), 
								imagen.get_crop(
									procesando[0], procesando[1], procesando[0], procesando[1]
									)
								) 
							)
						{
							retval(procesando[0] + i, procesando[1] + j) = 1;
							a_procesar.push( std::vector< int>(procesando[0] + i, procesando[1] + j) );
						}else{
							retval(procesando[0] + i, procesando[1] + j) = 0;
						}
					}
			}
		}
		std::cout<<"\nAno a procesar:"<<a_procesar.size()<<"\n";

	}
	return retval;
}




int main(int argc, char *argv[]) {
    //@ Lee una imagen y le aplica los distintos operadores de derivada
 	const char* _input = cimg_option("-i", "../img/cervezas/01.jpg", "Input Image File");

 	CImg<int> imagen(_input);

 	// imagen.get_channel(0).get_histogram(256).normalize(0,1).display_graph("R",3);
 	// imagen.get_channel(1).get_histogram(256).normalize(0,1).display_graph("G",3);
 	// imagen.get_channel(2).get_histogram(256).normalize(0,1).display_graph("B",3);

 	// CImg<int> tempy = imagen.get_crop(100,100,100,100);


 	CImg<int> tempy = crecimientoRegiones(imagen, 277, 277);

 	(imagen,tempy).display();



    return 0;
}
