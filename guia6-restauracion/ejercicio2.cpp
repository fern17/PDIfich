#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
using namespace cimg_library;   //Necesario

const double EPSILON = 0.001;
//Helper que te dice si un valor se va de rosca
bool fueraDeRango(int x, int y, int w, int h) {
    if (y < 0 or y > h) 
        return true;
    if (x < 0 or x > w)
        return true;
    return false;
}

//Templatizado porque me gusta la reusabilidad
template<typename T>
//@ Devuelve el vecindario centrado en x0,y0 y de tamanio step_x*2 x step_y*2...
//Los parametros se castean a int porque en los for hay resta que se autocastea al tipo de los operandos
std::vector<double> obtenerVecindario(CImg<T> &img, int x0, int y0, unsigned int w, unsigned int h, int step_x, int step_y) {

    std::vector<double> ret_val;                            

    for (int i = x0 - step_x; i <= x0 + step_x; i++) {
        for (int j = y0 - step_y; j <= y0 + step_y; j++) {
            if (not fueraDeRango(i, j, w, h)) {            //Evita acceder a elementos fuera de la imagen
                ret_val.push_back(img(i,j));
            }
        }
    }
    return ret_val; 
}

//@ Calcula la media geometrica para un vecindario dado
template<typename T>
double mediaGeometrica(std::vector<T> vecindario) {
    double producto = 1.0;

    //recorre el vecindario
    for (unsigned int ii = 0; ii < vecindario.size(); ii++) {
        producto *= vecindario[ii];
    }

    //Calcula raiz nm-esima
    producto = std::pow(producto,1.0/vecindario.size());
    return producto;
}

//@ Calcula la media Contra Armonica de un vecindario, para un valor de Q dado. Contempla los casos especiales
template<typename T>
double mediaContraArmonica(std::vector<T> vecindario, double Q) {
    //Inicializamos los contadores
    double suma_arriba = 0.0;
    double suma_abajo = 0.0;

    //Recorremos el vecindario
    for (unsigned int ii = 0; ii < vecindario.size(); ii++) {
        //Si el valor actual no es muy chico, exponencio, si no, es lo mismo que no sumar nada
        if (std::fabs(vecindario[ii]) > EPSILON) {
            if (Q != -1) //para cuando NO es media armonica
                suma_arriba += std::pow(vecindario[ii], Q+1);
            if (Q != 0)  //para cuando NO es media aritmetica
                suma_abajo  += std::pow(vecindario[ii], Q);
        }
    }
    
    //Normalizamos si el vecindario tenia algun valor
    if (vecindario.size() > 0) {
        suma_arriba /= (float) vecindario.size();
        suma_abajo  /= (float) vecindario.size();
    }
    
    double resultado;
    //Fix para media armonica, donde suma_arriba se hace 0
    if (Q == -1)
        resultado = 1.0;
    else
        resultado = suma_arriba;
    
    //Dividimos si suma_abajo tenia algun valor
    if (fabs(suma_abajo) > EPSILON)
        resultado /= suma_abajo;

    return resultado;
}

//@ Calcula la media armonica llamando a mediaContraArmonica con el parametro Q necesario
template<typename T>
double mediaArmonica(std::vector<T> vecindario) {
    return mediaContraArmonica(vecindario, -1);
}

//@ Calcula la media aritmetica llamando a mediaContraArmonica con el parametro Q necesario
template<typename T>
double mediaAritmetica(std::vector<T> vecindario) {
    return mediaContraArmonica(vecindario, 0);
}

//@ Aplica el filtro de medias, para un Q determinado. Llama a los otros filtros de medias segun el parametro tipo:
//tipo = 0: Media aritmetica
//tipo = 1: Media geometrica
//tipo = 2: Media armonica
//tipo = 3: media contra armonica con el parametro Q necesario
template<typename T>
CImg<T> filtradoMedias(CImg<T> img, unsigned int tipo = 0, unsigned int ancho = 3, unsigned int alto = 3, double Q = 1.0) {

    CImg<T> ret_val = img;
    unsigned int h = img.height(); 
    unsigned int w = img.width();  
    unsigned int step_x = ancho/2;              //ancho del vecindario centrado
    unsigned int step_y = alto/2;               //alto del vecindario centrado
    
    for (unsigned int i = 0; i < img.width(); i++) {
        for (unsigned int j = 0; j < img.height(); j++) {
            std::vector<T> vecindario = obtenerVecindario(img, i, j, w, h, step_x, step_y);
            if (tipo == 0)
                ret_val(i,j) = mediaAritmetica(vecindario);
            if (tipo == 1)
                ret_val(i,j) = mediaGeometrica(vecindario);
            if (tipo == 2)
                ret_val(i,j) = mediaArmonica(vecindario);
            if (tipo == 3)
                ret_val(i,j) = mediaContraArmonica(vecindario, Q);
        }
    }
    
    return ret_val;
}

//@ Cambia el canal de intesidad de una imagen RGB, y la devuelve en RGB
template <typename T>
CImg<T> cambiarIntesidad(CImg<T> imagen, CImg<T> intesidad) {
    if (imagen.spectrum() == 1) 
        return imagen;
    
    //Convertimos a HSI
    CImg<T> img = imagen.get_normalize(0,1).get_RGBtoHSI();
    
    //Separamos los 2 canales que nos interesan
    CImg<T> canal1 = img.get_channel(0);
    CImg<T> canal2 = img.get_channel(1);

    //Componemos
    CImg<T> ret_val;
    ret_val = canal1;
    ret_val.append(canal2, 'c');
    intesidad.normalize(0,1);
    ret_val.append(intesidad, 'c');

    return ret_val.HSItoRGB().normalize(0,255);
}


int main(int argc, char *argv[]) {
    //@ Calcula los filtros de medias para una imagen
	const char* input = cimg_option("-i", "../images/sangre.jpg", "Input Image File");
    const double desvio_impulsivo = cimg_option("-s", 20, "Desvio sal y pimienta");
    const double desvio_gaussiano = cimg_option("-g", 20, "Desvio Gaussiano");
    const double Q = cimg_option("-q", 1.2, "Parametro media contra armonica");
	const unsigned int ancho = cimg_option("-w", 3, "Input Image File");
	const unsigned int alto = cimg_option("-h", 3, "Input Image File");

    //Cargamos la imagen
    CImg<double> img (input);
    CImg<double> img_original = img;

    //Nos quedamos con su canal de intesidad
    img = img.RGBtoHSI().channel(2);
    img.normalize(0,255);
    
    //Aplicamos ruido gaussiano e impulsivo
    CImg<double> img_ruidosa = img;
    img_ruidosa.noise(desvio_impulsivo, 2);
    img_ruidosa.noise(desvio_gaussiano, 0);
    img_ruidosa.normalize(0,255);
    img_ruidosa.save("img_ruidosa.jpg");
    
    //Aplicamos los filtros
    std::cout<<"Aplicando media geometrica\n";
    CImg<double> resultado_geometrica = filtradoMedias(img_ruidosa, 1, ancho, alto).normalize(0,255);
    resultado_geometrica.save("resultado_geometrica.jpg");
    
    std::cout<<"Aplicando media Contra armonica\n";
    CImg<double> resultado_contraarmonica = filtradoMedias(img_ruidosa, 3, ancho, alto, Q).normalize(0,255);
    resultado_contraarmonica.save("resultado_contraarmonica.jpg");

    std::cout<<"Aplicando media armonica\n";
    CImg<double> resultado_armonica = filtradoMedias(img_ruidosa, 2, ancho, alto).normalize(0,255);
    resultado_armonica.save("resultado_armonica.jpg");
  
    std::cout<<"Aplicando media aritmetica\n";
    CImg<double> resultado_aritmetica = filtradoMedias(img_ruidosa, 0, ancho, alto).normalize(0,255);
    resultado_aritmetica.save("resultado_artimetica.jpg");

    //Computamos los resultados en color
    CImg<double> r_geometrica = cambiarIntesidad(img_original, resultado_geometrica);
    CImg<double> r_contraarmonica = cambiarIntesidad(img_original, resultado_contraarmonica);
    CImg<double> r_armonica = cambiarIntesidad(img_original, resultado_armonica);
    CImg<double> r_aritmetica = cambiarIntesidad(img_original, resultado_aritmetica);

    std::cout<<"\nErrores:\n";
    std::cout<<"\nImagen ruidosa = "<<img_ruidosa.MSE(img);
    std::cout<<"\nGeometrica = "<<resultado_geometrica.MSE(img);
    std::cout<<"\nContra Armonica = "<<resultado_contraarmonica.MSE(img);
    std::cout<<"\nArmonica = "<<resultado_armonica.MSE(img);
    std::cout<<"\nAritmetica = "<<resultado_aritmetica.MSE(img);
    std::cerr<<"\n";

    (img_original, img_ruidosa, r_geometrica, r_contraarmonica, r_armonica, r_aritmetica).display();
    return 0;
}
