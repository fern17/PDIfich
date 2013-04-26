#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/getValue.cpp"
#include "../utils/click.h"
using namespace cimg_library;   //Necesario

//@ Lee los parametros de una transformacion lineal por tramos y devuelve una LUT en un vector
void leerTransformacionLineal(std::string nombre, std::vector<unsigned char> &LUT) {
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
            LUT[i] = utils::getLinearValue(i,inicio,pendiente,offset);
        }
    }
    std::cout<<"\n\n\n";
    f.close();
}

//@ Devuelve una imagen que representa el mapeo de LUT
CImg<unsigned char> imagenMapeo(std::vector<unsigned char> LUT) {
    unsigned int ancho = LUT.size();
    CImg<unsigned char> mapeo(ancho, ancho,1,1,0);
    for (unsigned int i = 0; i < ancho; i++) {
        unsigned char valor = LUT[i];
        mapeo(i, ancho - valor - 1) = 255;
    }
    return mapeo;
}

//@ Aplica a img la LUT pasada por parametro
CImg<unsigned char> aplicarLUT(CImg<unsigned char> img, std::vector<unsigned char> LUT) {
    CImg<unsigned char> resultado(img.width(), img.height());

    cimg_forXY(img, x,y) {
        resultado(x,y) = LUT[img(x,y)];
    }
    return resultado;
}

unsigned int CANTIDAD_CLICK = 0;
utils::Click click1;
utils::Click click2;

//@ Captura la posicion de 2 clicks y crea una LUT que pase por la recta que definan. Aplica la Lut a la imagen
template<typename T>
void pendienteLinealInteractiva(CImg<T> mapeo, CImg<T> img, std::string title = "titulo") {
    CImgDisplay ventana(mapeo, title.c_str());   //Crea una ventana y dibuja la imagen...
    
    std::vector<unsigned char> LUT;
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
                    utils::Click temp = click1;
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
                        LUT[i] = utils::getLinearValue(i,0,pendiente1,offset1);
                    }
                    else if (i < click2.x) { //segundo tramo
                        LUT[i] = utils::getLinearValue(i,click1.x,pendiente2,offset2);
                    }
                    else { //tercer tramo
                        LUT[i] = utils::getLinearValue(i,click2.x,pendiente3,offset3);
                    }
                }
                CImg<unsigned char> nuevo_mapeo = imagenMapeo(LUT);
                CImg<unsigned char> resultado = aplicarLUT(img,LUT);
                
                CImgList<unsigned char> lista;
                lista.push_back(nuevo_mapeo);
                lista.push_back(resultado);
                lista.display();

            }
            CANTIDAD_CLICK++;
        }
    }
}


//@ Le aplica la transformada lineal a img, leida desde input
void inciso4( const char *input_img, const char *input_lut) {
    CImg<unsigned char> img (input_img);
    //Vector que guarda el mapeo
    std::vector<unsigned char> LUT;

    //Leemos el mapeo
    leerTransformacionLineal(input_lut, LUT);

    //Aplicamos el mapeo
    CImg<unsigned char> resultado = aplicarLUT(img, LUT);

    //Generamos la imagen del mapeo para graficar
    CImg<unsigned char> imagen_mapeo = imagenMapeo(LUT);

    //Dibujamos
    (img, imagen_mapeo, resultado).display("Imagen Original, Mapeo Aplicado y Resultado");
}

//@ Crea una imagen binaria con la linea identidad. Luego toma entrada del usuario para cambiar la pendiente
void inciso5(const char *input) {
    CImg<unsigned char> mapeo(255,255,1,1,0);
    cimg_forX(mapeo,x) {
        mapeo(x,255-x) = 255;
    }

    CImg<unsigned char> img(input);
    pendienteLinealInteractiva(mapeo, img);

}

int main(int argc, char *argv[]) {
    //@ Aplicacion de transformadas lineales y transformadas lineales por tramos a imagenes
    cimg_usage("Transformaciones Lineales");
    
    const char* input_img = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* input_archivo = cimg_option("-f","lineal.txt","Archivo de entrada");

    inciso4(input_img, input_archivo);
    
    inciso5(input_img);

    return 0;
}
