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

void listaImagenes() {
    //Define las imagenes que se van a agregar a la lista
    CImg<unsigned char> img1("camaleon.tif");
    CImg<unsigned char> img2("clown.jpg");
    CImg<unsigned char> img3("coins.tif");
    CImg<unsigned char> img4("cameraman.tif");
    CImgList<unsigned char> lista_img;              //Construye la lista vacia
    lista_img.assign(img1,img2,img3,img4);          //Asigna las imagenes a la lista. Maximo 8.

    lista_img.display();                            //Dibuja la lista de imagenes
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

    //Lee una lista de imagenes y las muestra en una sola ventana
    listaImagenes();


    return 0;
}
