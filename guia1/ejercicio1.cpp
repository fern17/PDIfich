#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
using namespace cimg_library;   //Necesario


//Funcion que toma una imagen cualquiera, y un titulo de ventana, y la muestra
template<typename T>
void disp(CImg<T> img, std::string title = "titulo") {
    CImgDisplay ventana(img, title.c_str());   //Crea una ventana y dibuja la imagen...
    while ( not ventana.is_closed() && not ventana.is_keyQ()) {
        //bucle infinito
    }
}

//Toma un nombre de imagen en input, recorta desde (x0,y0) hasta (xf,yf) y la guarda en output
void cortarImagen(const char *input, const char *output, unsigned int x0, unsigned int y0, unsigned int xf, unsigned int yf) {
    CImg<unsigned char> img(input);
    CImg<unsigned char> img2 = img.get_crop(x0,y0,xf,yf);   //Corta la imagen en el rango especificado.
    img2.print("info de la cortada",1);
    disp(img,"original");
    disp(img2,"cortada");
    img2.save(output);
}

int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");
    
    //Especifica parámetros para el ejecutable.
    // -i: opcion
    // "cameraman.tif": valor por defecto
    // "Input..": descripcion de la opcion, para usarla con -h
    //
    // Para usar como: ejercicio1.bin -i "lena.jpg" -o "salida.jpg"
    const char* input  = cimg_option("-i", "cameraman.tif", "Input Image File");
    const char* output = cimg_option("-o", "output1.bmp", "Output Image File");

    //Corta una imagen en las posiciones dadas, y la guarda en output
    cortarImagen(input,output,95,33,135,80);


    return 0;
}
