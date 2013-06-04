#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

using namespace cimg_library;   //Necesario

//Struct que representa un click de un mouse
typedef struct click{
    int x;
    int y;
    void print(bool cr) {
        std::cout<<x<<' '<<y;
        if (cr)
            std::cout<<'\n';
    }
}click;

unsigned int CANTIDAD_CLICK = 0;
//Primer y segundo click
click click1;
click click2;

//Distancia euclidea entre dos click
float dist(click c1, click c2) {
    float dx = c2.x-c1.x;
    float dy = c2.y-c1.y;
    return sqrt(dx*dx + dy*dy);
}


//@ Registra los dos clicks y al tenerlos, dibuja el histograma del recuadro
template<typename T>
void loopDisplayAndClick(CImg<T> img, std::string title = "titulo") {
    CImgDisplay ventana(img, title.c_str());   //Crea una ventana y dibuja la imagen...
    
    while ( not ventana.is_closed() && not ventana.is_keyQ()) {
        ventana.wait(); //espera eventos
    
        if (ventana.button() && ventana.mouse_y() >= 0 && ventana.mouse_x() >= 0) { 
            //si se apreta un boton del mouse dentro de la ventana
            if (CANTIDAD_CLICK % 2 == 0){ //primer click
                click1.x = ventana.mouse_x();
                click1.y = ventana.mouse_y();
                std::cout<<"Click1 en "; click1.print(true); 
            } else {    //segundo click
                click2.x = ventana.mouse_x();
                click2.y = ventana.mouse_y();
                std::cout<<"Click2 en "; click2.print(true);
                
                //Como ya tenemos dos clicks, estamos listos para calcular el histograma
                CImg<T> cropeada = img.get_crop(click1.x, click1.y, click2.x, click2.y);
                CImg<T> histograma = cropeada.get_histogram(256);
            
                //Dibuja el histograma y la imagen cropeada
                CImgDisplay ventana2 (cropeada, "Corte");   
                histograma.display_graph(0,3);
            }
            CANTIDAD_CLICK++;
            if (CANTIDAD_CLICK == 2) //acotamos para evitar overflow
                CANTIDAD_CLICK = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    
    const char* input  = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* output = cimg_option("-o", "output1.bmp", "Output Image File");

    CImg<unsigned char> img(input);
    loopDisplayAndClick(img);

    return 0;
}
