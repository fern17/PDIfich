#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <complex>
#include <ctime>
#include "../utils/PDI_functions.h"
#include "../utils/genArchivoMascara.cpp"
using namespace cimg_library;   //Necesario

CImg<float> get_filtro(std::string nombre) {
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
    CImg<float> salida(columnas, filas, 1 , 1, 0);
    for (unsigned int i = 0; i < filas; i++) { 
        for (unsigned int j = 0; j < columnas; j++) {
            f>>valor;
            // std::cerr<<"wololo"<<valor<<"\n";
            salida(j,i) =  valor;
        }
    }
    f.close();
    return salida;
}

int main(int argc, char *argv[]) {
    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const unsigned int _ancho = cimg_option("-w", 21, "Input Image File");
    const unsigned int _alto = cimg_option("-h", 21, "Input Image File");
    const float _sigma = cimg_option("-s", 2.0, "Input Image File");

    CImg<double> img(input), resultado_espacial, resultado_frecuencia;

    //Se genera el filtro y lo cargamos en una imagen
    utils::genArchivoMascaraGaussiana("filtroej3.txt", _ancho, _alto, _sigma);
    CImg<double> filtro = get_filtro("filtroej3.txt");

    time_t inicio;
    time_t fin;
    inicio = time(NULL);
    //Filtramos en el espacio con convoluciob
    resultado_espacial = img.get_convolve(filtro);
    fin = time(NULL);

    std::cout<<"El filtrado en espacio toma "<<difftime(fin,inicio)<<" segundos\n";
    //Ahora nos preparamos para filtrar en frecuencia

    //Calculamos los tamaños del zeropadding
    unsigned int p = (img.width() + filtro.width() - 1);
    unsigned int q = (img.height() + filtro.height() - 1);

    //Resizeamos en espacio para posibilitar la multiplicacion en frecuencia 
    filtro.resize(p, q, -100, -100, 0);
    img.resize(p, q, -100, -100, 0);

    //Obtenemos las transformadas de Fourier de la imagen y el filtro
    CImgList<double> fft_img = img.get_FFT(); 
    CImgList<double> fft_filtro = filtro.get_FFT();
    
    //Multiplicamos en frecuencia
    CImgList<double> tempy(2, img.width(), img.height(), img.depth(), img.spectrum(), 0);
   
    inicio = time(NULL);
    cimg_forXY(img,x,y) {
        //Capturamos los valores
        complex<double> factor1 (fft_img[0](x,y), fft_img[1](x,y)),
                        factor2 (fft_filtro[0](x,y), fft_filtro[1](x,y));

        //Realizamos el producto de binomios
        complex<double> prod = factor1*factor2;
        //Asignamos en real e imaginario
        tempy[0](x,y) = real(prod);
        tempy[1](x,y) = imag(prod);
    }
    fin = time(NULL);
    //Calculamos la inversa
    resultado_frecuencia = tempy.get_FFT(true)[0];
    std::cout<<"El filtrado en frecuencia toma "<<difftime(fin,inicio)<<" segundos \n";

    
    //Ahora realizamos un cropeado para sacar los bordes de mas
    unsigned int delta_crop_x = floor(_ancho/2.0); 
    unsigned int delta_crop_y = floor(_alto/2.0); 

    //Recorta la basura (resta delta_crop_x o delta_crop_y)
    resultado_frecuencia.crop(  delta_crop_x, delta_crop_y, 
                                resultado_frecuencia.width() - delta_crop_x, 
                                resultado_frecuencia.height() - delta_crop_y);

    //Dibuja
    CImgList<double> lista;
    lista.assign(img, resultado_espacial,resultado_frecuencia.normalize(0,255));
    lista.display();

    return 0;
}
