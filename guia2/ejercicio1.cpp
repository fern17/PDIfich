#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/getValue.cpp"
using namespace cimg_library;   //Necesario


void leerLUTdesdeArchivo(std::string nombre, std::vector<unsigned int> &LUT) {
    std::ifstream f(nombre.c_str());
    if (!f.is_open()) {
        std::cout<<"No se pudo abrir el archivo "<<nombre<<"\n";
        return;
    }
    unsigned int inicio;
    unsigned int final;
    float pendiente;
    int offset;
    LUT.resize(256,0);
    while(f>>inicio>>final>>pendiente>>offset) {
        std::cerr<<"Se leyeron: "<<inicio<<" "<<final<<" "<<pendiente<<" "<<offset<<"\n";
        for(int i = inicio; i < final; i++){
            LUT[i] = utils::getValue(i,inicio,pendiente,offset);
        }
    }
    std::cout<<"\n\n\n";
    f.close();
}

CImg<unsigned char> imagenMapeo(std::vector<unsigned int> &LUT) {
    unsigned int ancho = LUT.size();
    CImg<unsigned char> mapeo(ancho, ancho,1,1,0);
    for (unsigned int i = 0; i < ancho; i++) {
        unsigned int valor = LUT[i];
        mapeo(i, ancho - valor - 1) = 255;
    }
    return mapeo;
}

CImg<unsigned char> aplicarTabla(CImg<unsigned char> img, std::vector<unsigned int> LUT) {
    CImg<unsigned char> resultado(img.width(), img.height());
    cimg_forXY(img, x,y) {
        resultado(x,y) = LUT[img(x,y)];
    }
    return resultado;
}

typedef struct click{
    int x;
    int y;
    void print(bool cr) {
        std::cout<<x<<' '<<y;
        if (cr)
            std::cout<<'\n';
    }
    float norma() {
        return sqrt(x*x + y*y);
    }
    float normaManhattan() {
        return x+y;
    }
}click;

unsigned int CANTIDAD_CLICK = 0;
click click1;
click click2;



//Funcion que toma una imagen cualquiera, y un titulo de ventana, y la muestra
template<typename T>
void disp(CImg<T> mapeo, CImg<T> img, std::string title = "titulo") {
    CImgDisplay ventana(mapeo, title.c_str());   //Crea una ventana y dibuja la imagen...
    
    std::vector<unsigned int> LUT;
    while ( not ventana.is_closed() && not ventana.is_keyQ()) {
        ventana.wait();
        if (ventana.button() && ventana.mouse_y() >= 0 && ventana.mouse_x() >= 0) {
            if (CANTIDAD_CLICK % 2 == 0){
                click1.x = ventana.mouse_x();
                click1.y = 255-ventana.mouse_y();
                std::cout<<"Click1 en "; click1.print(true); 
            } else {
                click2.x = ventana.mouse_x();
                click2.y = 255-ventana.mouse_y();
                std::cout<<"Click2 en "; click2.print(true);
                
                //Swapeo si click2 > click1
                if (click1.normaManhattan() > click2.normaManhattan()) {
                    click temp = click1;
                    click1 = click2;
                    click2 = temp;
                }
                
                //Calculo de las pendientes
                float dx1 = click1.x;
                float dx2 = click2.x - click1.x;
                float dx3 = 255-click2.x;
                float dy1 = click1.y;
                float dy2 = click2.y - click1.y;
                float dy3 = 255 - click2.y;
                float pendiente1 = dy1/dx1;
                float pendiente2 = dy2/dx2;
                float pendiente3 = dy3/dx3;
                unsigned int offset1 = 0;
                unsigned int offset2 = click1.y;
                unsigned int offset3 = click2.y;
                LUT.clear();
                LUT.resize(256,0);
                for (unsigned int i = 0; i < 256; i++) {
                    if (i < click1.x) { //primer tramo
                        LUT[i] = utils::getValue(i,0,pendiente1,offset1);
                    }
                    else if (i < click2.x) { //segundo tramo
                        LUT[i] = utils::getValue(i,click1.x,pendiente2,offset2);
                    }
                    else { //tercer tramo
                        LUT[i] = utils::getValue(i,click2.x,pendiente3,offset3);
                    }
                }
                CImg<unsigned char> nuevo_mapeo = imagenMapeo(LUT);
                CImg<unsigned char> resultado = aplicarTabla(img,LUT);
                
                CImgList<unsigned char> lista;
                lista.push_back(nuevo_mapeo);
                lista.push_back(resultado);
                lista.display();

            }
            CANTIDAD_CLICK++;
        }

    }
}

void inciso4(const char *input, std::string archivo) {
    std::vector<unsigned int> LUT;
    leerLUTdesdeArchivo(archivo,LUT);

    CImg<unsigned char> img(input);
    
    CImg<unsigned char> mapeo;
    mapeo = imagenMapeo(LUT);
    
    CImg<unsigned char> resultado;
    resultado = aplicarTabla(img, LUT);
        
    CImgList<unsigned char> lista;
    lista.push_back(img);
    lista.push_back(mapeo);
    lista.push_back(resultado);
    lista.display();
}



void inciso5(const char *input) {
    CImg<unsigned char> mapeo(255,255,1,1,0);
    cimg_forX(mapeo,x) {
        mapeo(x,255-x) = 255;
    }

    CImg<unsigned char> img(input);
    disp(mapeo, img);

}

int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");
    
    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    std::string archivo = cimg_option("-f","lineal.txt","Archivo de entrada");
    const char* output = cimg_option("-o", "output1.bmp", "Output Image File");
    
    //inciso4(input,archivo);
    inciso5(input);
    

    return 0;
}
