#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <complex>
#include <ctime>
#include "../utils/filtradoFrecuencia.cpp"
#include "../utils/fourier.cpp"


using namespace cimg_library;   //Necesario



int main(int argc, char *argv[]) {
    const char* input = cimg_option("-i", "../images/casilla.tif", "Input Image File");
    const unsigned int _frec_corte = cimg_option("-f", 50, "Input Image File");
    const float gamma_l = cimg_option("-l", 0.3, "Efecto en bajas frecuencias");
    const float gamma_h = cimg_option("-h", 1.3, "Efecto en altas frecuencias");
   
    CImg<double> img(input);
    unsigned int w = img.width();
    unsigned int h = img.height();

    //Obtenemos un filtro gaussiano pasa altos
    CImgList<double> filtro_gaussiano_pa = utils::filtroGaussiano(w,h,_frec_corte,true);
    CImgList<double> filtro_gaussiano_pa_mod = filtro_gaussiano_pa; 

    //Corremos el filtro gaussiano
    cimg_forXY(filtro_gaussiano_pa[0],x,y) {
        filtro_gaussiano_pa_mod[0](x,y) = gamma_l + filtro_gaussiano_pa[0](x,y);
        if (filtro_gaussiano_pa_mod[0](x,y) > gamma_h) //acotamos si nos pasamos
            filtro_gaussiano_pa_mod[0](x,y) = gamma_h;
    }


    //Calculamos el logaritmo de la imagen 
    CImg<double> log_img = img.abs().get_log();

    //Filtramos (devuelve el resultado antitransformado)
    CImg<double> resultado_filtrado = utils::filtradoFrecuencia(img, filtro_gaussiano_pa_mod);

    //Exponenciamos
    CImg<double> resultado_exp = resultado_filtrado.get_exp();

    CImgList<double> lista;
    lista.assign(   img.get_normalize(0,255), log_img, 
                    utils::get_magnitud(filtro_gaussiano_pa_mod), 
                    resultado_filtrado.get_normalize(0,255), resultado_exp.get_normalize(0,255));
    lista.display();

    return 0;
}
