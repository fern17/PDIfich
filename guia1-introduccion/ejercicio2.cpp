#include <CImg.h>               //include basico de CImg
#include <iostream>
using namespace cimg_library;   //Necesario

//@ Carga distintas imagenes, las agrega a una CImgList y la muestra
void listaImagenes() {
    //Define las imagenes que se van a agregar a la lista
    CImg<unsigned char> img1("../images/camaleon.tif");
    CImg<unsigned char> img2("../images/clown.jpg");
    CImg<unsigned char> img3("../images/coins.tif");
    CImg<unsigned char> img4("../images/cameraman.tif");
    CImgList<unsigned char> lista_img;              //Construye la lista vacia
    lista_img.assign(img1,img2,img3,img4);          //Asigna las imagenes a la lista. Maximo 8.

    lista_img.display("Lista de imagenes");                            //Dibuja la lista de imagenes
}

int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Dibujado de varias imagenes en una misma ventana con CImgList");
    
    //Lee una lista de imagenes y las muestra en una sola ventana
    listaImagenes();
    return 0;
}
