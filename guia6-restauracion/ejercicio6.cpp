#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <utility>
#include "../utils/PDI_functions.h"
using namespace cimg_library;   //Necesario
using namespace std;   //Necesario

#define DEBUG false

// 1) No son adecuados ya que no cumplen con la propiedad de aplicabilidad tanto para escalares como para vectores.
//@ Funcion (aux) que dado un vecindario nos devuelve la menor distancia entre c/u de los puntos
pair<unsigned int, unsigned int> get_index_menor_distancia(CImg<double> &vecindario, unsigned int tipo_distancia = 0) {

	CImg<double> tempy = vecindario;

	double acumulador;
    //Para cada uno de los elementos del vecindario
	cimg_forXY(vecindario, x, y) {
		acumulador = 0.0;
        //Comparamos contra todos los demas
		cimg_forXY(vecindario, i, j) {
            //Recorriendo cada canal
			cimg_forC(vecindario, c ) {
                double delta = vecindario(x,y,0,c) - vecindario(i,j,0,c);
                
                if (tipo_distancia == 0) {
				    //Manhattan
				    acumulador += fabs(delta);
                }
                else if (tipo_distancia == 1) {
                    //Euclidea
                    acumulador += std::pow(delta, 2);
                }
			}
		}
        if (tipo_distancia == 1) {
            //Si la distancia es euclidea, sacamos raiz cuadrada
            acumulador = std::sqrt(acumulador);
        }
		tempy(x,y) = acumulador;
        
    }

	unsigned int retx = 0;
	unsigned int rety = 0;
	acumulador = tempy(retx,rety);
    //Ahora buscamos el valor menor
	cimg_forXY(tempy,x,y) {
		if (tempy(x,y) < acumulador) {
			retx = x;
			rety = y;
			acumulador = tempy(retx,rety);
		}
	}
    // y retornamos un pair de la posicion
	return std::make_pair(retx, rety);
}

//@ Función que calcula como un filtro de mediana para espacios de mas de 1D por ejemplo para aplicar sobre imágenes a color.
CImg<double> filtroMedianaGeneralizado(CImg<double> &img, unsigned int w_vecindario = 3, unsigned int h_vecindario = 3, unsigned int tipo_distancia = 0) {
	
	CImg<double> ret_val(img.width(), img.height(), img.depth(), img.spectrum(), 0);

	cimg_forXY(img,x,y) {
		//Obtener vecindario
		CImg<double> vecindario = img.get_crop(x-w_vecindario/2, y-h_vecindario/2, x+w_vecindario/2, y+h_vecindario/2);

		//Obtener la menor distancia
		pair<unsigned int, unsigned int> coordenadas_distancia = get_index_menor_distancia(vecindario, tipo_distancia);

        //Copiamos el valor de la menor distancia
        cimg_forC(vecindario, c) { 
		    ret_val(x,y,0,c) = vecindario(coordenadas_distancia.first, coordenadas_distancia.second, 0, c);
        }
	}

    return ret_val;	
}

int main(int argc, char *argv[]) {
    //@ Aplica el filtro de mediana generalizada para imagenes color  
	const char* input = cimg_option("-i", "../images/sangre.jpg", "Input Image File");
    const unsigned int ancho = cimg_option("-w", 3, "Ancho de la mascara");
    const unsigned int alto = cimg_option("-a", 3, "Alto de la mascara");
    const double desvio_impulsivo = cimg_option("-s", 20.0, "Desvio sal y pimienta");
    const double desvio_gaussiano = cimg_option("-g", 20.0, "Desvio Gaussiano");

    //Cargamos la imagen
    CImg<double> img(input);

    //Le agregamos ruido
    CImg<double> img_ruidosa = img;
    img_ruidosa.noise(desvio_impulsivo, 2);
    //img_ruidosa.noise(desvio_gaussiano, 0);

    //Aplicamos el filtro
    CImg<double> resultado_manhattan = filtroMedianaGeneralizado(img_ruidosa, ancho, alto, 0);
    CImg<double> resultado_euclidea = filtroMedianaGeneralizado(img_ruidosa, ancho, alto, 1);

    //Mostramos los errores
    std::cout<<"\nErrores:\n";
    std::cout<<"\nOriginal con ruido = "<<img_ruidosa.MSE(img);
    std::cerr<<"\nOriginal con restaurada manhattan = "<<resultado_manhattan.MSE(img);
    std::cerr<<"\nOriginal con restaurada euclidea = "<<resultado_euclidea.MSE(img);
    std::cerr<<"\n";

    //Dibujamos
    (img, img_ruidosa, resultado_manhattan, resultado_euclidea).display();


    return 0;
}
