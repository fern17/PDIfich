#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/PDI_functions.h"

using namespace cimg_library;   //Necesario

CImg<float> get_filtro(std::string nombre) {
    std::ifstream f(nombre.c_str());
    if (!f.is_open()) {
        std::cout<<"No se pudo abrir el archivo "<<nombre<<"\n";
        exit(-1);
    }
    unsigned int filas;
    unsigned int columnas;
    float valor;

    f>>filas;
    f>>columnas;
    CImg<float> salida(columnas, filas, 1 , 1, 0);
    for (unsigned int i = 0; i < filas; i++) { 
        for (unsigned int j = 0; j < columnas; j++) {
            f>>valor;
            // std::cerr<<"wololo"<<valor<<"\n";
            salida(j,i) =  valor;
            std::cout<<salida(j,i)<<' ';
        }
        std::cout<<'\n';
    }
    f.close();
    return salida;
}

int main(int argc, char *argv[]) {
    //@ Compara el resultado de ecualizar una imagen a partir de cada canal RGB y la intensidad de HSI

    const char* _input = cimg_option("-i", "../images/camino.tif", "Input Image File");
    const char* _filtro = cimg_option("-f", "filtro_palto_suma1_9.txt", "Input Image File");


    //Declaramos imagenes a trabajar
    CImg<double> input(_input) ;
    CImg<double> output_RGB(_input);
    CImg<double> output_HSI(_input);

    CImg<double> filtro = get_filtro(_filtro);

        //Temporales necesarios
    CImg<double> c1, c2, c3;
    
    //Ecualización de la RGB
        //Obtenemos los canales
    c1 = output_RGB.get_channel(0);
    c2 = output_RGB.get_channel(1);
    c3 = output_RGB.get_channel(2);
        //Los ecualizamos
    c1.convolve(filtro);
    c2.convolve(filtro);
    c3.convolve(filtro);
        //Recomponemos la imágen
    c1.append(c2, 'c');
    c1.append(c3, 'c');
    output_RGB = c1;


    //Ecualizamos la imagen HSI
    output_HSI.RGBtoHSI();
          //Obtenemos los canales
    c1 = output_HSI.get_channel(0);
    c2 = output_HSI.get_channel(1);
    c3 = output_HSI.get_channel(2);
        //Ecualizo el canal de Intensidad solamente
    c3.convolve(filtro);
        //Recomponemos la imágen
    c1.append(c2, 'c');
    c1.append(c3, 'c');
    output_HSI = c1;
    output_HSI.HSItoRGB();


    //Display! 
    CImgList<double> lista;

    lista.assign(input, output_RGB, output_HSI );
    lista.display();



    return 0;
}
