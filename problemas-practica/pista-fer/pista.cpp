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

        CImg<double> t_hough = hough(binaria);
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
        std::vector<unsigned int> val = utils::coordHoughToImg(imagen, tita, rho);
        double step_tita = M_PI/double(imagen.width()-1.0);
        double diagonal = std::sqrt(std::pow(imagen.width()-1,2) + std::pow(imagen.height()-1,2));
        double step_rho = 2.0*diagonal/double(imagen.height()-1);
        //Actualiza tita y rho a valores reales
        tita = tita*step_tita - M_PI/2.0; 
        rho = rho*step_rho - diagonal;

        unsigned int x0;
        unsigned int x1;
        unsigned int y0;
        unsigned int y1;
        unsigned int M = imagen.width();
        unsigned int N = imagen.height();

        if (tita>-M_PI/2 && tita<M_PI/2){
            y0 = 0; 
            y1 = M-1;
            x0 = rho/cos(tita);      // calcula y para y=0
            x1 = rho/cos(tita)-(M-1)*tan(tita); // calcula y para y=M-1	  
	    }else{
	        x0 = 0; 
            x1 = N-1;
            y0 = rho/sin(tita);      // calcula y para x=0
            y1 = rho/sin(tita)-(N-1)/tan(tita); // calcula y para x=M-1
	    }
	//cout<<endl<<"("<<t<<","<<r<<")->("<<theta<<","<<rho<<") "<<"("<<y0<<","<<x0<<")->("<<y1<<","<<x1<<")"<<endl;
        //iHough.draw_line(y0,x0,y1,x1,blanco); // dibuja una línea de (0,y0) a (M-1,y1)
/*
        //double mm = -cos(tita)/sin(tita);
        //double bb = rho/sin(tita);
        double mm = -sin(tita)/cos(tita);
        double bb = rho/cos(tita);
        int y0 = trunc(bb);
        int yf = trunc(bb - double(imagen.height())*mm);
        */
        filtrada.draw_line(y0,x0,y1,x1, rojo);
        std::cerr<<"Rho = "<<rho<<"\t tita = "<<tita<<"\t y0 = "<<y0<<"\t y1 = "<<y1<<'\n';

        (imagen, filtrada, binaria, t_hough
         ).display();
 	}

    return 0;
}
