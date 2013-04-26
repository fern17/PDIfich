#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
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

//@ Operador AND logico entre dos imagenes booleanas
CImg<bool> andIMG(CImg<bool> img1, CImg<bool> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    //Aplica el operador AND
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) and img2(x,y);
    }    
    return resultado;
}

//@ Operador NOT logico a una imagen
CImg<bool> notIMG(CImg<bool>  img1) {
    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    //Aplica el operador NOT
    cimg_forXY(img1, x, y) {
        resultado(x,y) = !img1(x,y);
    }    
    return resultado;
}


//@ Operador OR logico entre dos imagenes booleanas
CImg<bool> orIMG(CImg<bool>  img1, CImg<bool> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
   
    //Aplica el operador or
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) or img2(x,y);
    }    
    return resultado;
}

//@ Operador XOR logico entre dos imagenes booleanas
CImg<bool> xorIMG(CImg<bool>  img1, CImg<bool> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    //Aplica el operador XOR
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) xor img2(x,y);
    }    
    return resultado;
}

//@ Operador menor < entre dos imagenes en escala de grises
CImg<bool> menorIMG(CImg<unsigned char>  img1, CImg<unsigned char> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
   
    //Aplicacion del operador
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) < img2(x,y);
    }    
    return resultado;
}

//@ Operador menor igual <= entre dos imagenes en escala de grises
CImg<bool> menorigualIMG(CImg<unsigned char> img1, CImg<unsigned char> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    //Aplicacion del operador
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) <= img2(x,y);
    }    
    return resultado;
}

//@ Operador mayor > entre dos imagenes en escala de grises
CImg<bool> mayorIMG(CImg<unsigned char>  img1, CImg<unsigned char> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    //Aplicacion del operador
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) > img2(x,y);
    }    
    return resultado;
}

//@ Operador mayor igual >= entre dos imagenes en escala de grises
CImg<bool> mayorigualIMG(CImg<unsigned char>  img1, CImg<unsigned char> img2) {
    assert(img1.is_sameXY(img2));

    CImg<bool> resultado(img1.width(), img1.height(), 1, 1, 0 );
    
    //Aplicacion del operador
    cimg_forXY(img1, x, y) {
        resultado(x,y) = img1(x,y) >= img2(x,y);
    }    
    return resultado;
}

int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Operadores booleanos aplicados a imagenes");

    
    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* input2 = cimg_option("-j", "../images/clown.jpg", "Input2 Image File");
    const char* operation = cimg_option("-o", "", "Operation Type");
    

    //Cargamos dos imagenes
    CImg<unsigned char> img1(input);
    CImg<unsigned char> img2(input2);
    CImg<bool> salida;

    //Cargamos el mapeo
    std::vector<bool> LUT;
    leerLUTdesdeArchivo("ejercicio5.txt", LUT);
    
    //Creamos la imagen del mapeo
    CImg<bool> mapeo = imagenMapeo(LUT);

    //Aplicamos el mapeo
    CImg<bool> img1bool = aplicarTabla(img1, LUT);
    CImg<bool> img2bool = aplicarTabla(img2, LUT);

    //Dibujamos
    (mapeo.get_normalize(0,255), img1bool.get_normalize(0,255), img2bool.get_normalize(0,255)).display();


    //Aplicacion de operadores booleanos
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

        //operacion compuesta
        salida = andIMG(xorIMG(img1bool, img2bool), orIMG( notIMG(img1bool), img2bool));

    }
   
    salida.display();

    return 0;
}
