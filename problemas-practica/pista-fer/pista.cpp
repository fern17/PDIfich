#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <algorithm>
#include "../../utils/PDI_functions.h"
#include "../../utils/coordHoughToImg.cpp"
using namespace cimg_library;   //Necesario

CImg<unsigned char> filtroMediaAlfaRecortado(CImg<unsigned char> img, unsigned int talla = 3, unsigned int alfa = 3) {
    CImg<unsigned char> retval(img.width(), img.height(), 1, 1, 0);
    cimg_forXY(img, x, y) {
        CImg<unsigned char> vecindario = img.get_crop(x - talla/2, y - talla/2, x + talla/2, y + talla/2);
        std::vector<unsigned char> vec;
        cimg_forXY(vecindario, u, v) {
            vec.push_back(vecindario(u,v));
        }
        std::sort(vec.begin(), vec.end());
        std::vector<unsigned char> copia;
        unsigned int value = 0.0;
        //copiamos solo los que no quedan dentro del alfa
        for (unsigned int i = alfa/2; i < vec.size() - alfa/2; i++) {
            value += vec[i];
            copia.push_back(vec[i]);
        }

        retval(x,y) = value / copia.size();
    }
    return retval;
}
CImg<double> binarizar(CImg<double> img, double umbral) {
    CImg<double> retval(img.width(), img.height(), 1,1,0.0);
    cimg_forXY(img, x, y) {
        if (img(x,y) > umbral)
            retval(x,y) = 255.0;
    }
    return retval;
}

int main(int argc, char *argv[]) {
 	const char* _input = cimg_option("-i", "../img/cervezas/01.jpg", "Input Image File");

 	const unsigned char umbral = cimg_option("-u", 100.0, "Umbral de permiso para getzonainteres");
 	const unsigned int talla = cimg_option("-t", 5, "Umbral de permiso para getzonainteres");
 	const unsigned int alfa = cimg_option("-a", 10, "Umbral de permiso para getzonainteres");

    unsigned char rojo[] = {255, 0, 0};
    unsigned char verde[] = {0,255,0};
    unsigned char azul[] = {0,0,255};
    unsigned char blanco[] = {255,255,255};

 	std::vector<std::string> archivos;

 	archivos.push_back("../img/pista/01.jpg");
 	archivos.push_back("../img/pista/02.jpg");


 	for (int i = 0; i < archivos.size(); ++i)	{
        CImg<unsigned char> imagen(archivos[i].c_str());
        unsigned int h = imagen.height();
        unsigned int w = imagen.width();
        CImg<unsigned char> filtrada = filtroMediaAlfaRecortado(imagen, talla, alfa);
        CImgList<double> gradientes = filtrada.get_gradient("xy",2);
        CImg<double> gradiente = gradientes[0];
        gradiente.normalize(0.0,255.0);
        
        CImg<double> binaria = binarizar(gradiente, umbral);
        CImg<double> fourier = binaria.get_FFT()[0].normalize(0,255);
        fourier.display();
        CImg<double> t_hough = hough(fourier);
        double maximo = t_hough.max();
        double tita = 0;
        double rho = 0;

        cimg_forXY(t_hough, x, y) {
            if (fabs(maximo - t_hough(x,y)) < 0.001) {
                tita = double(x);
                rho = double(y);
                break;
            }
        }
        std::vector<unsigned int> coord_recta = utils::coordHoughToImg(imagen, tita, rho);

        filtrada.draw_line(coord_recta[0],coord_recta[1],coord_recta[2],coord_recta[3], rojo);

        (imagen, filtrada, binaria, t_hough
         ).display();
 	}

    return 0;
}
