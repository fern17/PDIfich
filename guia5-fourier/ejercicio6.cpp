#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <complex>
#include <ctime>
#include "../utils/filtradoFrecuencia.cpp"
#include "../utils/fourier.cpp"


using namespace cimg_library;   //Necesario

//@ Genera un filtro homomorfico
CImgList<double> filtroHomomorfico(unsigned int w, unsigned int h, unsigned int frec_corte, float gamma_l, float gamma_h, float c) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    unsigned int centro_x = w/2;
    unsigned int centro_y = h/2;
    
    cimg_forXY(img[0],x,y) {
        double distancia = sqrt( (x - centro_x)*(x - centro_x) + (y - centro_y)*(y - centro_y)); 
        img[0](x,y) = (gamma_h - gamma_l) * (1 - exp (-c * pow(distancia,2) / (pow(frec_corte,2)))) + gamma_l;
    }

    img[0].shift(w/2, h/2,0,0,2);
    return img;
}

int main(int argc, char *argv[]) {
    const char* input = cimg_option("-i", "../images/casilla.tif", "Input Image File");
    const unsigned int _frec_corte = cimg_option("-f", 150, "Input Image File");
    const float gamma_l = cimg_option("-b", 0.5, "Efecto en bajas frecuencias");
    const float gamma_h = cimg_option("-a", 2.0, "Efecto en altas frecuencias");
    const float c = cimg_option("-c", 1, "Constante de sharpness");
    std::cout<<"Parametros seleccionados: \n  * frec_corte = "<<_frec_corte<<
        "\n  * gamma_l = "<<gamma_l<<"\n  * gamma_h = "<<gamma_h<<"\n  * c = "<<c<<'\n';
   
    CImg<double> img(input);
    //img.normalize(0,255);
    unsigned int w = img.width();
    unsigned int h = img.height();

    //Obtenemos un filtro gaussiano pasa altos
    CImgList<double> filtro_homomorfico = filtroHomomorfico(w, h,_frec_corte, gamma_l, gamma_h, c);


    //Calculamos el logaritmo de la imagen 
    CImg<double> log_img(w, h, 1, 1, 0);

    cimg_forXY(img,x,y) {
        if (img(x,y) < 1.0) //Si el logaritmo es menor a 1, explota
            log_img(x,y) = 0.0;
        else
            log_img(x,y) = log(img(x,y));
    }

    //Filtramos (devuelve el resultado antitransformado)
    CImg<double> resultado_filtrado = utils::filtradoFrecuencia(log_img, filtro_homomorfico);

    //Exponenciamos
    CImg<double> resultado_exp = resultado_filtrado.get_exp();

    //Aplicamos el filtro a la imagen ecualizada
    CImg<double> equ = log_img;
    equ.equalize(256);
    CImg<double> resultado_filtrado_equ = utils::filtradoFrecuencia(equ, filtro_homomorfico);
    resultado_filtrado_equ.exp();

    CImgList<double> lista;
    lista.assign(   img, 
                    utils::get_magnitud(filtro_homomorfico, true), 
                    resultado_exp, img.get_equalize(256), abs(resultado_exp - img).equalize(0,255), resultado_filtrado_equ);
    lista.display();

    return 0;
}
