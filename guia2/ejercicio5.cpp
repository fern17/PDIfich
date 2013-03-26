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


CImg<bool> andIMG(CImg<bool> & img1, CImg<bool> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) && img2(x,y);
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

    salida = andIMG(img1bool, img2bool);
    disp(salida);

    // if (strcmp(operation,"and") == 0) {
        
    //     CImg<bool> mascara = drawcircle(img1.width() , img1.width()/4 );
    //     salida = dividirImg(img1, mascara);
        
    // } else if (strcmp(operation,"resta") == 0 ) {
        
    //     salida = restarImg(img1, img2);

    // } else if (strcmp(operation,"multiplicacion") == 0 ) {
        
    //     CImg<bool> mascara = drawcircle(img1.width() , img1.width()/4 );
    //     salida = multiplicarImg(img1, mascara);

    // } else if (strcmp(operation,"ruido") == 0 ) {

    //     std::vector<CImg<unsigned char> > imagenes;

    //     for ( unsigned int i = 0; i < cant_imagenes; i++ ) {
    //         CImg<unsigned char> imgtmp(img1);
    //         imgtmp.noise( 3 , 0 );
    //         imagenes.push_back(imgtmp);
    //     }

    //     img2 = imagenes[25];
    //     salida = limpiarRuido(imagenes);

    // } else {
    //     salida = sumarImg(img1, img2);

    // }

    // CImgList<unsigned char> lista;
    // lista.assign(img1, img2, salida);

    // lista.display();

    // return 0;
}
