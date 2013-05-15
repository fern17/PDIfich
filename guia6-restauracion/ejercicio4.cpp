#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <complex>
#include "../utils/fourier.cpp"
#include "../utils/PDI_functions.h"
#include "../utils/filtradoFrecuencia.cpp" 
using namespace cimg_library;   //Necesario

bool dentroCirculo(int x, int y, int x0, int y0, unsigned int radio);

//@ Genera una máscara con un filtro rechazabanda entre las frecuencias f0 y ff
CImg<double> mascaraRechazaBanda(CImg<double> &img, unsigned int f0, unsigned int ff ) {
    CImg<double> ret_val(img.width(), img.height(), 1,1,1);

	cimg_forXY(img, x, y) {
		bool dentro_grande = dentroCirculo((int) x, (int) y, img.width()/2, img.height()/2, ff );
		bool dentro_chico = !dentroCirculo((int) x, (int) y, img.width()/2, img.height()/2, f0 );

		if (dentro_grande && dentro_chico)
			ret_val(x,y) = 0;
	}
	return ret_val;
}

//@ Lee los centros de circulos desde un archivo
std::vector<std::pair<unsigned int, unsigned int> > leerCentros(std::string file) {
    ifstream f (file.c_str());
    if (!f.is_open()) {
        std::cout<<"No pudo abrir el archivo "<<file<<"\n";
        exit(-1);
    }
    unsigned int cantidad;
    f>>cantidad;
    std::vector<std::pair<unsigned int, unsigned int> > ret_val;
    for (unsigned int i = 0; i < cantidad; i++) {
        unsigned int x;
        unsigned int y;
        f>>x>>y;
        ret_val.push_back(std::make_pair<unsigned int, unsigned int>(x,y));
    }
    f.close();
    return ret_val;
}

//@ Lee los radios de los circulos desde un archivo
std::vector<unsigned int> leerRadios(std::string file) {
    ifstream f (file.c_str());
    if (!f.is_open()) {
        std::cout<<"No pudo abrir el archivo "<<file<<"\n";
        exit(-1);
    }
    unsigned int cantidad;
    f>>cantidad;
    std::vector<unsigned int> ret_val;
    for (unsigned int i = 0; i < cantidad; i++) {
        unsigned int r;
        f>>r;
        ret_val.push_back(r);
    }
    f.close();
    return ret_val;
}

//@ Realiza un filtro rechaza banda ideal con ancho W y ubicado al radio especificado D0
CImgList<double> filtroRechazaBandaIdeal(unsigned int w, unsigned int h, unsigned int W, unsigned int D0, bool pasabanda = false) {
    //El radio es el centro de la corona. La corona tiene ancho W, y sus limites estan a W/2 del radio
    CImgList<double> img(2, w, h, 1, 1, 0);
    if (pasabanda)
        img[0].fill(1.0);

    unsigned int centro_u = w/2;
    unsigned int centro_v = h/2;
    
    cimg_forXY(img[0],u,v) {
        double distancia = sqrt( (u - centro_u)*(u - centro_u) + (v - centro_v)*(v - centro_v));
        if ((distancia < D0 - W/2) or (distancia > D0 + W/2)) {
            if (pasabanda)
                img[0](u, v) = 0.0;
            else
                img[0](u, v) = 1.0;
        }
    }

    img[0].shift(-w/2, -h/2,0,0,2);
    return img;
}

//@ Realiza un filtro rechaza banda de Butterworth con ancho W y ubicado al radio especificado
CImgList<double> filtroRechazaBandaButterworth(unsigned int w, unsigned int h, unsigned int W, unsigned int D0, unsigned int orden, bool pasabanda = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);
    if (pasabanda)
        img[0].fill(1.0);

    unsigned int centro_u = w/2;
    unsigned int centro_v = h/2;
    unsigned int D0s = D0*D0;
    
    cimg_forXY(img[0],u,v) {
        double distancia = sqrt( (u - centro_u)*(u - centro_u) + (v - centro_v)*(v - centro_v));
        double valor = (distancia * W) / (distancia * distancia - D0s);
        if (pasabanda)
            img[0](u,v) = 1 - (1 / (1 + std::pow(valor, 2*orden)));
        else
            img[0](u,v) = (1) / (1 + std::pow(valor, 2*orden));
    }

    img[0].shift(-w/2, -h/2, 0, 0, 2);
    return img;
}

//@ Realiza un filtro rechaza banda Gaussiano con ancho W y ubicado al radio especificado
CImgList<double> filtroRechazaBandaGaussiano(unsigned int w, unsigned int h, unsigned int W, unsigned int D0, bool pasabanda = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);
    if (pasabanda)
        img[0].fill(1.0);

    unsigned int centro_u = w/2;
    unsigned int centro_v = h/2;
    unsigned int D0s = D0*D0;
    
    cimg_forXY(img[0],u,v) {
        double distancia = sqrt( (u - centro_u)*(u - centro_u) + (v - centro_v)*(v - centro_v));
        double valor = -0.5*std::pow((distancia*distancia - D0s) / (distancia * W), 2);
        if (pasabanda)
            img[0](u,v) = std::exp(valor);
        else
            img[0](u,v) = 1 - std::exp(valor);
    }
    
    img[0].shift(-w/2, -h/2, 0, 0, 2);
    return img;
}

bool dentroCirculo(int x, int y, int x0, int y0, unsigned int radio) {
    return std::pow(x - x0, 2) + std::pow(y - y0, 2) - radio*radio < 0;
}

//@ Realiza un filtro notch para un vector de centros y radios
CImgList<double> filtroNotchAdHoc(unsigned int w, unsigned int h, std::vector<std::pair<unsigned int, unsigned int> > centros, std::vector<unsigned int> radios, bool pasante = false) {
    //Inicializamos todo en 1
    CImgList<double> img(2, w, h, 1, 1, 1);
    if (pasante)
        img[0].fill(0); //Si es pasante, se inicializa con 0

    cimg_forXY(img[0], x, y) {
        for (unsigned int i = 0; i < centros.size(); i++) {
            if ( dentroCirculo(int(x), int(y), int(centros[i].first), int(centros[i].second), radios[i])) {
                if (pasante) //si es pasante, solo se pone en 1 si esta dentro del circulo
                    img[0](x,y) = 1.0;
                else        //si no es pasante, se pone en 0 solo en los circulos
                    img[0](x,y) = 0.0;
            }
        }
    }
    //La parte imaginaria se queda todo en 0
    img[1].fill(0.0);

    img[0].shift(-w/2, -h/2, 0, 0, 2);

    return img;
}

int main(int argc, char *argv[]) {
    //@ Aplica filtro notch pero no anda 
	const char* input = cimg_option("-i", "../images/img_degradada.tif", "Input Image File");
	const char* input2 = cimg_option("-i", "../images/img.tif", "Input Image File");
	const char* f_centros = cimg_option("-c", "centros_ej4_1.txt", "Input Image File");
	const char* f_radios = cimg_option("-r", "radios_ej4_1.txt", "Input Image File");
    const unsigned int W = cimg_option("-w", 25, "Ancho del filtro");
    const unsigned int D0 = cimg_option("-d", 115, "Banda de centrado de rechazo del filtro");
    const unsigned int orden = cimg_option("-o", 2, "Orden del filtro de Butterworth");

    //Cargamos la imagen
    CImg<double> img(input);
    CImg<double> img_original(input2);
    unsigned int w = img.width();
    unsigned int h = img.height();

    //Calculamos los filtros 
    CImgList<double> filtro_ideal_rb = filtroRechazaBandaIdeal(w, h, W, D0);
    CImgList<double> filtro_butterworth_rb = filtroRechazaBandaButterworth(w, h, W, D0, orden);
    CImgList<double> filtro_gaussiano_rb = filtroRechazaBandaGaussiano(w, h, W, D0);

    //Leemos los parametros para el filtro ad hoc
    std::vector<unsigned int> radios = leerRadios(f_radios);
    std::vector<std::pair<unsigned int, unsigned int> > centros = leerCentros(f_centros);
    
    //Creamos el filtro ad hoc
    CImgList<double> filtro_adhoc = filtroNotchAdHoc(w, h, centros, radios);
    
    //Aplicamos los filtros
    CImg<double> resultado_ideal = utils::filtradoFrecuencia(img, filtro_ideal_rb);
    CImg<double> resultado_butterworth = utils::filtradoFrecuencia(img, filtro_butterworth_rb);
    CImg<double> resultado_gaussiano = utils::filtradoFrecuencia(img, filtro_gaussiano_rb);
    CImg<double> resultado_adhoc = utils::filtradoFrecuencia(img, filtro_adhoc);

    //Muestra de los errores
    std::cerr<<"\nErrores:";
    std::cout<<"\n\tOriginal con Degradada = "<<img_original.MSE(img);
    std::cout<<"\n\tOriginal con Ideal = "<<img_original.MSE(resultado_ideal);
    std::cout<<"\n\tOriginal con Butterworth = "<<img_original.MSE(resultado_butterworth);
    std::cout<<"\n\tOriginal con Gaussiana = "<<img_original.MSE(resultado_gaussiano);
    std::cout<<"\n\tOriginal con Ad Hoc = "<<img_original.MSE(resultado_adhoc);
    std::cerr<<"\n";
    
    //Dibujamos los rechaza banda
    (img, 
        filtro_ideal_rb[0], resultado_ideal, 
        filtro_butterworth_rb[0], resultado_butterworth, 
        filtro_gaussiano_rb[0], resultado_gaussiano,
        img_original).display();

    //Dibujamos el ad hoc
    (img, filtro_adhoc[0], resultado_adhoc, img_original).display();
 

    //A continuacion se van a hacer los filtros inversos
    
    //Calculamos el filtro para aislar el ruido
    //Calculamos los filtros pasabanda 
    CImgList<double> filtro_ideal_pb = filtroRechazaBandaIdeal(w, h, W, D0, true);
    CImgList<double> filtro_butterworth_pb = filtroRechazaBandaButterworth(w, h, W, D0, orden, true);
    CImgList<double> filtro_gaussiano_pb = filtroRechazaBandaGaussiano(w, h, W, D0, true);
    CImgList<double> filtro_adhoc_pasante = filtroNotchAdHoc(w, h, centros, radios, true);
   
    //Aplicamos los filtros pasantes
    CImg<double> ruido_ideal = utils::filtradoFrecuencia(img, filtro_ideal_pb);
    CImg<double> ruido_butterworth = utils::filtradoFrecuencia(img, filtro_butterworth_pb);
    CImg<double> ruido_gaussiano = utils::filtradoFrecuencia(img, filtro_gaussiano_pb);
    CImg<double> ruido_adhoc = utils::filtradoFrecuencia(img, filtro_adhoc_pasante);

    //Dibujamos
    (ruido_ideal, ruido_butterworth, ruido_gaussiano, ruido_adhoc).display();

    return 0;
}
