#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <complex>
#include <ctime>
#include "../utils/PDI_functions.h"
#include "../utils/fourier.cpp"
#include "../utils/genArchivoMascara.cpp"
using namespace cimg_library;   //Necesario

//@ Genera un filtro ideal con el radio especificado (frecuencia de corte)
CImgList<double> filtroIdeal(unsigned int w, unsigned int h, unsigned int radio, bool pasaaltos = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    //Si es pasa altos, tenemos que iniciarla con 1, no con 0
    if (pasaaltos) {
        img[0].fill(1);
        //img[1].fill(1); //la parte imaginaria se queda en 0 igual
    }

    unsigned int centro_x = w/2;
    unsigned int centro_y = h/2;
    
    cimg_forXY(img[0],x,y) {
        if (  (x - centro_x)*(x - centro_x) + (y - centro_y)*(y - centro_y) <= radio*radio  ) {//si esta dentro del circulo
            if (pasaaltos) img[0](x,y) = 0.0;
            else img[0](x,y) = 1.0;
        }
    }

    img[0].shift(w/2, h/2,0,0,2);
    return img;
}


//@ Genera un filtro de Butterworth del orden y frecuencia de corte dados
CImgList<double> filtroButterworth(unsigned int w, unsigned int h, unsigned int frec_corte, unsigned int orden, bool pasaaltos = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    unsigned int centro_x = w/2;
    unsigned int centro_y = h/2;
    
    cimg_forXY(img[0],x,y) {
        double distancia = sqrt( (x - centro_x)*(x - centro_x) + (y - centro_y)*(y - centro_y)); 
        if (pasaaltos) img[0](x,y) = (1)/(1 + pow((float(frec_corte)/distancia), 2*orden));
        else img[0](x,y) =           (1)/(1 + pow((distancia/float(frec_corte)), 2*orden));
    }

    img[0].shift(w/2, h/2,0,0,2);
    return img;
}

//@ Genera un filtro Gaussiano
CImgList<double> filtroGaussiano(unsigned int w, unsigned int h, unsigned int frec_corte, bool pasaaltos = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    unsigned int centro_x = w/2;
    unsigned int centro_y = h/2;
    
    cimg_forXY(img[0],x,y) {
        double distancia = sqrt( (x - centro_x)*(x - centro_x) + (y - centro_y)*(y - centro_y)); 
        if (pasaaltos) img[0](x,y) = 1 - exp(-pow(distancia,2)/(2*(pow(frec_corte,2))));
        else           img[0](x,y) = exp(-pow(distancia,2)/(2*(pow(frec_corte,2))));
    }

    img[0].shift(w/2, h/2,0,0,2);
    return img;
}

CImg<double> filtradoFrecuencia(CImg<double> img, CImgList<double> filtro_frec) {
    
    //Obtenemos las transformadas de Fourier de la imagen y el filtro
    CImgList<double> fft_img = img.get_FFT(); 
    
    //Multiplicamos en frecuencia
    CImgList<double> tempy(2, img.width(), img.height(), img.depth(), img.spectrum(), 0);
   
    cimg_forXY(img,x,y) {
        //Capturamos los valores
        complex<double> factor1 (fft_img[0](x,y), fft_img[1](x,y)),
                        factor2 (filtro_frec[0](x,y), filtro_frec[1](x,y));

        //Realizamos el producto de binomios
        complex<double> prod = factor1*factor2;
        //Asignamos en real e imaginario
        tempy[0](x,y) = real(prod);
        tempy[1](x,y) = imag(prod);
    }
    //Calculamos la inversa
    return tempy.get_FFT(true)[0];
}
 
void filtradoFrecuenciaGaussiano(CImg<double> img, CImgList<double> filtro_frec, CImg<double> & resultado_espacio, CImgList<double> & resultado_frec) {
    
    //Obtenemos las transformadas de Fourier de la imagen y el filtro
    CImgList<double> fft_img = img.get_FFT(); 
    
    //Multiplicamos en frecuencia
    CImgList<double> tempy(2, img.width(), img.height(), img.depth(), img.spectrum(), 0);
   
    cimg_forXY(img,x,y) {
        //Capturamos los valores
        complex<double> factor1 (fft_img[0](x,y), fft_img[1](x,y)),
                        factor2 (filtro_frec[0](x,y), filtro_frec[1](x,y));

        //Realizamos el producto de binomios
        complex<double> prod = factor1*factor2;
        //Asignamos en real e imaginario
        tempy[0](x,y) = real(prod);
        tempy[1](x,y) = imag(prod);
    }
    //Copiamos los resultados
    resultado_frec = tempy;
    
    resultado_espacio = tempy.get_FFT(true)[0];
}
  
CImg<double> get_filtro(std::string nombre) {
    std::ifstream f(nombre.c_str());
    if (!f.is_open()) {
        std::cout<<"No se pudo abrir el archivo "<<nombre<<"\n";
        exit(-1);
    }
    unsigned int filas;
    unsigned int columnas;
    float valor;

    f>>filas;
    f>>columnas;
    CImg<double> salida(columnas, filas, 1 , 1, 0);
    for (unsigned int i = 0; i < filas; i++) { 
        for (unsigned int j = 0; j < columnas; j++) {
            f>>valor;
            // std::cerr<<"wololo"<<valor<<"\n";
            salida(j,i) =  valor;
            std::cout<<salida(j,i)<<' ';
        }
        std::cout<<'\n';
    }
    f.close();
    return salida;
}

int main(int argc, char *argv[]) {
    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* archivo_filtro = cimg_option("-a", "filtro_gaussiano_espacio.txt", "Archivo del filtro gaussiano");
    const unsigned int _frec_corte = cimg_option("-f", 150, "Input Image File");
    const unsigned int _orden_butterworth = cimg_option("-b", 1, "Input Image File");
    const unsigned int _ancho = cimg_option("-w", 11, "ancho del filtro gaussiano");
    const unsigned int _alto = cimg_option("-e", 11, "alto del filtro gaussiano");
    const float ruido = cimg_option("-r", 1.0, "varianza del ruido");

    CImg<double> img(input), resultado_ideal_pb, resultado_butterworth_pb, resultado_gaussiano_pb;
    CImg<double> resultado_ideal_pa, resultado_butterworth_pa, resultado_gaussiano_pa, resultado_gaussiano_espacio;
    CImgList<double> resultado_gaussiano_frec;

    //Agregamos ruido
    img.noise(ruido);

    unsigned int w = img.width();
    unsigned int h = img.height();

    //Cargamos todos los filtros
    CImgList<double> filtro_ideal_pb = filtroIdeal(w, h, _frec_corte);
    CImgList<double> filtro_butterworth_pb = filtroButterworth(w, h, _frec_corte, _orden_butterworth);
    CImgList<double> filtro_gaussiano_pb = filtroGaussiano(w, h, _frec_corte);

    //Generamos el filtro gaussiano en el espacio
    utils::genArchivoMascaraGaussiana(archivo_filtro, _ancho, _alto, _frec_corte);
    //Lo leemos
    CImg<double> filtro_gaussiano_espacio = get_filtro(archivo_filtro);
   
    unsigned int p = w + filtro_gaussiano_espacio.width() - 1;
    unsigned int q = h + filtro_gaussiano_espacio.height() - 1;

    //Redimensionamos el filtro
    filtro_gaussiano_espacio.resize(p, q, -100, -100, 0);

    //Transformamos el filtro a frecuencia
    CImgList<double> filtro_gaussiano_espacio_fft = filtro_gaussiano_espacio.get_FFT();

    //Filtramos con el filtro gaussiano definido en espacio
    filtradoFrecuenciaGaussiano(img.get_resize(p, q, -100, -100, 0), filtro_gaussiano_espacio_fft, resultado_gaussiano_espacio, resultado_gaussiano_frec);
    resultado_gaussiano_espacio.crop(_ancho/2, _alto/2, p - _ancho/2, q - _alto/2);

    //Calculamos todos los resultados en frecuencia
    resultado_ideal_pb = filtradoFrecuencia(img, filtro_ideal_pb);
    resultado_butterworth_pb = filtradoFrecuencia(img, filtro_butterworth_pb);
    resultado_gaussiano_pb = filtradoFrecuencia(img, filtro_gaussiano_pb);

    CImgList<double> lista_gauss;
    lista_gauss.assign(img,  utils::get_magnitud(resultado_gaussiano_frec, true), 
                      resultado_gaussiano_espacio);
    //lista_gauss.display("Original|GaussianoEspacio(frec)|ResultadoGaussianoEspacio");

    //Dibuja
    CImgList<double> lista_pb;
    lista_pb.assign(img, 
                     //utils::get_magnitud(filtro_ideal_pb), 
                        resultado_ideal_pb,
                      //utils::get_magnitud(filtro_butterworth_pb), 
                      resultado_butterworth_pb,
                      //utils::get_magnitud(filtro_gaussiano_pb), 
                      //resultado_gaussiano_pb);
                      resultado_gaussiano_pb,
                      resultado_gaussiano_espacio);
    lista_pb.display("Original|ResultadoIdeal|ResultadoButterworth||ResultadoGaussianoFrec|ResultadoGaussianoEspacio");

    /*
    //Filtrado pasa altos
    CImgList<double> filtro_ideal_pa = filtroIdeal(w, h, _frec_corte, true);
    CImgList<double> filtro_butterworth_pa = filtroButterworth(w, h, _frec_corte, _orden_butterworth, true);
    CImgList<double> filtro_gaussiano_pa = filtroGaussiano(w, h, _frec_corte, true);

    //Calculamos todos los resultados
    resultado_ideal_pa = filtradoFrecuencia(img, filtro_ideal_pa);
    resultado_butterworth_pa = filtradoFrecuencia(img, filtro_butterworth_pa);
    resultado_gaussiano_pa = filtradoFrecuencia(img, filtro_gaussiano_pa);

    //Dibuja
    CImgList<double> lista_pa;
    lista_pa.assign(img, utils::get_magnitud(filtro_ideal_pa), resultado_ideal_pa,
                      utils::get_magnitud(filtro_butterworth_pa), resultado_butterworth_pa,
                      utils::get_magnitud(filtro_gaussiano_pa), resultado_gaussiano_pa);
    lista_pa.display();
    */
    return 0;
}
