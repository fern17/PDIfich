#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <complex>
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

CImg<double> get_magnitud(CImgList<double> & tdf) {
    CImg<double> output = tdf[0];

    cimg_forXY(output, x ,y ) {
        output(x,y) = sqrt( pow(tdf[0](x,y),2) + pow(tdf[1](x,y),2) );
    }
    return output;
}

CImg<double> get_fase(CImgList<double> & tdf) {
    CImg<double> output = tdf[0];

    cimg_forXY(output, x ,y ) {
        output(x,y) = atan2( tdf[1](x,y) , tdf[0](x,y) );
    }
    return output;
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

    //Convolucionamos en el espacio
    resultado_espacial = img.get_convolve(filtro);
    
    //Calculamos los tamaños del zeropadding
    //unsigned int p = img.width();
    //unsigned int q = img.height();
    unsigned int p = (img.width() + filtro.width() - 1);
    unsigned int q = (img.height() + filtro.height() - 1);

    //Resizeamos en espacio para posibilitar la multiplicacion en frecuencia 
    filtro.resize(p, q, -100, -100, 0);
    img.resize(p, q, -100, -100, 0);

    //Obtenemos las transformadas de Fourier de la imagen y el filtro
    CImgList<double> fft_img = img.get_FFT(); 
    CImgList<double> fft_filtro = filtro.get_FFT();
    
    //Multiplicamos en frecuencia
    CImg<double> tempy_r(img.width(), img.height(), img.depth(), img.spectrum(), 0);
    CImg<double> tempy_i(img.width(), img.height(), img.depth(), img.spectrum(), 0);
    
    cimg_forXY(img,x,y) {
        //Capturamos los valores
        complex<double> factor1 (fft_img[0](x,y), fft_img[1](x,y)),
                        factor2 (fft_filtro[0](x,y), fft_filtro[1](x,y));

        //Realizamos el producto de binomios
        complex<double> prod = factor1*factor2;
        //Asignamos en real e imaginario
        tempy_r(x,y) = real(prod);
        tempy_i(x,y) = imag(prod);
    }
    //Juntamos en tempy la parte real e imaginaria
    CImgList<double> tempy;
    tempy.assign(tempy_r, tempy_i);
    //Calculamos la inversa
    resultado_frecuencia = tempy.get_FFT(true)[0];

    //Ahora realizamos un cropeado para sacar los bordes
    unsigned int delta_crop_x = ceil(_ancho/2.0); //11 pero para mostrar de donde se saca
    unsigned int delta_crop_y = ceil(_alto/2.0); //11 pero para mostrar de donde se saca

    //Recorta la basura (resta delta_crop_x o delta_crop_y)
    resultado_frecuencia.crop(delta_crop_x, delta_crop_y, 
                            resultado_frecuencia.width()-delta_crop_x, resultado_frecuencia.height() - delta_crop_y);

    CImgList<double> lista;
    lista.assign(img, resultado_espacial,resultado_frecuencia.normalize(0,255));
    lista.display();

    return 0;
}
