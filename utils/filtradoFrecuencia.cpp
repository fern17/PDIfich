namespace utils {
#include <cmath>
#include <CImg.h>               //include basico de CImg
#include <complex>

using namespace cimg_library;   //Necesario

//@ Genera un filtro ideal con el radio especificado (frecuencia de corte)
CImgList<double> filtroIdeal(unsigned int w, unsigned int h, unsigned int radio, bool pasaaltos = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    //Si es pasa altos, tenemos que iniciarla con 1, no con 0
    if (pasaaltos) {
        img[0].fill(1);
        //img[1].fill(1); //La parte imaginaria se queda en 0 igual
    }

    unsigned int centro_x = w/2;
    unsigned int centro_y = h/2;
    
    cimg_forXY(img[0],x,y) {
        if (  (x - centro_x)*(x - centro_x) + (y - centro_y)*(y - centro_y) <= radio*radio  ) {//si esta dentro del circulo
            if (pasaaltos) img[0](x,y) = 0.0;
            else img[0](x,y) = 1.0;
        }
    }

    img[0].shift(-w/2, -h/2,0,0,2);
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

    img[0].shift(-w/2, -h/2,0,0,2);
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

//@ Realiza el filtrado en frecuencia a partir de la especificacion de una imagen y el filtro en frecuencia
CImg<double> filtradoFrecuencia(CImg<double> img, CImgList<double> filtro_frec) {
    
    //Obtenemos las transformadas de Fourier de la imagen y el filtro
    CImgList<double> fft_img = img.get_FFT(); 
    
    //Multiplicamos en frecuencia
    CImgList<double> tempy(2, img.width(), img.height(), img.depth(), img.spectrum(), 0);
   
    cimg_forXY(img,x,y) {
        //Capturamos los valores
        std::complex<double> factor1 (fft_img[0](x,y), fft_img[1](x,y)),
                        factor2 (filtro_frec[0](x,y), filtro_frec[1](x,y));

        //Realizamos el producto de binomios
        std::complex<double> prod = factor1*factor2;
        //Asignamos en real e imaginario
        tempy[0](x,y) = std::real(prod);
        tempy[1](x,y) = std::imag(prod);
    }
    //Calculamos la inversa
    return tempy.get_FFT(true)[0];
}

}

