#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>
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

//@ Devuelve la mediana de un vector calculando floor
double mediana(std::vector<double> V) {
    std::sort(V.begin(),V.end());
    return V[std::floor(V.size()/2)];
}

double puntoMedio(std::vector<double> V) {
    double maxi = *(std::max(V.begin(), V.end()));
    double mini = *(std::min(V.begin(), V.end()));
    return 0.5*(maxi + mini);
}

double alfaRecortado(std::vector<double> V, unsigned int alfa) {
    std::sort(V.begin(), V.end());
    std::vector<double> copia;
    for (unsigned int i = std::floor(alfa/2); i < V.size() - std::floor(alfa/2); i++) {
        copia.push_back(V[i]);
    }
    double ret_val = copia[std::floor(copia.size()/2)] / double(V.size() - alfa);
    return ret_val;
}

//@ Calcula todos los filtros de orden
//Tipo 0 = Mediana
//Tipo 1 = Punto medio
//Tipo 2 = Alfa Recortado
template<typename T>
CImg<T> filtradoOrden(CImg<T> img, unsigned int tipo = 0, unsigned int ancho = 3, unsigned int alto = 3, unsigned int alfa = 4) {

    CImg<T> ret_val = img;
    unsigned int h = img.height(); 
    unsigned int w = img.width();  
    unsigned int step_x = ancho/2;              //ancho del vecindario centrado
    unsigned int step_y = alto/2;               //alto del vecindario centrado
    
    for (unsigned int i = 0; i < img.width(); i++) {
        for (unsigned int j = 0; j < img.height(); j++) {
            std::vector<T> vecindario = obtenerVecindario(img, i, j, w, h, step_x, step_y);
            if (tipo == 0)
                ret_val(i,j) = mediana(vecindario);
            if (tipo == 1)
                ret_val(i,j) = puntoMedio(vecindario);
            if (tipo == 2)
                ret_val(i,j) = alfaRecortado(vecindario, alfa);
        }
    }
    
    return ret_val;
}

int main(int argc, char *argv[]) {
    //@ Calcula los filtros de medias para una imagen
	const char* input = cimg_option("-i", "../images/sangre.jpg", "Input Image File");
    const double desvio_impulsivo = cimg_option("-s", 20.0, "Desvio sal y pimienta");
    const double desvio_gaussiano = cimg_option("-g", 20.0, "Desvio Gaussiano");
	const unsigned int ancho = cimg_option("-w", 3, "Input Image File");
	const unsigned int alto = cimg_option("-a", 3, "Input Image File");
	const unsigned int alfa = cimg_option("-f", 2, "Input Image File");

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
    std::cout<<"Aplicando mediana\n";
    CImg<double> resultado_mediana = filtradoOrden(img_ruidosa, 0, ancho, alto).normalize(0,255);

    std::cout<<"Aplicando punto medio\n";
    CImg<double> resultado_puntomedio = filtradoOrden(img_ruidosa, 1, ancho, alto).normalize(0,255);

    std::cout<<"Aplicando alfarecortado\n";
    CImg<double> resultado_alfarecortado = filtradoOrden(img_ruidosa, 2, ancho, alto, alfa).normalize(0,255);

    std::cout<<"Aplicando mediana y punto medio\n";
    CImg<double> resultado_mpm = filtradoOrden(resultado_mediana, 1, ancho, alto).normalize(0,255);

    CImg<double> r_mediana = cambiarIntesidad(img_original, resultado_mediana);
    CImg<double> r_puntomedio = cambiarIntesidad(img_original, resultado_puntomedio);
    CImg<double> r_alfarecortado = cambiarIntesidad(img_original, resultado_alfarecortado);
    CImg<double> r_mpm = cambiarIntesidad(img_original, resultado_mpm); 

    std::cout<<"\nErrores:\n";
    std::cout<<"\nImagen ruidosa = "<<img_ruidosa.MSE(img);
    std::cout<<"\nMediana = "<<resultado_mediana.MSE(img);
    std::cout<<"\nPunto Medio = "<<resultado_puntomedio.MSE(img);
    std::cout<<"\nAlfa Recortado = "<<resultado_alfarecortado.MSE(img);
    std::cout<<"\nResultado Mediana y Punto Medio = "<<resultado_mpm.MSE(img);
    std::cerr<<"\n";

    (img_original, img_ruidosa, r_mediana, r_puntomedio, r_alfarecortado, r_mpm).display();

    return 0;
}
