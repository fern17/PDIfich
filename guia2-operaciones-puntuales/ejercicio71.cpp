#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cassert>
#include <vector>
#include "../utils/LUT.cpp"
using namespace cimg_library;   //Necesario


//@ Crea una máscara de tamaño WxH que tiene en blanco sólo un rectangulo entre (x0,y0) y (x1,y1)
CImg<bool> mascaraRectangular(unsigned int w, unsigned int h,
							unsigned int x0, unsigned int y0, 
							unsigned int x1, unsigned int y1) {
	//Mascara de salida
	CImg<bool> salida(w,h,1,1,0);


    //Si los puntos vienen al reves
	if (x0 + y0 > x1 + y1) {
		//Punto 1 esta mas cerca del origen => intercambio
		unsigned int tmp;
		tmp = x0;
		x0 = x1;
		x1 = tmp;

		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

    //Ponemos en true los valores dentro del rectangulo
	for (unsigned int x = x0; x < x1 && x < w; x++ ) {
		for (unsigned int y=y0; y < y1 && y < h; y++) {
			salida(x,y) = true;
		}
	}
	return salida;

}

//Enmascara una imagen con la mascara dada
CImg<unsigned char> aplicarMascara(CImg<unsigned char> & img1, CImg<bool> & mascara) {
    assert( img1.is_sameXY(mascara) );

    CImg<unsigned char> salida(img1.width(), img1.height(), 1, 1, 0);

    cimg_forXY(img1,x,y) {
        salida(x,y) = ( img1(x,y) * (unsigned int) mascara(x,y) );
    }

    return salida;
}


int main(int argc, char *argv[]) {

    cimg_usage("Enmascaramiento de una parte de la imagen y aplicacion de transformada logaritmo");
    
    const char* input = cimg_option("-i", "../images/earth.bmp", "Input Image File");
    const float c = cimg_option("-c", 10.0, "Constant of logarithmicLUT");

    //Leemos la imagen
    CImg<unsigned char> img(input);

    CImg<unsigned char> resultado;

    //Creamos una mascara del mismo tamaño de la imagen
    CImg<bool> mascara = mascaraRectangular(img.width() , img.height() , 437, 0, img.width() , 237);
    
    //Aplicamos la mascara a la imagen
    CImg<unsigned char> mascara_imagen = aplicarMascara(img , mascara);

    //Aplicamos una transformacion logaritmo a la imagen enmascarada (las partes negras no las cambia)
    resultado = utils::aplicarLUT(mascara_imagen, utils::logarithmicLUT(c ));

    //Sumamos las dos imagenes
    resultado = resultado + img;

    //Dibujamos
    CImgList<unsigned char> lista;
    lista.assign(img, resultado);
    lista.display();

    return 0;
}
