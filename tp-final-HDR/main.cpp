#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include "getSeeds.cpp"
#include "cubicInterpolator.cpp"
#include "correccionGamma.cpp"
#include "toString.cpp"
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

//Realiza un promediado en vecindarios de 2x2
CImg<double> promediado(CImg<double> img_compuesta) {
    CImg<double> resultado(W,H,1,C);

    //Recorre la imagen y promedia los valores
    for (unsigned int x = 0; x < img_compuesta.width(); x+=2) {
        for (unsigned int y = 0; y < img_compuesta.height(); y+=2) {
            //if (x % 533 == 0 && y % 499 == 0) std::cout<<x<<' '<<y<<'\n';
            
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
            //if (x % 366 == 0 && y % 297 == 0) std::cout<<x<<' '<<y<<'\n';
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


// Devuelve el Operador de Sobel para el gradiente en las 4 direcciones
CImgList<double> OperadorSobel(){
    //CImgList<double> ret_val(4,3,3,1,1,0);
    CImgList<double> ret_val(2,3,3,1,1,0);
    //Gradiente en X
    ret_val[0](0,0) = -1;
    ret_val[0](1,0) = -2;
    ret_val[0](2,0) = -1;
    ret_val[0](0,2) = 1;
    ret_val[0](1,2) = 2;
    ret_val[0](2,2) = 1;
    
    //Gradiente en Y
    ret_val[1](0,0) = -1;
    ret_val[1](0,1) = -2;
    ret_val[1](0,2) = -1;
    ret_val[1](2,0) = 1;
    ret_val[1](2,1) = 2;
    ret_val[1](2,2) = 1;

    /*
    ret_val[2](0,1) = -1;
    ret_val[2](0,2) = -2;
    ret_val[2](1,2) = -1;
    ret_val[2](1,0) = 1;
    ret_val[2](2,0) = 2;
    ret_val[2](2,1) = 1;

    ret_val[3](0,1) = -1;
    ret_val[3](0,0) = -2;
    ret_val[3](1,0) = -1;
    ret_val[3](1,2) = 1;
    ret_val[3](2,2) = 2;
    ret_val[3](2,1) = 1;
    */
    return ret_val;
}


/*
Aplica el operador deseado por ejemplo OperadorSobel
*/
CImg<double> AplicarOperador(CImg<double> & img, CImgList<double> &operador) {
    CImgList<double> resultados;
    unsigned int cantidad = operador.size();
    
    //Aplica los operadores a la imagen y lo guarda en resultados
    for (unsigned int i = 0; i < cantidad; i++) {
        CImg<double> tempy = img.get_convolve(operador[i]);
        resultados.push_back(tempy);
    }

    //Mezcla los resultados utilizando la formula: G = sqrt( G(x)G(x) + G(y)G(y) )
    CImg<double> retval(img.width(), img.height(), 1, img.spectrum(), 0);
    cimg_forXYC(retval, x, y, c) {
        retval(x,y,0,c) = std::sqrt(std::pow(resultados[0](x,y,0,c),2) + std::pow(resultados[1](x,y,0,c),2));
    }

    return retval;
}

/* Version vieja
CImg<double> AplicarOperador(CImg<double> & img, CImgList<double> &operador) {
    CImgList<double> resultados;
    CImg<double> retval(img.width(), img.height(), 1, 1, 0);
    unsigned int cantidad = operador.size();
    for (unsigned int i = 0; i < cantidad; i++) {
        CImg<double> tempy = img.get_convolve(operador[i]);
        retval += tempy;
    }
     
    return retval;
}
*/

int main(int argc, char *argv[]) {

	//Leer sobre que ejemplo se va a trabajar
	const char* _ejemplo = cimg_option("-e", "interiorb", "Carpeta de Ejemplos");
	//Leer sobre que formato se va a trabajar
	const char* _format = cimg_option("-f", "JPG", "Formato de Ejemplos");

    //Para realizar la interpolacion, se debe elegir un paso (dx,dy) desde el centro del vecindario
    const double dx = cimg_option("-x", 0.5, "Paso en x en la interpolacion");
    const double dy = cimg_option("-y", 0.5, "Paso en y en la interpolacion");
    const double gamma = cimg_option("-g", 0.8, "Gamma de la correccion gamma");
    const double A_gamma = cimg_option("-a", 1.0, "Parametro a de la corrección gamma");
    const double cte_bordes = cimg_option("-b", 0.2, "Constante de Bordes");
    const bool guardar = cimg_option("-s", true, "Guardar archivos");
    const bool dibujar = cimg_option("-d", true, "Dibujar imagenes");

    std::cout<<"\n\nCreacion de imagenes de alto rango dinamico mediante interpolacion de fuentes\n";
    std::cout<<"Autores: Damian Benassi, Fernando Nellmeldin y Mariano Peyregne\n";
    std::cout<<"Procesamiento digital de imagenes. Facultad de Ingenieria y Ciencias Hidricas. Universidad Nacional del Litoral\n\n";

    std::cout<<"Parametros:\n";
    //Impresion de parametros
    std::cout<<"gamma = "<<gamma<<"\nA_gamma = "<<A_gamma<<"\nConstante de bordes = "<<cte_bordes<<'\n';


    //Guardado de estadisticas
    std::ofstream salida("output.txt", std::ios::app);

    //Contador de tiempo
	const clock_t begin_time = clock();
	
    salida<<_ejemplo<<",";
	
    //Obtengo el operador SOBEL
    CImgList<double> bordes;
    CImgList<double> operador_sobel = OperadorSobel();

    
    //Obtener las imágenes de la carpeta
	std::vector<std::string> imagenes_a_leer = getSeeds(_ejemplo, _format);
    std::vector<bool> imagenes_existentes;

    std::cout<<"Leyendo imagenes...\n";
	//Recorremos las imágenes leidas y las agregamos a una lista
	//En caso de no existir alguna (faltante), la dejamos en negro
	unsigned int _n = imagenes_a_leer.size();

	for(unsigned int i = 0; i < _n; i++) {
		if (imagenes_a_leer[i].compare("NOT_FOUND") == 0) { //Bandera para identificar que no existe la imagen
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
            CImg<double> borde = AplicarOperador(img_tmp, operador_sobel);
            bordes.push_back(  borde );
		}
	}

	//Declaramos la imagen compuesta
	CImg<double> img_compuesta(2*W, 2*H, 1, C, 0 );

    std::cout<<"Combinando imagenes...\n";

    //Creamos la imagen compuesta
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

    std::cout<<"Realizando el promediado...\n";
    //Calculamos los resultados con los dos metodos
    CImg<double> resultado_promedio = promediado(img_compuesta);
    std::cout<<"Realizando la interpolacion cubica bidimensional...\n";
    CImg<double> resultado_interpolacion = interpolacion(img_compuesta, dx, dy);

    std::cout<<"Aplicando la correccion gamma...\n";
    //Aplicamos correccion gamma
    CImg<double> promedio_gamma = correccionGamma(resultado_promedio, gamma, A_gamma);
    CImg<double> interpolacion_gamma = correccionGamma(resultado_interpolacion, gamma, A_gamma);
    
    std::cout<<"Aplicando el realce de bordes...\n";
    //Aplicamos realce de bordes
    unsigned int n_bordes = bordes.size();
    //CImg<double> bordes_total(W,H,1,1);
    CImg<double> bordes_total(W, H, 1, C, 0.0);
    for (unsigned int i = 0; i < n_bordes; i++) {
        bordes_total += bordes[i];
    }
    bordes_total.normalize(0.0,1.0);


    //Definimos las imagenes resultado finales luego del postproceso
    CImg<double> promedio_postproceso = promedio_gamma.get_normalize(0.0,1.0) + cte_bordes * bordes_total;
    CImg<double> interpolacion_postproceso = interpolacion_gamma.get_normalize(0.0,1.0) + cte_bordes * bordes_total;

    //(promedio_postproceso, interpolacion_postproceso).display();

    std::cout<<"Proceso terminado.\n";
    //Contador del tiempo
    salida<<float( clock () - begin_time ) /  CLOCKS_PER_SEC<<"\n";
    salida.close();


    if (guardar == true) {

        std::cout<<"Guardando las imagenes...\n";
        //-----Guardado sin postproceso
        //Conversion para guardado 
        CImg<double> resultado_imagen_prom(resultado_promedio);
        CImg<double> resultado_imagen_inter(resultado_interpolacion);

        resultado_imagen_prom.normalize(0,255);
        resultado_imagen_inter.normalize(0,255);

        //Guardado
        std::string archivo_spp = "resultados/" + std::string(_ejemplo) + "_promedio_sinpostproceso.png";
        std::string archivo_spi = "resultados/" + std::string(_ejemplo) + "_interpolacion_sinpostproceso.png";
        resultado_imagen_prom.save(archivo_spp.c_str());
        resultado_imagen_inter.save(archivo_spi.c_str());

        //-----Guardado con postproceso
        //Conversion para guardado 
        CImg<double> postproceso_imagen_prom(promedio_postproceso);
        CImg<double> postproceso_imagen_inter(interpolacion_postproceso);
        postproceso_imagen_prom.normalize(0,255);
        postproceso_imagen_inter.normalize(0,255);

        //Guardado
        std::string archivo_p = "resultados/" + std::string(_ejemplo) + "_promedio__g_" + toString(gamma) + "_a_" + toString(A_gamma) + "_b_" + toString(cte_bordes) + ".png";
        std::string archivo_i = "resultados/" + std::string(_ejemplo) + "_interpolacion__g_" + toString(gamma) + "_a_" + toString(A_gamma) + "_b_" + toString(cte_bordes) + ".png";
        postproceso_imagen_prom.save(archivo_p.c_str());
        postproceso_imagen_inter.save(archivo_i.c_str());
    }

    //Dibujamos
    if (dibujar == true) {
        lista.display("Imagenes de Entrada", false);
        (resultado_promedio, promedio_postproceso).display("Resultado Promedio, Promedio Postproceso", false);
        (resultado_interpolacion, interpolacion_postproceso).display("Resultado Interpolacion, Interpolacion Postproceso", false);
    }

    return 0;
}
