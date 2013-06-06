#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include "MyCImg.cpp"
using namespace cimg_library;   //Necesario

int main(int argc, char *argv[]) {
 	const unsigned char par1 = cimg_option("-j", 6, "p1");
    const double ang = cimg_option("-a", 3.14, "angulo");

    std::vector<std::string> archivos;

 	archivos.push_back("img/01.jpg");
 	archivos.push_back("img/02.jpg");

    CImg<double> imagen(200,200), imagen2(200,200);
    cimg_forX(imagen,x) {
        unsigned int pos = imagen.width() - 2*x;
        if (pos < imagen.height())
            imagen(x,pos) = 200;
        imagen2(x,x) = 200;
    }
    CImg<double> th = hough(imagen);
 // double maxi = th.max();
 // unsigned int tita;
 // unsigned int rho;
 // cimg_forXY(th,x,y) {
 //     if (fabs(maxi - th(x,y)) < EPS) {
 //         tita = x;
 //         rho = y;
 //     }
 // }
 // double diagonal = std::sqrt(pow(imagen.width(),2) + pow(imagen.height(),2));
 // double step_rho = 2.0 * diagonal/(imagen.height()-1);
 // double step_tita = M_PI/(imagen.width()-1);
 // double _rho = rho*step_rho - diagonal;
 // double _tita = tita*step_tita - M_PI/2;

    //double angulo = 180.0/double(imagen.width())*tita - 90.0;
    double angulo = houghAngulo(th);
    double distancia = houghRho(th);
    
  //std::cout<<"diag\tstep_tita\tstep_rho\t_tita\ttita_dg\t_rho\n";
  //std::cerr<<diagonal<<'\t'<<step_tita<<'\t'<<step_rho<<'\t'<<_tita<<'\t'<<_tita*180<<'\t'<<_rho<<'\n';
  //std::cerr<<tita<<' '<<rho<<' '<<maxi<<' '<<angulo<<'\n';
    std::cout<<"Angulo = "<<angulo<<'\n';
    CImg<double> imagen3 = imagen.get_rotate(angulo-90);
    (th, imagen, imagen2, imagen3).display();
    return 0;

 	for (int i = 0; i < archivos.size(); i++){
        CImg<unsigned char> img(archivos[i].c_str());
        unsigned int h = img.height();
        unsigned int w = img.width();

        (img).display();
 	}

    return 0;
}
