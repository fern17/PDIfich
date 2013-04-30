#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <complex>
#include <ctime>
#include <fstream>
#include "../utils/filtradoFrecuencia.cpp"
#include "../utils/fourier.cpp"


using namespace cimg_library;   //Necesario

   
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
    const char* input = cimg_option("-i", "../images/camaleon.tif", "Input Image File");
    const char * _filtro = cimg_option("-f", "filtro_palto_suma0.txt", "Filtro a leer");
    const float A = cimg_option("-g", 1.0, "Factor de ganancia A");
    const float _a = cimg_option("-a", 0.0, "Constante a");
    const float _b = cimg_option("-b", 1.0, "Constante b");
  
    std::cout<<"Parametros elegidos";
    std::cout<<"\n A = "<<A;
    std::cout<<"\n a = "<<_a;
    std::cout<<"\n b = "<<_b;
    std::cout<<"\n";
    CImg<double> filtro = get_filtro(_filtro);
    CImg<double> filtro_chico = filtro;

    CImg<double> img(input);
    //img.normalize(0,255);
    unsigned int w = img.width();
    unsigned int h = img.height();


    unsigned int p = w + filtro.width() - 1;
    unsigned int q = h + filtro.height() - 1;
    img.resize(p, q, -100, -100, 0);
    filtro.resize(p, q, -100, -100, 0);

    //Calculamos todos los filtros
    CImgList<double> filtro_freq = filtro.get_FFT();
    CImgList<double> filtro_alta_pot = filtro_freq;
    CImgList<double> filtro_enfasis_alta_frec = filtro_freq;

    cimg_forXY(filtro_freq[0], x, y) {
        //calculamos el filtro de alta potencia
        filtro_alta_pot[0](x,y) = (A-1) + filtro_freq[0](x,y);
        filtro_alta_pot[1](x,y) = (A-1) + filtro_freq[1](x,y);

        //calculamos el filtro de enfasis de alta frecuencia
        filtro_enfasis_alta_frec[0](x,y) = _a + _b * filtro_freq[0](x,y);
        filtro_enfasis_alta_frec[1](x,y) = _a + _b * filtro_freq[1](x,y);
    }

    //Calculamos los resultados
    CImg<double> resultado_ap = utils::filtradoFrecuencia(img, filtro_alta_pot);
    CImg<double> resultado_eaf = utils::filtradoFrecuencia(img, filtro_enfasis_alta_frec);

    //Vemos el resultado en el espacio
    CImg<double> resultado_espacio = img.get_convolve(filtro_chico);

    //Dibujamos
    CImgList<double> lista;
    lista.assign(img, utils::get_magnitud(img.get_FFT(), true), 
                 resultado_ap,  utils::get_magnitud(resultado_ap.get_FFT(), true).get_normalize(0,255), 
                 resultado_eaf, utils::get_magnitud(resultado_eaf.get_FFT(), true).get_normalize(0,255), 
                 resultado_espacio);
    lista.display();

    return 0;
}
