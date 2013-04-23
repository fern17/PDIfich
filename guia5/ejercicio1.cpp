#include <CImg.h>               //include basico de CImg
#include <iostream>
#include "../utils/PDI_functions.h"
using namespace cimg_library;   //Necesario

CImg<bool> lineaVertical(unsigned int w, unsigned int h, unsigned int pos = 0) {
    CImg<bool> ret_val(w,h,1,1,false);
    for (unsigned int i = 0; i < h; i++) {
        ret_val(pos,i) = true;
    }
    return ret_val;
}

CImg<bool> lineaHorizontal(unsigned int w, unsigned int h, unsigned int pos = 0) {
    CImg<bool> ret_val(w,h,1,1,false);
    for (unsigned int i = 0; i < h; i++) {
        ret_val(i,pos) = true;
    }
    return ret_val;
}

CImg<bool> rectCentrado(unsigned int w, unsigned int h, unsigned int w1, unsigned int w2) {
    CImg<bool> ret_val(w,h,1,1,false);
    unsigned int xc = w/2;
    unsigned int yc = h/2;
    for (unsigned int i = xc - w1/2; i <= xc + w1/2; i++) {
        for (unsigned int j = xc - w2/2; j <= xc + w2/2; j++) {
            ret_val(i,j) = true;
        }
    }
    return ret_val;
}

CImg<bool> circuloCentrado(unsigned int lado, unsigned int radio) {
    CImg<bool> ret_val(lado,lado,1,1,false);

    unsigned int centro = lado/2;

    cimg_forXY(ret_val,x,y) {
        if (  (x-centro)*(x-centro) + (y-centro)*(y-centro) <= radio*radio  )
            ret_val(x,y) = true;
    }
    return ret_val;
}

void inciso3() {
    
    unsigned int w = 256;
    unsigned int h = 256;

    CImg<double> linea_vertical = lineaVertical(w,h,w/2).get_normalize(0,255);
    CImg<double> linea_horizontal = lineaHorizontal(w,h,h/2).get_normalize(0,255);
    CImg<double> cuadrado = rectCentrado(w,h,w/4,h/4).get_normalize(0,255);
    CImg<double> rectangulo = rectCentrado(w,h,w/2,h/10).get_normalize(0,255);
    CImg<double> circulo = circuloCentrado(w,w/1.8).get_normalize(0,255);

    CImgList<double> lista;
    lista.assign(linea_vertical,linea_horizontal,cuadrado,rectangulo,circulo);
    lista.display();

  //CImgList<double> f_linea_vertical = linea_vertical.get_FFT();
  //CImgList<double> f_linea_horizontal = linea_horizontal.get_FFT();
  //CImgList<double> f_cuadrado = cuadrado.get_FFT();
  //CImgList<double> f_rectangulo = rectangulo.get_FFT();
  //CImgList<double> f_circulo = circulo.get_FFT();

    CImg<double> fm_linea_vertical = magn_tdf(linea_vertical, true); 
    CImg<double> fm_linea_horizontal = magn_tdf(linea_horizontal, true); 
    CImg<double> fm_cuadrado = magn_tdf(cuadrado, true); 
    CImg<double> fm_rectangulo = magn_tdf(rectangulo, true); 
    CImg<double> fm_circulo = magn_tdf(circulo, true); 

    CImgList<double> lista_fft;
    lista_fft.assign(fm_linea_vertical, fm_linea_horizontal, fm_cuadrado, fm_rectangulo, fm_circulo);
    lista_fft.display();


}

void inciso4() {
    unsigned int w = 512;
    unsigned int h = 512;

    CImg<double> linea = lineaVertical(w,h,w/2).get_normalize(0,255);
    CImg<double> rotada = linea.get_rotate(20);

    CImg<double> c_linea = linea.get_crop(w/4,h/4,3*w/4, 3*h/4);
    CImg<double> c_rotada = rotada.get_crop(w/4+100,h/4,3*w/4+100, 3*h/4);

    CImgList<double> lista;
    lista.assign(linea,rotada,c_linea,c_rotada);
    lista.display();

    CImg<double> fm_linea = magn_tdf(c_linea, true); 
    CImg<double> fm_rotada = magn_tdf(c_rotada, true); 

    CImgList<double> lista_fft;
    lista_fft.assign(fm_linea, fm_rotada);
    lista_fft.display();

}

void inciso5(const char *input) {
    CImg<double> img(input);
    CImg<double> fm_img = magn_tdf(img,true);
    (img,fm_img).display();
}


int main(int argc, char *argv[]) {

	const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    //inciso3();
    //inciso4();
    inciso5(input);
    return 0;
}
