#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "../utils/PDI_functions.h"

using namespace cimg_library;   //Necesario


int main(int argc, char *argv[]) {
    //@ Compara el resultado de ecualizar una imagen a partir de cada canal RGB y la intensidad de HSI

    const char* _input = cimg_option("-i", "../images/chairs_oscura.jpg", "Input Image File");


    //Declaramos imagenes a trabajar
    CImg<double> input(_input) ;
    CImg<double> desired("../images/chairs.jpg") ;
    CImg<double> output_RGB(_input);
    CImg<double> output_HSI(_input);

        //Temporales necesarios
    CImg<double> c1, c2, c3;
    
    //Ecualización de la RGB
        //Obtenemos los canales
    c1 = output_RGB.get_channel(0);
    c2 = output_RGB.get_channel(1);
    c3 = output_RGB.get_channel(2);
        //Los ecualizamos
    c1.equalize(256,0,255);
    c2.equalize(256,0,255);
    c3.equalize(256,0,255);
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
    c3.equalize(256,0,1);
        //Recomponemos la imágen
    c1.append(c2, 'c');
    c1.append(c3, 'c');
    output_HSI = c1;
    output_HSI.HSItoRGB();


    //Display! 
    CImgList<double> lista;

    lista.assign(input, desired, output_RGB, output_HSI );
    (output_RGB - output_HSI).RGBtoHSI().get_channel(2).display();
    lista.display();



    return 0;
}
