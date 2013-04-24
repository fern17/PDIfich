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

bool dentro_circulo(double valor, double centro, double radio) {
    return ( pow(valor-centro,2) < radio*radio );
}

int main(int argc, char *argv[]) {
    //@ Compara el resultado de ecualizar una imagen a partir de cada canal RGB y la intensidad de HSI

    const char* _input = cimg_option("-i", "../images/futbol.jpg", "Input Image File");


    //Declaramos imagenes a trabajar
    CImg<double> input(_input), output(input.width(), input.height(), input.depth(), 3 , 0) ;

    (input.get_RGBtoHSI().get_channel(0), input.get_RGBtoHSI().get_channel(1)).display();


    CImg<double> recorte = input.get_crop(132,105,203,230);
    // recorte.display();

    // CImg<unsigned char> histograma_r = recorte.get_channel(0).get_histogram(256, 0, 255);
    // CImg<unsigned char> histograma_g = recorte.get_channel(1).get_histogram(256, 0, 255);
    // CImg<unsigned char> histograma_b = recorte.get_channel(2).get_histogram(256, 0, 255);
    // histograma_r.display_graph("",3);
    // histograma_g.display_graph("",3);
    // histograma_b.display_graph("",3);


    CImg<bool> mascara_binaria(input.width(), input.height());

    CImg<double> c1 = input.get_channel(0);
    CImg<double> c2 = input.get_channel(1);
    CImg<double> c3 = input.get_channel(2);
    
    cimg_forXY(input, x , y) {
        if (dentro_circulo(c1(x,y), 40, 20) &&  //rojo
            dentro_circulo(c2(x,y), 85, 10) &&  //verde
            dentro_circulo(c3(x,y), 150, 105)) { //azul

            mascara_binaria(x,y) = true;
            
            output(x,y,0,0) = input(x,y,0,0);
            output(x,y,0,1) = input(x,y,0,1);
            output(x,y,0,2) = input(x,y,0,2);
        } else {
            mascara_binaria(x,y) = false;
        }
    }

        // //Display! 
    CImgList<double> lista;

    lista.assign(input, mascara_binaria.normalize(0,255) , output );
    lista.display();

    // CImg<double> output_RGB(_input);
    // CImg<double> output_HSI(_input);

    // CImg<double> filtro = get_filtro(_filtro);

    //     //Temporales necesarios
    // CImg<double> c1, c2, c3;
    
    // //Ecualización de la RGB
    //     //Obtenemos los canales
    // c1 = output_RGB.get_channel(0);
    // c2 = output_RGB.get_channel(1);
    // c3 = output_RGB.get_channel(2);
    //     //Los ecualizamos
    // c1.convolve(filtro);
    // c2.convolve(filtro);
    // c3.convolve(filtro);
    //     //Recomponemos la imágen
    // c1.append(c2, 'c');
    // c1.append(c3, 'c');
    // output_RGB = c1;


    // //Ecualizamos la imagen HSI
    // output_HSI.RGBtoHSI();
    //       //Obtenemos los canales
    // c1 = output_HSI.get_channel(0);
    // c2 = output_HSI.get_channel(1);
    // c3 = output_HSI.get_channel(2);
    //     //Ecualizo el canal de Intensidad solamente
    // c3.convolve(filtro);
    //     //Recomponemos la imágen
    // c1.append(c2, 'c');
    // c1.append(c3, 'c');
    // output_HSI = c1;
    // output_HSI.HSItoRGB();






    // return 0;
}
