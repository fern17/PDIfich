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
    unsigned int p = (img.width() + filtro.width() - 1);
    unsigned int q = (img.height() + filtro.height() - 1);

    //Resizeamos en espacio para posibilitar la multiplicacion en frecuencia 
    //filtro.resize(p, q, -100, -100, 0);
    //img.resize(p, q, -100, -100, 0);

    //Centramos el filtro
    //filtro.shift(p/2,q/2);

    //Obtenemos las transformadas de Fourier de la imagen y el filtro
    CImgList<double> fft_img = img.get_FFT(); 
    CImgList<double> fft_filtro = filtro.get_FFT();
    
    //Redimensiona la imagen en frecuencia
    fft_img[0].resize(p, q, -100, -100, 0);
    fft_img[1].resize(p, q, -100, -100, 0);
    //Centra la imagen en frecuencia
    fft_img[0].shift(p/2,q/2,0,0,2);
    fft_img[1].shift(p/2,q/2,0,0,2);

    
    //Redimensiona el filtro en frecuencia
    fft_filtro[0].resize(p, q, -100, -100, 0);
    fft_filtro[1].resize(p, q, -100, -100, 0);
    //Centra el filtro en frecuencia
    fft_filtro[0].shift(p/2,q/2,0,0,2);
    fft_filtro[1].shift(p/2,q/2,0,0,2);
 
    
    //Multiplicamos en frecuencia
    fft_img[0] = fft_img[0] * fft_filtro[0];
    fft_img[1] = fft_img[1] * fft_filtro[1];

    
    //Calculamos la inversa
    resultado_frecuencia = fft_img.get_FFT(true)[0];

    //resultado_frecuencia.shift(-p/2, -q/2, 0, 0, 2).normalize(0,255);
    CImgList<double> lista;
    lista.assign(img, resultado_espacial,resultado_frecuencia);
    lista.display();





    // CImgList<double> tdf = img.get_FFT();
    // CImgList<double> tdf2 = img2.get_FFT();


    // CImg<double> magnitud = get_magnitud(tdf);
    // CImg<double> fase = get_fase(tdf2);

    // CImgList<double> resultado(2, img.width(), img.height(), img.depth(), img.spectrum(), 0 );

    
    // complex<double> I(0,1);

    // cimg_forXY(img, x, y) {
    //     complex<double> valor = magnitud(x,y) * exp(I * fase(x,y) );

    //     resultado[0](x,y) = real(valor);
    //     resultado[1](x,y) = imag(valor);
    // }

    // CImg<double> resultado_i = resultado.get_FFT(true)[0];

    // CImgList<double> lista;

    // lista.assign(img, img2, resultado_i);

    // lista.display();

    return 0;
}
