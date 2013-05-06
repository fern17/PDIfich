#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <vector>
#include <cmath>
using namespace cimg_library;   //Necesario

//Helper que te dice si un valor se va de rosca
template<typename T>
bool fueraDeRango(CImg<T> img, int x, int y, int w, int h) {
    if (y < 0 or y > w) 
        return true;
    if (x < 0 or x > h)
        return true;
    return false;
}

//Templatizado porque me gusta la reusabilidad
template<typename T>
//@ Devuelve el vecindario centrado en x0,y0 y de tamanio step_x*2 x step_y*2...
std::vector<double> obtenerVecindario(CImg<T> &img, unsigned int x0, unsigned int y0, unsigned int w, unsigned int h, unsigned int step_x, unsigned int step_y) {
    
    std::vector<double> ret_val; //Devuelve el vector de los pixeles del vecindario 

    for (int i = y0 - step_y; i <= y0 + step_y; i++) {        //Here be dragons... 
        for (int j = x0 - step_x; j <= x0 + step_x; j++) {    //aca tambien hay dragones, rojos
            if (not fueraDeRango(img, j, i, w, h)) 
                ret_val.push_back(img(j,i));
        }
    }
    return ret_val; 
}

template<typename T>
double mediaGeometrica(std::vector<T> vecindario) {
    double producto = 1.0;
    for (unsigned int ii = 0; ii < vecindario.size(); ii++) {
        producto *= vecindario[ii];
    }
    producto = pow(producto,1.0/vecindario.size());
    return producto;
}

template<typename T>
double mediaContraArmonica(std::vector<T> vecindario, double Q) {
    double suma_arriba = 0.0;
    double suma_abajo = 0.0;
    for (unsigned int ii = 0; ii < vecindario.size(); ii++) {
        if (Q != -1)
            suma_arriba += pow(vecindario[ii], Q+1);
        if (Q != 0)
            suma_abajo  += pow(vecindario[ii], Q);
    }
    suma_arriba /= (float) vecindario.size();
    suma_abajo  /= (float) vecindario.size();

    return suma_arriba/suma_abajo;
}

template<typename T>
double mediaArmonica(std::vector<T> vecindario) {
    return mediaContraArmonica(vecindario, -1);
}

template<typename T>
double mediaAritmetica(std::vector<T> vecindario) {
    return mediaContraArmonica(vecindario, 0);
}

//@ Aplica la media geometrica 
template<typename T>
CImg<T> filtradoMedias(CImg<T> img, unsigned int tipo = 0, unsigned int ancho = 3, unsigned int alto = 3, double Q = 1.0) {

    CImg<T> ret_val = img;
    unsigned int h = img.height(); 
    unsigned int w = img.width();  
    unsigned int step_x = ancho/2;              //ancho del vecindario centrado
    unsigned int step_y = alto/2;               //alto del vecindario centrado
    
    for (unsigned int i = 0; i < img.width(); i++) {
        for (unsigned int j = 0; j < img.height(); j++) {
            std::cout<<"iteracion "<<i<<" "<<j<<"\n";
            std::vector<T> vecindario = obtenerVecindario(img, j, i, w, h, step_x, step_y);
            if (tipo == 0)
                ret_val(j,i) = mediaAritmetica(vecindario);
            if (tipo == 1)
                ret_val(j,i) = mediaGeometrica(vecindario);
            if (tipo == 2)
                ret_val(j,i) = mediaArmonica(vecindario);
            if (tipo == 3)
                ret_val(j,i) = mediaContraArmonica(vecindario, Q);
        }
    }
    return ret_val;
}
int main(int argc, char *argv[]) {
	const char* input = cimg_option("-i", "../images/sangre.jpg", "Input Image File");
    const double desvio_impulsivo = cimg_option("-s", 20, "Desvio sal y pimienta");
    const double desvio_gaussiano = cimg_option("-g", 20, "Desvio Gaussiano");
    const double Q = cimg_option("-q", 1.0, "Parametro media contra armonica");
	const unsigned int ancho = cimg_option("-w", 3, "Input Image File");
	const unsigned int alto = cimg_option("-h", 3, "Input Image File");

    //Cargamos la imagen
    CImg<double> img (input);
    img = img.RGBtoHSI().channel(2);
    //Aplicamos ruido
    CImg<double> img_ruidosa = img.get_noise(desvio_impulsivo, 2);
    img_ruidosa.noise(desvio_gaussiano, 0);
    
    //Aplicamos los filtros
    std::cerr<<"wololo1\n";
    CImg<double> resultado_geometrica = filtradoMedias(img_ruidosa, 1, ancho, alto).normalize(0,255);
    std::cerr<<"wololo2\n";
    CImg<double> resultado_contraarmonica = filtradoMedias(img_ruidosa, 3, ancho, alto, Q).normalize(0,255);
    std::cerr<<"wololo3\n";
    CImg<double> resultado_armonica = filtradoMedias(img_ruidosa, 2, ancho, alto).normalize(0,255);
    std::cerr<<"wololo4\n";
    CImg<double> resultado_aritmetica = filtradoMedias(img_ruidosa, 0, ancho, alto).normalize(0,255);
    std::cerr<<"wololo5\n";

    (img, img_ruidosa, resultado_geometrica, resultado_contraarmonica, resultado_armonica, resultado_aritmetica).display();
    return 0;
}
