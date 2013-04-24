#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/getValue.cpp"
using namespace cimg_library;   //Necesario


void leerLUTdesdeArchivo(std::string nombre, std::vector<bool> &LUT) {
    std::ifstream f(nombre.c_str());
    if (!f.is_open()) {
        std::cout<<"No se pudo abrir el archivo "<<nombre<<"\n";
        return;
    }
    unsigned int inicio;
    unsigned int final;
    unsigned int valor;
    LUT.resize(256,0);
    while(f>>inicio>>final>>valor) {
        std::cerr<<"Se leyeron: "<<inicio<<" "<<final<<" "<<valor<<"\n";
        for(int i = inicio; i < final; i++){
            LUT[i] = valor;
        }
    }
    std::cout<<"\n\n\n";
    f.close();
}

CImg<bool> imagenMapeo(std::vector<bool> &LUT) {
    unsigned int ancho = LUT.size();
    CImg<bool> mapeo(ancho, ancho,1,1,0);
    for (unsigned int i = 0; i < ancho; i++) {
        if ( LUT[i] )
            mapeo(i, 5 ) = true;
        else
            mapeo(i, 250 ) = true;
    }
    return mapeo;
}

CImg<bool> aplicarTabla(CImg<unsigned char> & img, std::vector<bool> & LUT) {
    CImg<bool> resultado(img.width(), img.height(), 1, 1, 0);
    cimg_forXY(img, x,y) {
        resultado(x,y) = LUT[img(x,y)];
    }
    return resultado;
}


//Funcion que toma una imagen cualquiera, y un titulo de ventana, y la muestra
template<typename T>
void disp(CImg<T> img, std::string title = "titulo") {
    CImgDisplay ventana(img, title.c_str());   //Crea una ventana y dibuja la imagen...
    while ( not ventana.is_closed() && not ventana.is_keyQ()) {
    }
}


CImg<bool> andIMG(CImg<bool> img1, CImg<bool> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) && img2(x,y);
    }    
    return resultado;
}

CImg<bool> notIMG(CImg<bool>  img1) {
    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    cimg_forXY(img1, x, y) {
        resultado(x,y) = !img1(x,y);
    }    
    return resultado;
}

CImg<bool> orIMG(CImg<bool>  img1, CImg<bool> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) or img2(x,y);
    }    
    return resultado;
}

CImg<bool> xorIMG(CImg<bool>  img1, CImg<bool> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) xor img2(x,y);
    }    
    return resultado;
}


CImg<bool> menorIMG(CImg<unsigned char>  img1, CImg<unsigned char> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) < img2(x,y);
    }    
    return resultado;
}

CImg<bool> menorigualIMG(CImg<unsigned char> img1, CImg<unsigned char> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) <= img2(x,y);
    }    
    return resultado;
}

CImg<bool> mayorIMG(CImg<unsigned char>  img1, CImg<unsigned char> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) > img2(x,y);
    }    
    return resultado;
}

CImg<bool> mayorigualIMG(CImg<unsigned char>  img1, CImg<unsigned char> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) >= img2(x,y);
    }    
    return resultado;
}

int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");
    
    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* input2 = cimg_option("-j", "../images/clown.jpg", "Input2 Image File");
    const char* operation = cimg_option("-o", "and", "Operation Type");
    

    std::vector<bool> LUT;
    leerLUTdesdeArchivo("ejercicio5.txt", LUT);
    

    CImg<unsigned char> img1(input);
    CImg<unsigned char> img2(input2);
    CImg<bool> salida;

    CImg<bool> mapeo = imagenMapeo(LUT);
    CImg<bool> img1bool = aplicarTabla(img1, LUT);
    CImg<bool> img2bool = aplicarTabla(img2, LUT);

    disp(img1bool);
    disp(img2bool);
    disp(mapeo);


    if (strcmp(operation,"and") == 0) {
        salida = andIMG(img1bool, img2bool);

    } else if (strcmp(operation,"or") == 0 ) {
        
        salida = orIMG(img1bool, img2bool);

    } else if (strcmp(operation,"not") == 0 ) {
    
        salida = notIMG(img1bool);
    } else if (strcmp(operation,"xor") == 0 ) {

        salida = xorIMG(img1bool, img2bool);

    } else if (strcmp(operation,"menor") == 0 ) {

        salida = menorIMG(img1, img2);

    } else if (strcmp(operation,"menorigual") == 0 ) {

        salida = menorigualIMG(img1, img2);

    } else if (strcmp(operation,"mayor") == 0 ) {

        salida = mayorIMG(img1, img2);

    } else if (strcmp(operation,"mayorigual") == 0 ) {
        salida = mayorigualIMG(img1, img2);

    } else {
        salida = andIMG(xorIMG(img1bool, img2bool), orIMG( notIMG(img1bool), img2bool));

    }
    
    disp(salida);

    return 0;
}
