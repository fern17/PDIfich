#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include "getSeeds.cpp"
//#include "getCubicKernel.cpp"
#include "cubicInterpolator.cpp"
#include "correccionGamma.cpp"
#include "transformadaLogaritmo.cpp"
#include "linealPotencia.cpp"

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



CImg<double> highBoosting(double A, CImg<double> & imagen, CImg<double> & mascara) {

    CImg<double> ret_val;
    cimg_forC(imagen, c) {
        CImg<double> canal = imagen.get_channel(c);
        ret_val.append((A-1)*canal + canal.get_convolve(mascara), 'c' );  
    }

    return ret_val;

}

//@ Cambia el canal de intesidad de una imagen RGB, y la devuelve en RGB
template <typename T>
CImg<T> cambiarIntesidad(CImg<T> imagen, CImg<T> intesidad) {
    if (imagen.spectrum() == 1) 
        return imagen;
    
    //Convertimos a HSI
    CImg<T> img = imagen.get_normalize(0,1).get_RGBtoHSI();
    
    //Separamos los 2 canales que nos interesan
    CImg<T> canal1 = img.get_channel(0);
    CImg<T> canal2 = img.get_channel(1);

    //Componemos
    CImg<T> ret_val;
    ret_val = canal1;
    ret_val.append(canal2, 'c');
    intesidad.normalize(0,1);
    ret_val.append(intesidad, 'c');

    return ret_val.HSItoRGB().normalize(0,255);
}



CImg<double> promediado(CImg<double> img_compuesta) {
    CImg<double> resultado(W,H,1,C);

    //Recorre la imagen y promedia los valores
    for (unsigned int x = 0; x < img_compuesta.width(); x+=2) {
        for (unsigned int y = 0; y < img_compuesta.height(); y+=2) {
            if (x % 533 == 0 && y % 499 == 0) std::cout<<x<<' '<<y<<'\n';
            
            CImg<double> vecindario = img_compuesta.get_crop(x,y,x+4,y+4);
            //Para cada canal
            CImg<double> img_promedios(4,1,1,C);
            //Para cada canal
            for (unsigned int c = 0; c < C; c++) {

                //Promedios por fila
                for (unsigned int i = 0; i < vecindario.height(); i++) {
                    double sum = 0.0;
                    //Para cada columna
                    for (unsigned int j = 0; j < vecindario.width(); j++) { 
                        sum += vecindario(i,j,0,c);
                    }
                    img_promedios(i,0,0,c) = sum/((double) vecindario.width());
                }
                double sum = 0.0;
                //Para cada una de las filas de los promedios
                for (unsigned int i = 0; i < img_promedios.width(); i++) {
                    sum += img_promedios(i,0,0,c);
                }
                double promedio_final = sum / ((double) img_promedios.width());
                resultado(x/2,y/2,0,c) = promedio_final;
            }
        }
    }
    return resultado;

}

//Realiza una interpolacion cubica en vecindarios de 4x4
CImg<double> interpolacion(CImg<double> img_compuesta, double dx = 0.5, double dy = 0.5) {
    CImg<double> resultado(W,H,1,C);

    for (unsigned int x = 0; x < img_compuesta.width(); x+=2) {
        for (unsigned int y = 0; y < img_compuesta.height(); y+=2) {
            if (x % 366 == 0 && y % 297 == 0) std::cout<<x<<' '<<y<<'\n';
            //Obtenemos un vecindario
            CImg<double> vecindario = img_compuesta.get_crop(x,y,x+4,y+4);
            
            //Interpolamos
            CImg<double> interpolada = cubicInterpolator(vecindario, dx, dy);
            
            //Copiamos el nuevo color al centro del vecindario
            for (unsigned int c = 0; c < C; c++) {
                resultado(x/2,y/2,0,c) = interpolada(0,0,0,c);
            }
       }
    }
    return resultado;
}


int main(int argc, char *argv[]) {
	std::ofstream salida("output.txt", std::ios::app);

	const clock_t begin_time = clock();
	
	//Leer sobre que ejemplo se va a trabajar
	const char* _ejemplo = cimg_option("-e", "cave", "Carpeta de Ejemplos");
	//Leer sobre que formato se va a trabajar
	const char* _format = cimg_option("-f", "jpg", "Formato de Ejemplos");
	//Cantidad de muestras en el kernel de convolucion cubico
    unsigned int _muestras = cimg_option("-m", 49 , "Cantidad de muestras en el kernel");

    //Para realizar la interpolacion, se debe elegir un paso (dx,dy) desde el centro del vecindario
    const double dx = cimg_option("-x", 0.5, "Paso en x en la interpolacion");
    const double dy = cimg_option("-y", 0.5, "Paso en y en la interpolacion");
    const double gamma = cimg_option("-g", 0.8, "Gamma de la correccion gamma");
    const double A_gamma = cimg_option("-a", 1.2, "Parametro c del logaritmo");
    const double c_log = cimg_option("-c", 0.8, "Parametro c del logaritmo");
    const double A_hb = cimg_option("-b", 5.0, "Parametro A del High Boost");
    const unsigned int transformada_mariano = cimg_option("-z", 1, "Cantidad de veces de aplicar la transformada mariano");

	salida<<_ejemplo<<",";
	//Obtener las imágenes de la carpeta
	std::vector<std::string> imagenes_a_leer = getSeeds(_ejemplo, _format);
    std::vector<bool> imagenes_existentes;

	//Recorremos las imágenes leidas y las agregamos a una lista
	//En caso de no existir alguna (faltante), la generaremos
	unsigned int _n = imagenes_a_leer.size();

	for(unsigned int i = 0; i < _n; i++) {
		if (imagenes_a_leer[i].compare("NOT_FOUND") == 0) { //Bandera para identificar que no existe la imagen y hay que generarla
            //Coming soon
            imagenes_existentes.push_back(false);
		} else {
            imagenes_existentes.push_back(true);
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

    lista.display();
	//Declaramos la imagen compuesta
	CImg<double> img_compuesta(2*W, 2*H, 1, C, 0 );


	for (unsigned int x = 0; x < W; x++) {
		for (unsigned int y = 0; y < H; y++ ) {
			for (unsigned int c = 0; c < C; c++ ) {
                //Asigna solo si la encuentra
				if (imagenes_existentes[3]) img_compuesta(x*2      ,  y*2      ,0,c) = lista[3](x,y,0,c);
				if (imagenes_existentes[2]) img_compuesta(x*2      ,  y*2 + 1  ,0,c) = lista[2](x,y,0,c);
				if (imagenes_existentes[1]) img_compuesta(x*2 + 1  ,  y*2 + 1  ,0,c) = lista[1](x,y,0,c);
				if (imagenes_existentes[0]) img_compuesta(x*2 + 1  ,  y*2      ,0,c) = lista[0](x,y,0,c);
			}
		}
	}

    //Calculamos los resultados con los dos metodos
    CImg<double> resultado_promedio = promediado(img_compuesta);
    CImg<double> resultado_interpolacion = interpolacion(img_compuesta, dx, dy);

    //resultado_promedio = correccionGamma(resultado_promedio, gamma, A_gamma);
    CImg<double> intensidad = resultado_promedio.get_RGBtoHSI().get_channel(2).get_equalize(256);
    CImg<double> promedio_ieq = cambiarIntesidad(resultado_promedio, intensidad );

    //resultado_interpolacion = correccionGamma(resultado_interpolacion, gamma, A_gamma);
    intensidad = resultado_interpolacion.get_RGBtoHSI().get_channel(2).get_equalize(256);
    CImg<double> interpolacion_ieq = cambiarIntesidad(resultado_interpolacion, intensidad );
    
    salida<<float( clock () - begin_time ) /  CLOCKS_PER_SEC<<"\n";

    //Aplica el High Boost
    CImg<double> mascara(3,3,1,1,-1);
    mascara(1,1) = 8;
    CImg<double> hb_prom = highBoosting(A_hb, promedio_ieq, mascara);
    CImg<double> hb_interp = highBoosting(A_hb, interpolacion_ieq, mascara);

    CImg<double> lineal_pot_prom = linealPotencia(promedio_ieq);
    CImg<double> lineal_pot_interp = linealPotencia(interpolacion_ieq);
    //Se aplica varias veces la transformada
    for (unsigned int i = 1; i < transformada_mariano; i++) {
        lineal_pot_prom = linealPotencia(lineal_pot_prom);
        lineal_pot_interp = linealPotencia(lineal_pot_interp);
    }
    //Aplica la transformacion lineal potencia
    
    //Draw Graph
    //lineal_pot_prom.get_RGBtoHSI().get_channel(2).get_equalize(256).display_graph("titulo",3);
    //lineal_pot_interp.get_RGBtoHSI().get_channel(2).get_equalize(256).display_graph("titulo",3);
    
    (resultado_promedio, promedio_ieq
     , resultado_interpolacion,  interpolacion_ieq
     , lineal_pot_prom, lineal_pot_interp
     //, hb_prom, hb_interp
     //, correccionGamma(hb_prom, gamma, A_gamma), correccionGamma(hb_interp, gamma, A_gamma)
     //,correccionGamma(interpolacion_ieq, gamma, A_gamma), correccionGammaIntensidad(interpolacion_ieq, gamma, A_gamma) 
    ).display("Promedio|PromedioEcu|Interpolacion|InterpolacionEcu|PromLinealPot|InterLinealPot");
        //|PromHB|InterHB");


    return 0;
}
