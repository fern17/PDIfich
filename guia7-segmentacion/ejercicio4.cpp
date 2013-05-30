#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <set>
#include <map>
#include "../utils/PDI_functions.h"

using namespace cimg_library;   //Necesario


//Devuelve si un valor esta dentro de un rango
bool inRange(double value, double min, double max) {
    if (min > max) {
        double tempy = min;
        min = max;
        max = tempy;
    }
    return (value>min && value<max);
}

//@ Dada una imágen, hacemos un color slice y luego segmentamos utilizando etiquetado de componentes conexas y con eso calculamos la cantidad de clases y dibujamos un circulo en c/u de ellas
int main(int argc, char *argv[]) {
  const char* input = cimg_option("-i", "../images/rosas.jpg", "Input Image File");
  const unsigned char _umbral = cimg_option("-u", 180 , "Input Image File");
  const unsigned char _talla = cimg_option("-t", 11 , "Input Image File");
  const unsigned char _radio = cimg_option("-r", 20 , "Input Image File");

  CImg<double> imagen(input);

  //@ Definición de colores por ejemplo: color azúl
  unsigned char color_azul[3] = {0, 0 , 255 };

  
  // imagen.crop(215,256,253,284);

  // CImg<double> r = imagen.get_channel(0);
  // CImg<double> g = imagen.get_channel(1);
  // CImg<double> b = imagen.get_channel(2);

  // r.get_histogram(256).display_graph("Canal 0", 3);
  // g.get_histogram(256).display_graph("Canal 1", 3);
  // b.get_histogram(256).display_graph("Canal 2", 3);

  //Imagen para calculos auxiliares que resuelven el error en el sliceo de la flor
  CImg<unsigned char> tempy(imagen.width(), imagen.height(), imagen.depth(), 1, 0);

  cimg_forXY(imagen, x , y) {
    if ( inRange(imagen(x,y,0,0), 90, 212) && inRange(imagen(x,y,0,1),0 , 56 ) && inRange(imagen(x,y,0,2), 10, 100) ) {
        tempy(x,y) = 255;
    }

  }
  //Con un blureo va a andar
  CImg<double> mascara(_talla, _talla ,1,1, 1.0f/pow(_talla,2) );
  tempy.convolve(mascara);
  cimg_forXY(tempy, x , y) {
    if (tempy(x,y) > _umbral) {
        tempy(x,y) = 255;
    } else {
        tempy(x,y) = 0;
    }

  }
  //Etiquetado de componentes conexas.
  tempy = label_cc(tempy, 255).display();

  std::set<unsigned char> unicos;
  cimg_forXY(tempy, x, y) {
    unicos.insert(tempy(x,y));  
  }

  //Se le resta uno por el fondo
  std::cout<<"Cantidad de clases Detectadas:"<<unicos.size()-1<<"\n";


  std::map<unsigned char, std::vector<unsigned int> > mapa;

  cimg_forXY(tempy, x ,y ) {
    unsigned char clase = tempy(x,y);
    if (clase == 0) //ES EL FONDO
        continue;
    if (mapa.find(clase) != mapa.end() ) { //Se encontró => actualizar
        if (mapa[clase][0] > x)
            mapa[clase][0] = x; //xmin
        if (mapa[clase][1] < x)
            mapa[clase][1] = x; //xmax
        if (mapa[clase][2] > y)
            mapa[clase][2] = y; //ymin
        if (mapa[clase][3] < y)
            mapa[clase][3] = y; //ymax
    } else {
        std::vector<unsigned int> vtemp;
        vtemp.push_back(x);
        vtemp.push_back(x);
        vtemp.push_back(y);
        vtemp.push_back(y);
        mapa[clase] = vtemp; 
    }

  }

  std::map<unsigned char, std::vector<unsigned int> >::iterator p = mapa.begin();

  while(p != mapa.end() ) {
    unsigned int cx = ( p->second[0] + p->second[1] ) / 2;
    unsigned int cy = ( p->second[2] + p->second[3] ) / 2;
    cout<<"Clase: "<<(unsigned int)p->first<<"\tCentro: "<<cx<<" "<<cy<<"\n";
    imagen.draw_circle(cx,cy, _radio, color_azul, 100 );
    p++;
  }


  (imagen, tempy).display();


  return 0;
}


