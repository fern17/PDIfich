#include <CImg.h>
using namespace cimg_library;

int main(int argc, char *argv[]) {
    cimg_usage("Ejemplo basico.");
    const char* filename = cimg_option("-i","cameraman.tif","Image file\n"); //Imagen color, 8 bits de profundidad por componente.
    CImg<unsigned char> img(640,400,1,3);
    img.fill(0); //Asigna 0 a todos los pıxeles
    unsigned char rojo[] = { 255,0,0 }; //Define color rojo
    img.draw_text(100,100,"Hello World",rojo); //Escribe en (100,100).
    //img.display("Hola...");//Visualiza
     
    CImg<unsigned char> img1(filename);//Carga de disco
     
    //Copia lo que hay en img1 pero como valores de punto flotante
    CImg<float> img2(img1);
    img2.normalize(0,1);//Normaliza en [0,1]
   
    img2.sqrt();//Aplica raız cuadrada a cada pixel
   
    //img2.resize(250,150);//Redimensiona a 250x150
   
    //Visualiza ambas imagenes a la vez
    CImgDisplay vent1(img1,"original"), vent2(img2,"procesada");
    while(!vent1.is_closed()){}
    return 0;
}

