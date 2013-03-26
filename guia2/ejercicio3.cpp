#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/getValue.cpp"
using namespace cimg_library;   //Necesario



CImg<bool> drawcircle(unsigned int lado, unsigned int radio) {
    CImg<bool> img(lado,lado);

    unsigned int centro = lado/2;

    cimg_forXY(img,x,y) {
        if (  (x-centro)*(x-centro) + (y-centro)*(y-centro) <= radio*radio  )
            img(x,y) = true;
        else
            img(x,y) = false;
    }
    return img;
}


//Funcion que toma una imagen cualquiera, y un titulo de ventana, y la muestra
template<typename T>
void disp(CImg<T> img, std::string title = "titulo") {
    CImgDisplay ventana(img, title.c_str());   //Crea una ventana y dibuja la imagen...
    while ( not ventana.is_closed() && not ventana.is_keyQ()) {
    }
}

CImg<unsigned char> sumarImg(CImg<unsigned char> & img1, CImg<unsigned char> & img2) {
    assert( img1.is_sameXY(img2) );

    CImg<unsigned char> salida(img1.width(), img1.height(), 1, 1, 0);

    cimg_forXY(img1,x,y) {
        salida(x,y) = ( img1(x,y) + img2(x,y) ) / 2;
    }

    return salida;
}

CImg<unsigned char> restarImg(CImg<unsigned char> & img1, CImg<unsigned char> & img2) {
    assert( img1.is_sameXY(img2) );

    CImg<unsigned char> salida(img1.width(), img1.height(), 1, 1, 0);

    int temporal;
    cimg_forXY(img1,x,y) {
        temporal = ( img1(x,y) - img2(x,y) ) / 2;
        if (temporal < 0) temporal = 0;
        else if (temporal > 255) temporal = 255;
        
        salida(x,y) = temporal;
    }

    return salida;
}


CImg<unsigned char> multiplicarImg(CImg<unsigned char> & img1, CImg<bool> & mascara) {
    assert( img1.is_sameXY(mascara) );

    CImg<unsigned char> salida(img1.width(), img1.height(), 1, 1, 0);

    cimg_forXY(img1,x,y) {
        salida(x,y) = ( img1(x,y) * (unsigned int) mascara(x,y) );
    }

    return salida;
}

CImg<unsigned char> dividirImg(CImg<unsigned char> & img1, CImg<bool> & mascara) {
    assert( img1.is_sameXY(mascara) );

    CImg<unsigned char> salida(img1.width(), img1.height(), 1, 1, 0);

    cimg_forXY(img1,x,y) {
        salida(x,y) = ( img1(x,y) * (unsigned int) !mascara(x,y) );
    }

    return salida;
}

CImg<unsigned char> limpiarRuido(std::vector<CImg<unsigned char> > imagenes) {
    unsigned int n = imagenes.size();
    assert(n != 0);
    
    CImg<unsigned char> salida(imagenes[0].width(), imagenes[0].height(), 1, 1, 0);


    int temporal;   
    for (unsigned int i = 0; i < n; i++) {
        cimg_forXY(imagenes[i],x,y) {
            temporal = salida(x,y) + floor(imagenes[i](x,y) / n);
            if (temporal > 255) temporal = 255;
            else if (temporal < 0 ) temporal = 0; 
            salida(x,y) = temporal;
        }
    }

    return salida;
}


int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");
    
    //Igual Size
    // cameraman.tiff
    // clown.jpg    
    // coins.tif
    // hand.tif
    // huang3.jpg


    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* input2 = cimg_option("-j", "../images/clown.jpg", "Input2 Image File");
    const char* operation = cimg_option("-o", "suma", "Operation between images");
    const unsigned int cant_imagenes = cimg_option("-n", 50, "Cantidad de imagenes ruidosas");
    const float ruido = cimg_option("-r", sqrt(0.05), "Sigma de Ruido");

    CImg<unsigned char> img1(input);
    CImg<unsigned char> img2(input2);
    CImg<unsigned char> salida;

    if (strcmp(operation,"division") == 0) {
        
        CImg<bool> mascara = drawcircle(img1.width() , img1.width()/4 );
        salida = dividirImg(img1, mascara);
        
    } else if (strcmp(operation,"resta") == 0 ) {
        
        salida = restarImg(img1, img2);

    } else if (strcmp(operation,"multiplicacion") == 0 ) {
        
        CImg<bool> mascara = drawcircle(img1.width() , img1.width()/4 );
        salida = multiplicarImg(img1, mascara);

    } else if (strcmp(operation,"ruido") == 0 ) {

        std::vector<CImg<unsigned char> > imagenes;

        for ( unsigned int i = 0; i < cant_imagenes; i++ ) {
            CImg<unsigned char> imgtmp(img1);
            imgtmp.noise( 3 , 0 );
            imagenes.push_back(imgtmp);
        }

        img2 = imagenes[25];
        salida = limpiarRuido(imagenes);

    } else {
        salida = sumarImg(img1, img2);

    }

    CImgList<unsigned char> lista;
    lista.assign(img1, img2, salida);

    lista.display();

    return 0;
}
